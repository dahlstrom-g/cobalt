/*
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/script/javascriptcore/jsc_debugger.h"

#include <cstdlib>
#include <string>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/logging.h"
#include "base/string_number_conversions.h"
#include "base/values.h"
#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/javascriptcore/jsc_global_object.h"
#include "cobalt/script/javascriptcore/jsc_global_object_proxy.h"
#include "cobalt/script/javascriptcore/jsc_object_handle_holder.h"

#include "third_party/WebKit/Source/JavaScriptCore/debugger/DebuggerCallFrame.h"
#include "third_party/WebKit/Source/JavaScriptCore/heap/MarkedBlock.h"
#include "third_party/WebKit/Source/JavaScriptCore/interpreter/CallFrame.h"
#include "third_party/WebKit/Source/JavaScriptCore/parser/SourceProvider.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/Executable.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSCell.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSFunction.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSGlobalData.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSGlobalObject.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSScope.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSValue.h"

namespace cobalt {
namespace script {

// Static factory method declared in public interface.
scoped_ptr<JavaScriptDebuggerInterface>
JavaScriptDebuggerInterface::CreateDebugger(
    GlobalObjectProxy* global_object_proxy, Delegate* delegate) {
  return scoped_ptr<JavaScriptDebuggerInterface>(
      new javascriptcore::JSCDebugger(global_object_proxy, delegate));
}

namespace javascriptcore {

namespace {
// Returns a string indicating the type of a scope object.
// One of the set of enumerated strings defined here:
// https://developer.chrome.com/devtools/docs/protocol/1.1/debugger#type-Scope
std::string GetScopeType(JSC::JSScope* scope) {
  int type = scope->structure()->typeInfo().type();
  std::string type_string;
  switch (type) {
    case JSC::GlobalObjectType:
      return "global";
    case JSC::ActivationObjectType:
      return "local";
    default: {
      DLOG(WARNING) << "Unexpected scope type: " << type;
      return "local";
    }
  }
}

// Conversions between intptr_t and std::string.
// Used to interchange sourceIds, callFrameIds, etc. with devtools.
// Assume we can store an intptr_t as an int64, as string_number_conversions
// does not directly support intptr_t.
std::string IntptrToString(intptr_t input) {
  COMPILE_ASSERT(sizeof(int64) >= sizeof(intptr_t),
                 int64_not_big_enough_to_store_intptr_t);
  int64 input_as_int64 = static_cast<int64>(input);
  return base::Int64ToString(input_as_int64);
}

intptr_t StringToIntptr(const std::string& input) {
  COMPILE_ASSERT(sizeof(int64) >= sizeof(intptr_t),
                 int64_not_big_enough_to_store_intptr_t);
  int64 input_as_int64 = 0;
  base::StringToInt64(input, &input_as_int64);
  return static_cast<intptr_t>(input_as_int64);
}

// Event "methods" (names) from the set here:
// https://developer.chrome.com/devtools/docs/protocol/1.1/debugger
const char kScriptFailedToParse[] = "Debugger.scriptFailedToParse";
const char kScriptParsed[] = "Debugger.scriptParsed";
}  // namespace

JSCDebugger::JSCDebugger(GlobalObjectProxy* global_object_proxy,
                         Delegate* delegate)
    : global_object_proxy_(global_object_proxy),
      delegate_(delegate),
      pause_on_next_statement_(false),
      pause_on_call_frame_(NULL),
      current_call_frame_(NULL),
      current_source_id_(0),
      current_line_number_(0),
      current_column_number_(0),
      is_paused_(false) {
  attach(GetGlobalObject());
}

JSCDebugger::~JSCDebugger() { detach(GetGlobalObject()); }

scoped_ptr<base::DictionaryValue> JSCDebugger::GetScriptSource(
    const scoped_ptr<base::DictionaryValue>& params) {
  DCHECK(thread_checker_.CalledOnValidThread());
  scoped_ptr<base::DictionaryValue> response(new base::DictionaryValue);

  // Get the scriptId from the parameters object.
  std::string script_id_string = "0";
  params->GetString("scriptId", &script_id_string);
  intptr_t script_id = StringToIntptr(script_id_string);

  // Search the set of source providers for a matching scriptId.
  JSC::SourceProvider* source_provider = NULL;
  for (SourceProviderSet::iterator iter = source_providers_.begin();
       iter != source_providers_.end(); ++iter) {
    if ((*iter)->asID() == script_id) {
      source_provider = *iter;
      break;
    }
  }
  if (source_provider) {
    response->SetString("result.scriptSource",
                        source_provider->source().latin1().data());
  } else {
    response->SetString("error.message",
                        "No source provider found with specified scriptId");
  }

  return response.Pass();
}

void JSCDebugger::Pause() {
  pause_on_next_statement_ = true;
  pause_on_call_frame_ = NULL;
}

void JSCDebugger::Resume() {
  pause_on_next_statement_ = false;
  pause_on_call_frame_ = NULL;
}

void JSCDebugger::StepInto() {
  pause_on_next_statement_ = true;
  pause_on_call_frame_ = NULL;
}

void JSCDebugger::StepOut() {
  pause_on_next_statement_ = false;
  const JSC::CallFrame* call_frame = current_call_frame_.callFrame();
  pause_on_call_frame_ = call_frame ? call_frame->callerFrame() : NULL;

  // If we are already at the top of the call stack, resume normal script
  // execution.
  delegate_->OnScriptDebuggerResume();
}

void JSCDebugger::StepOver() {
  pause_on_next_statement_ = false;
  pause_on_call_frame_ = current_call_frame_.callFrame();
  DCHECK(pause_on_call_frame_);
}

void JSCDebugger::GathererFunctor::operator()(JSC::JSCell* cell) {
  JSC::JSFunction* function = JSC::jsDynamicCast<JSC::JSFunction*>(cell);
  if (function && !function->isHostFunction() &&
      function->scope()->globalObject() == global_object_ &&
      function->executable()->isFunctionExecutable()) {
    source_providers_->add(
        JSC::jsCast<JSC::FunctionExecutable*>(function->executable())
            ->source()
            .provider());
  }
}

scoped_ptr<base::DictionaryValue> JSCDebugger::CreateCallFrameData(
    const JSC::DebuggerCallFrame& call_frame,
    scoped_ptr<base::DictionaryValue> location,
    scoped_ptr<base::ListValue> scope_chain) {
  DCHECK(call_frame.callFrame());
  DCHECK(location);
  DCHECK(scope_chain);

  scoped_ptr<base::DictionaryValue> call_frame_data(
      new base::DictionaryValue());
  call_frame_data->SetString(
      "callFrameId",
      IntptrToString(reinterpret_cast<intptr_t>(call_frame.callFrame())));
  call_frame_data->SetString("functionName",
                             call_frame.functionName().latin1().data());
  call_frame_data->Set("location", location.release());
  call_frame_data->Set("scopeChain", scope_chain.release());

  // Add the "this" object for the call frame.
  JSC::JSObject* this_object = call_frame.thisObject();
  scoped_ptr<base::DictionaryValue> this_object_data(
      CreateRemoteObjectData(this_object));
  if (this_object_data) {
    call_frame_data->Set("this", this_object_data.release());
  }

  return call_frame_data.Pass();
}

scoped_ptr<base::ListValue> JSCDebugger::CreateCallStackData(
    JSC::CallFrame* call_frame) {
  DCHECK(call_frame);

  scoped_ptr<base::ListValue> call_frame_list(new base::ListValue());
  intptr_t source_id = current_source_id_;
  int line_number = current_line_number_;
  int column_number = current_column_number_;
  String url;
  JSC::JSValue function;

  // Iterate over the call frames on the stack.
  while (call_frame && !call_frame->hasHostCallFrameFlag()) {
    // Create data for the call frame.
    scoped_ptr<base::DictionaryValue> location_data(
        CreateLocationData(source_id, line_number, column_number));
    scoped_ptr<base::ListValue> scope_chain_data(
        CreateScopeChainData(call_frame));
    scoped_ptr<base::DictionaryValue> call_frame_data(
        CreateCallFrameData(JSC::DebuggerCallFrame(call_frame),
                            location_data.Pass(), scope_chain_data.Pass()));

    // Add the call frame data to the list.
    DCHECK(call_frame_data);
    call_frame_list->Append(call_frame_data.release());

    // Move on to the next call frame.
    call_frame->interpreter()->retrieveLastCaller(call_frame, line_number,
                                                  source_id, url, function);
    call_frame = call_frame->callerFrame();
  }

  return call_frame_list.Pass();
}

scoped_ptr<base::DictionaryValue> JSCDebugger::CreateLocationData(
    intptr_t source_id, int line_number, int column_number) {
  scoped_ptr<base::DictionaryValue> location_data(new base::DictionaryValue());
  location_data->SetString("scriptId", IntptrToString(source_id));
  location_data->SetInteger("columnNumber", column_number);
  location_data->SetInteger("lineNumber", line_number);
  return location_data.Pass();
}

scoped_ptr<base::DictionaryValue> JSCDebugger::CreateRemoteObjectData(
    JSC::JSObject* js_object) {
  DCHECK(js_object);
  JSCObjectHandleHolder object_holder(
      JSCObjectHandle(js_object), GetGlobalObject()->script_object_registry());
  scoped_ptr<base::DictionaryValue> remote_object_params(
      new base::DictionaryValue());
  remote_object_params->SetString("objectGroup", "debugger-api");
  remote_object_params->SetBoolean("returnByValue", false);
  return delegate_->CreateRemoteObject(&object_holder, remote_object_params);
}

scoped_ptr<base::ListValue> JSCDebugger::CreateScopeChainData(
    const JSC::CallFrame* call_frame) {
  DCHECK(call_frame);
  scoped_ptr<base::ListValue> scope_chain_data(new base::ListValue());

  // Iterate over the scope chain for the call frame.
  for (JSC::JSScope* scope = call_frame->scope(); scope;
       scope = scope->next()) {
    scoped_ptr<base::DictionaryValue> scope_data(CreateScopeData(scope));
    DCHECK(scope_data);
    scope_chain_data->Append(scope_data.release());
  }

  return scope_chain_data.Pass();
}

scoped_ptr<base::DictionaryValue> JSCDebugger::CreateScopeData(
    JSC::JSScope* scope) {
  DCHECK(scope);
  JSC::JSObject* scope_object = JSC::JSScope::objectAtScope(scope);
  DCHECK(scope_object);
  scoped_ptr<base::DictionaryValue> scope_object_data(
      CreateRemoteObjectData(scope));

  if (!scope) {
    DLOG(WARNING) << "Cannot create Scope data.";
    return scoped_ptr<base::DictionaryValue>(NULL);
  }

  scoped_ptr<base::DictionaryValue> scope_data(new base::DictionaryValue());
  scope_data->Set("object", scope_object_data.release());
  scope_data->SetString("type", GetScopeType(scope));
  return scope_data.Pass();
}

JSCGlobalObject* JSCDebugger::GetGlobalObject() const {
  return base::polymorphic_downcast<JSCGlobalObjectProxy*>(global_object_proxy_)
      ->global_object();
}

void JSCDebugger::attach(JSC::JSGlobalObject* global_object) {
  DCHECK(global_object);
  JSC::Debugger::attach(global_object);

  // Gather the source providers and call sourceParsed() on each one.
  GatherSourceProviders(global_object);
  for (SourceProviderSet::iterator iter = source_providers_.begin();
       iter != source_providers_.end(); ++iter) {
    sourceParsed(global_object->globalExec(), *iter, -1, String());
  }
}

void JSCDebugger::detach(JSC::JSGlobalObject* global_object) {
  DCHECK(global_object);
  JSC::Debugger::detach(global_object);
  delegate_->OnScriptDebuggerDetach("canceled_by_user");
}

void JSCDebugger::sourceParsed(JSC::ExecState* exec_state,
                               JSC::SourceProvider* source_provider,
                               int error_line,
                               const WTF::String& error_message) {
  UNREFERENCED_PARAMETER(exec_state);
  DCHECK(source_provider);

  // Build the event parameters.
  scoped_ptr<base::DictionaryValue> params(new base::DictionaryValue());
  params->SetString("scriptId", IntptrToString(source_provider->asID()));
  params->SetString("url", source_provider->url().latin1().data());

  // TODO(***REMOVED***) There are a bunch of other parameters I don't know how
  // to get from a SourceProvider: start/end line/column for inline scripts,
  // etc. Need to check whether it is possible to find these somwhere, or if
  // Chrome Dev Tools can manage without these, dummy values would suffice, etc.

  // If there was an error, add the parameters for that.
  if (error_line >= 0) {
    params->SetInteger("errorLine", error_line);
    params->SetString("errorMessage", error_message.latin1().data());
  }

  if (error_line < 0) {
    delegate_->OnScriptDebuggerEvent(kScriptParsed, params);
  } else {
    delegate_->OnScriptDebuggerEvent(kScriptFailedToParse, params);
  }
}

void JSCDebugger::exception(const JSC::DebuggerCallFrame& call_frame,
                            intptr_t source_id, int line_number,
                            int column_number, bool has_handler) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::atStatement(const JSC::DebuggerCallFrame& call_frame,
                              intptr_t source_id, int line_number,
                              int column_number) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::callEvent(const JSC::DebuggerCallFrame& call_frame,
                            intptr_t source_id, int line_number,
                            int column_number) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::returnEvent(const JSC::DebuggerCallFrame& call_frame,
                              intptr_t source_id, int line_number,
                              int column_number) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::willExecuteProgram(const JSC::DebuggerCallFrame& call_frame,
                                     intptr_t source_id, int line_number,
                                     int column_number) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::didExecuteProgram(const JSC::DebuggerCallFrame& call_frame,
                                    intptr_t source_id, int line_number,
                                    int column_number) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::didReachBreakpoint(const JSC::DebuggerCallFrame& call_frame,
                                     intptr_t source_id, int line_number,
                                     int column_number) {
  UpdateAndPauseIfNeeded(call_frame, source_id, line_number, column_number);
}

void JSCDebugger::GatherSourceProviders(JSC::JSGlobalObject* global_object) {
  DCHECK(global_object);
  source_providers_.clear();
  GathererFunctor gatherer_functor(global_object, &source_providers_);
  JSC::JSGlobalData& global_data = global_object->globalData();
  global_data.heap.objectSpace().forEachLiveCell(gatherer_functor);
}

void JSCDebugger::UpdateAndPauseIfNeeded(
    const JSC::DebuggerCallFrame& call_frame, intptr_t source_id,
    int line_number, int column_number) {
  // Don't do anything if we're currently paused. We want to remember the call
  // frame and source location at the point we paused, not override them with
  // any debugging scripts that get evaluated while paused.
  if (is_paused_) {
    return;
  }

  UpdateSourceLocation(source_id, line_number, column_number);
  UpdateCallFrame(call_frame);
  PauseIfNeeded(call_frame);
}

void JSCDebugger::UpdateSourceLocation(intptr_t source_id, int line_number,
                                       int column_number) {
  current_source_id_ = source_id;
  current_line_number_ = line_number;
  current_column_number_ = column_number;
}

void JSCDebugger::UpdateCallFrame(const JSC::DebuggerCallFrame& call_frame) {
  current_call_frame_ = call_frame;
}

void JSCDebugger::PauseIfNeeded(const JSC::DebuggerCallFrame& call_frame) {
  // Determine whether we should pause.
  // TODO(***REMOVED***): More to handle here: exceptions, breakpoints, etc.
  bool will_pause = pause_on_next_statement_;
  will_pause |=
      pause_on_call_frame_ && pause_on_call_frame_ == call_frame.callFrame();
  if (!will_pause) {
    return;
  }

  // Set the |is_paused_| state for the remainder of this function.
  ScopedPausedState paused(this);

  // Notify the clients we're about to pause.
  SendPausedEvent(call_frame);

  // Delegate handles the actual blocking of the thread to implement Pause.
  delegate_->OnScriptDebuggerPause();

  // Notify the clients we've resumed.
  SendResumedEvent();
}

void JSCDebugger::SendPausedEvent(const JSC::DebuggerCallFrame& call_frame) {
  std::string event_method = "Debugger.paused";
  scoped_ptr<base::DictionaryValue> event_params(new base::DictionaryValue());
  scoped_ptr<base::ListValue> call_stack_data(
      CreateCallStackData(call_frame.callFrame()));
  DCHECK(call_stack_data);
  event_params->Set("callFrames", call_stack_data.release());
  event_params->SetString("reason", "debugCommand");
  delegate_->OnScriptDebuggerEvent(event_method, event_params);
}

void JSCDebugger::SendResumedEvent() {
  // Send the event to the clients. No parameters.
  std::string event_method = "Debugger.resumed";
  scoped_ptr<base::DictionaryValue> event_params(new base::DictionaryValue());
  delegate_->OnScriptDebuggerEvent(event_method, event_params);
}

}  // namespace javascriptcore
}  // namespace script
}  // namespace cobalt
