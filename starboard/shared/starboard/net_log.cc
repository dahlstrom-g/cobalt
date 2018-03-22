// Copyright 2018 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "starboard/shared/starboard/net_log.h"

#include <windows.h>

#include <algorithm>
#include <deque>
#include <functional>
#include <map>
#include <string>

#include "starboard/atomic.h"
#include "starboard/common/semaphore.h"
#include "starboard/common/thread.h"
#include "starboard/log.h"
#include "starboard/mutex.h"
#include "starboard/once.h"
#include "starboard/socket.h"
#include "starboard/socket_waiter.h"
#include "starboard/string.h"

#ifndef NET_LOG_PORT
// Default port was generated from a random number generator.
#define NET_LOG_PORT 49353
#endif

// Controls whether using IPv4 or IPv6.
#ifndef NET_LOG_IP_VERSION
#define NET_LOG_IP_VERSION kSbSocketAddressTypeIpv4
#endif

// Default Socket write buffer is 100k.
#ifndef NET_LOG_SOCKET_BUFFER_SIZE
#define NET_LOG_SOCKET_BUFFER_SIZE (1024 * 100)
#endif

// Default in memory write buffer is 1mb.
#ifndef NET_LOG_MAX_IN_MEMORY_BUFFER
#define NET_LOG_MAX_IN_MEMORY_BUFFER (1024 * 1024)
#endif

// Default block to send to the socket is 1k.
#ifndef NET_LOG_SOCKET_SEND_SIZE
#define NET_LOG_SOCKET_SEND_SIZE 1024
#endif

namespace starboard {
namespace shared {
namespace starboard {
namespace {

using JoinFunction = std::function<void()>;
using RunFunction = std::function<void(Semaphore*, atomic_bool*)>;
using std::placeholders::_1;
using std::placeholders::_2;

class FunctionThread : public Thread {
 public:
  static scoped_ptr<Thread> Create(
      const std::string& thread_name,
      RunFunction run_function,
      JoinFunction on_join_called = JoinFunction()) {
    scoped_ptr<Thread> out(
        new FunctionThread(thread_name, run_function, on_join_called));
    return out.Pass();
  }

  FunctionThread(const std::string& name,
                 RunFunction run_function,
                 JoinFunction join_function)
      : Thread(name),
        run_function_(run_function),
        join_function_(join_function) {
  }

  void Run() override {
    run_function_(join_sema(), joined_bool());
  }

  void Join() override {
    if (join_function_) {
      join_function_();
    }
    Thread::Join();
  }

 private:
  RunFunction run_function_;
  JoinFunction join_function_;
};

scoped_ptr<Socket> CreateListenSocket() {
  scoped_ptr<Socket> socket(
      new Socket(NET_LOG_IP_VERSION, kSbSocketProtocolTcp));
  socket->SetReuseAddress(true);
  SbSocketAddress sock_addr;
  // Ip address will be set to 0.0.0.0 so that it will bind to all sockets.
  SbMemorySet(&sock_addr, 0, sizeof(SbSocketAddress));
  sock_addr.type = NET_LOG_IP_VERSION;
  sock_addr.port = NET_LOG_PORT;
  SbSocketError sock_err = socket->Bind(&sock_addr);

  const char kErrFmt[] = "Socket error while attempting to bind, error = %d\n";
  // Can't use SB_LOG_IF() because SB_LOG_IF() might have triggered this call,
  // and therefore would triggered reentrant behavior and then deadlock.
  // SbLogRawFormat() is assumed to be safe to call. Note that NetLogWrite()
  // ignores reentrant calls.
  if (sock_err != kSbSocketOk) {
    SbLogRawFormatF(kErrFmt, sock_err);
  }
  sock_err = socket->Listen();
  if (sock_err != kSbSocketOk) {
    SbLogRawFormatF(kErrFmt, sock_err);
  }
  return socket.Pass();
}

// This class will listen to the provided socket for a client
// connection. When a client connection is established, a
// callback will be invoked.
class SocketListener {
 public:
  typedef std::function<void(scoped_ptr<Socket>)> Callback;

  SocketListener(Socket* listen_socket, Callback cb)
      : listen_socket_(listen_socket),
        callback_(cb) {
    RunFunction run_cb = std::bind(&SocketListener::Run, this, _1, _2);
    thread_ = FunctionThread::Create("NetLogSocketListener",
                                     run_cb);
    thread_->Start();
  }

  ~SocketListener() {
    thread_->Join();
  }

