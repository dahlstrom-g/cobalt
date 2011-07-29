// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_NSEXCEPTION_ENABLER_H_
#define BASE_MAC_SCOPED_NSEXCEPTION_ENABLER_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"

namespace base {
namespace mac {

// BrowserCrApplication attempts to restrict throwing of NSExceptions
// because they interact badly with C++ scoping rules.  Unfortunately,
// there are some cases where exceptions must be supported, such as
// when third-party printer drivers are used.  These helpers can be
// used to enable exceptions for narrow windows.

// Make it easy to safely allow NSException to be thrown in a limited
// scope.  Note that if an exception is thrown, then this object will
// not be appropriately destructed!  If the exception ends up in the
// top-level event loop, things are cleared in -reportException:.  If
// the exception is caught at a lower level, a higher level scoper
// should eventually reset things.
class BASE_API ScopedNSExceptionEnabler {
 public:
  ScopedNSExceptionEnabler();
  ~ScopedNSExceptionEnabler();

 private:
  bool was_enabled_;

  DISALLOW_COPY_AND_ASSIGN(ScopedNSExceptionEnabler);
};

// Access the exception setting for the current thread.  This is for
// the support code in BrowserCrApplication, other code should use
// the scoper.
BASE_API bool GetNSExceptionsAllowed();
BASE_API void SetNSExceptionsAllowed(bool allowed);

}  // namespace mac
}  // namespace base

#endif  // BASE_MAC_SCOPED_NSEXCEPTION_ENABLER_H_