 private:
  void Run(Semaphore* joined_sema, atomic_bool*) {
    while (!joined_sema->TakeWait(100 * kSbTimeMillisecond)) {
      scoped_ptr<Socket> client_connection(listen_socket_->Accept());

      if (client_connection) {
        callback_(client_connection.Pass());
        break;
      }
    }
  }

  Socket* listen_socket_;
  Callback callback_;
  scoped_ptr<Thread> thread_;
};

class NetLogServer {
 public:
  static NetLogServer* Instance();
  NetLogServer() {
    ScopedLock lock(socket_mutex_);
    listen_socket_ = CreateListenSocket();
    ListenForClient();

    writer_thread_ = FunctionThread::Create(
        "NetLogSocketWriter",
        std::bind(&NetLogServer::WriterTask, this, _1, _2),
        std::bind(&NetLogServer::OnWriterTaskJoined, this));
    writer_thread_->Start();
  }

  ~NetLogServer() {
    SB_NOTREACHED();  // Should never reach here because of singleton use.
    Close();
  }

  void ListenForClient() {
    SocketListener::Callback cb = std::bind(&NetLogServer::OnClientConnect,
                                            this,
                                            std::placeholders::_1);
    socket_listener_.reset();
    socket_listener_.reset(new SocketListener(listen_socket_.get(), cb));
  }

  void OnClientConnect(scoped_ptr<Socket> client_socket) {
    ScopedLock lock(socket_mutex_);
    client_socket_ = client_socket.Pass();
    client_socket_->SetSendBufferSize(NET_LOG_SOCKET_BUFFER_SIZE);
  }

  // Has a client ever connected?
  bool HasClientConnected() {
    ScopedLock lock(socket_mutex_);
    return client_socket_;
  }

  void OnLog(const char* msg) {
    AppendToLog(msg);
    writer_thread_sema_.Put();
  }

  void Close() {
    writer_thread_->Join();
    writer_thread_.reset(nullptr);
    socket_listener_.reset();

    InternalFlush();  // One last flush to the socket.
    ScopedLock lock(socket_mutex_);
    client_socket_.reset();
    listen_socket_.reset();
  }

  bool Flush() { return InternalFlush(); }

 private:
  void OnWriterTaskJoined() {
    writer_thread_sema_.Put();
  }

  void WriterTask(Semaphore* /*joined_sema*/, atomic_bool* is_joined) {
    std::string curr_log_data;
    while (true) {
      writer_thread_sema_.Take();

      if (HasClientConnected()) {
        if (!Flush()) {
          break;  // Connection dropped.
        }
      }
      if (is_joined->load()) {
        break;
      }
    }
  }

  std::string ToString(SbSocketError error) {
    switch (error) {
      case kSbSocketOk: { return "kSbSocketOk"; }
      case kSbSocketPending: { return "kSbSocketErrorConnectionReset"; }
      case kSbSocketErrorFailed: { return "kSbSocketErrorFailed"; }

#if SB_HAS(SOCKET_ERROR_CONNECTION_RESET_SUPPORT) || \
    SB_API_VERSION >= 9
      case kSbSocketErrorConnectionReset: {
        return "kSbSocketErrorConnectionReset";
      }
#endif  // SB_HAS(SOCKET_ERROR_CONNECTION_RESET_SUPPORT) ||
        // SB_API_VERSION >= 9
    }
    SB_NOTREACHED() << "Unexpected case " << error;
    std::stringstream ss;
    ss << "Unknown-" << error;
    return ss.str();
  }

  // Return false if socket disconnected.
  bool InternalFlush() {
    ScopedLock lock(socket_mutex_);
    if (!client_socket_) {
      return false;
    }
    // Dummy read from client socket. This is required by the socket api.
    char buff[2048];
    client_socket_->ReceiveFrom(buff, sizeof(buff), NULL);
    std::string curr_write_block;
    while (TransferLog(NET_LOG_SOCKET_SEND_SIZE, &curr_write_block)) {
      while (!curr_write_block.empty()) {
        int bytes_to_write = static_cast<int>(curr_write_block.size());
        int result = client_socket_->SendTo(curr_write_block.c_str(),
                                            bytes_to_write,
                                            NULL);
        if (result < 0) {
          SbSocketError err = client_socket_->GetLastError();
          client_socket_->ClearLastError();
          if (err == kSbSocketPending) {
            // Buffer was full.
            SbThreadSleep(kSbTimeMillisecond);
            continue;
          } else {
            // Unexpected error. Clear the current write block to prevent
            // endless loop.
            SB_LOG(ERROR) << "An error happened while writing to socket: "
                          << ToString(err);
            curr_write_block.clear();
          }
          break;
        } else if (result == 0) {
          // Socket has closed.
          return false;
        } else {
          // Expected condition. Partial or full write was successful.
          size_t bytes_written = static_cast<size_t>(result);
          SB_DCHECK(bytes_written <= bytes_to_write);
          curr_write_block.erase(0, bytes_written);
        }
      }
    }
    return true;
  }

  bool TransferLog(size_t max_size, std::string* destination) {
    ScopedLock lock_log(log_mutex_);
    size_t log_size = log_.size();
    if (log_size == 0) {
      return false;
    }

    size_t size = std::min<size_t>(max_size, log_size);
    std::deque<char>::iterator begin_it = log_.begin();
    std::deque<char>::iterator end_it = begin_it;
    std::advance(end_it, size);

    destination->assign(begin_it, end_it);
    log_.erase(begin_it, end_it);
    return true;
  }

  void AppendToLog(const char* msg) {
    size_t str_len = SbStringGetLength(msg);
    bool overflow = false;
    log_mutex_.Acquire();
    for (const char* curr = msg; curr != msg + str_len; ++curr) {
      log_.push_back(*curr);
      if (log_.size() > NET_LOG_MAX_IN_MEMORY_BUFFER) {
        overflow = true;
        log_.pop_front();
      }
    }
    log_mutex_.Release();

    SB_LOG_IF(ERROR, overflow) << "Net log dropped buffer data.";
  }

  scoped_ptr<Socket> listen_socket_;
  scoped_ptr<Socket> client_socket_;
  Mutex socket_mutex_;
  std::deque<char> log_;
  Mutex log_mutex_;

  scoped_ptr<SocketListener> socket_listener_;
  scoped_ptr<Thread> writer_thread_;
  Semaphore writer_thread_sema_;
};

class ThreadLocalBoolean {
 public:
  ThreadLocalBoolean() : slot_(SbThreadCreateLocalKey(NULL)) {}
  ~ThreadLocalBoolean() { SbThreadDestroyLocalKey(slot_); }

  void Set(bool val) {
    void* ptr = val ? reinterpret_cast<void*>(0x1) : nullptr;
    SbThreadSetLocalValue(slot_, ptr);
  }

  bool Get() const {
    void* ptr = SbThreadGetLocalValue(slot_);
    return ptr != nullptr;
  }
 private:
  SbThreadLocalKey slot_;
  SB_DISALLOW_COPY_AND_ASSIGN(ThreadLocalBoolean);
};

SB_ONCE_INITIALIZE_FUNCTION(NetLogServer, NetLogServer::Instance);
SB_ONCE_INITIALIZE_FUNCTION(ThreadLocalBoolean, ScopeGuardTLB);

// Prevents re-entrant behavior for sending logs. This is useful if/when
// sub-routines will invoke logging on an error condition.
class ScopeGuard {
 public:
  ScopeGuard() : disabled_(false), tlb_(ScopeGuardTLB()) {
    disabled_ = tlb_->Get();
    tlb_->Set(true);
  }

  ~ScopeGuard() {
    tlb_->Set(disabled_);
  }

  bool IsEnabled() {
    return !disabled_;
  }
 private:
  bool disabled_;
  ThreadLocalBoolean* tlb_;
};

}  // namespace.

const char kNetLogCommandSwitchWait[] = "net_log_wait_for_connection";

void NetLogWaitForClientConnected() {
#if !SB_LOGGING_IS_OFFICIAL_BUILD
  ScopeGuard guard;
  if (guard.IsEnabled()) {
    while (!NetLogServer::Instance()->HasClientConnected()) {
      SbThreadSleep(kSbTimeMillisecond);
    }
  }
#endif
}

void NetLogWrite(const char* data) {
#if !SB_LOGGING_IS_OFFICIAL_BUILD
  ScopeGuard guard;
  if (guard.IsEnabled()) {
    NetLogServer::Instance()->OnLog(data);
  }
#endif
}

void NetLogFlush() {
#if !SB_LOGGING_IS_OFFICIAL_BUILD
  ScopeGuard guard;
  if (guard.IsEnabled()) {
    NetLogServer::Instance()->Flush();
  }
#endif
}

void NetLogFlushThenClose() {
#if !SB_LOGGING_IS_OFFICIAL_BUILD
  ScopeGuard guard;
  if (guard.IsEnabled()) {
    NetLogServer::Instance()->Close();
  }
#endif
}

}  // namespace starboard
}  // namespace shared
}  // namespace starboard
