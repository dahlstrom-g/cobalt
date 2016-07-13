/*
 * Copyright 2016 Google Inc. All Rights Reserved.
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

// This file has been auto-generated by bindings/code_generator_cobalt.py. DO NOT MODIFY!
// Auto-generated from template: bindings/mozjs/templates/interface.cc.template

// clang-format off

#include "MozjsNumericTypesTestInterface.h"

#include "base/debug/trace_event.h"
#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/global_object_proxy.h"
#include "cobalt/script/opaque_handle.h"
#include "cobalt/script/script_object.h"

#include "base/lazy_instance.h"
#include "cobalt/script/mozjs/conversion_helpers.h"
#include "cobalt/script/mozjs/mozjs_exception_state.h"
#include "cobalt/script/mozjs/mozjs_global_object_proxy.h"
#include "cobalt/script/mozjs/mozjs_object_handle_holder.h"
#include "cobalt/script/mozjs/mozjs_callback_function.h"
#include "cobalt/script/mozjs/type_traits.h"
#include "cobalt/script/mozjs/wrapper_factory.h"
#include "cobalt/script/mozjs/wrapper_private.h"
#include "third_party/mozjs/js/src/jsapi.h"
#include "third_party/mozjs/js/src/jsfriendapi.h"

namespace {
using cobalt::bindings::testing::NumericTypesTestInterface;
using cobalt::bindings::testing::MozjsNumericTypesTestInterface;
using cobalt::script::CallbackInterfaceTraits;
using cobalt::script::GlobalObjectProxy;
using cobalt::script::OpaqueHandle;
using cobalt::script::OpaqueHandleHolder;
using cobalt::script::ScriptObject;
using cobalt::script::Wrappable;

using cobalt::script::CallbackFunction;
using cobalt::script::CallbackInterfaceTraits;
using cobalt::script::mozjs::FromJSValue;
using cobalt::script::mozjs::InterfaceData;
using cobalt::script::mozjs::MozjsCallbackFunction;
using cobalt::script::mozjs::MozjsExceptionState;
using cobalt::script::mozjs::MozjsGlobalObjectProxy;
using cobalt::script::mozjs::MozjsObjectHandleHolder;
using cobalt::script::mozjs::ToJSValue;
using cobalt::script::mozjs::TypeTraits;
using cobalt::script::mozjs::WrapperPrivate;
using cobalt::script::mozjs::WrapperFactory;
using cobalt::script::Wrappable;
}  // namespace

namespace cobalt {
namespace bindings {
namespace testing {

namespace {

InterfaceData* CreateCachedInterfaceData() {
  InterfaceData* interface_data = new InterfaceData();
  memset(&interface_data->instance_class_definition, 0,
         sizeof(interface_data->instance_class_definition));
  memset(&interface_data->prototype_class_definition, 0,
         sizeof(interface_data->prototype_class_definition));
  memset(&interface_data->interface_object_class_definition, 0,
         sizeof(interface_data->interface_object_class_definition));

  JSClass* instance_class = &interface_data->instance_class_definition;
  const int kGlobalFlags = 0;
  instance_class->name = "MozjsNumericTypesTestInterface";
  instance_class->flags = kGlobalFlags | JSCLASS_HAS_PRIVATE;
  instance_class->addProperty = JS_PropertyStub;
  instance_class->delProperty = JS_DeletePropertyStub;
  instance_class->getProperty = JS_PropertyStub;
  instance_class->setProperty = JS_StrictPropertyStub;
  instance_class->enumerate = JS_EnumerateStub;
  instance_class->resolve = JS_ResolveStub;
  instance_class->convert = JS_ConvertStub;
  // Function to be called before on object of this class is garbage collected.
  instance_class->finalize = &WrapperPrivate::Finalizer;

  JSClass* prototype_class = &interface_data->prototype_class_definition;
  prototype_class->name = "MozjsNumericTypesTestInterfacePrototype";
  prototype_class->flags = 0;
  prototype_class->addProperty = JS_PropertyStub;
  prototype_class->delProperty = JS_DeletePropertyStub;
  prototype_class->getProperty = JS_PropertyStub;
  prototype_class->setProperty = JS_StrictPropertyStub;
  prototype_class->enumerate = JS_EnumerateStub;
  prototype_class->resolve = JS_ResolveStub;
  prototype_class->convert = JS_ConvertStub;

  JSClass* interface_object_class = &interface_data->interface_object_class_definition;
  interface_object_class->name = "MozjsNumericTypesTestInterfaceConstructor";
  interface_object_class->flags = 0;
  interface_object_class->addProperty = JS_PropertyStub;
  interface_object_class->delProperty = JS_DeletePropertyStub;
  interface_object_class->getProperty = JS_PropertyStub;
  interface_object_class->setProperty = JS_StrictPropertyStub;
  interface_object_class->enumerate = JS_EnumerateStub;
  interface_object_class->resolve = JS_ResolveStub;
  interface_object_class->convert = JS_ConvertStub;
  return interface_data;
}

JSBool get_byteProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<int8_t >::ReturnType value =
      impl->byte_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_byteProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<int8_t >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_byte_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_octetProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<uint8_t >::ReturnType value =
      impl->octet_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_octetProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<uint8_t >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_octet_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_shortProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<int16_t >::ReturnType value =
      impl->short_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_shortProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<int16_t >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_short_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_unsignedShortProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<uint16_t >::ReturnType value =
      impl->unsigned_short_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_unsignedShortProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<uint16_t >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_unsigned_short_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_longProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<int32_t >::ReturnType value =
      impl->long_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_longProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<int32_t >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_long_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_unsignedLongProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<uint32_t >::ReturnType value =
      impl->unsigned_long_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_unsignedLongProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<uint32_t >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_unsigned_long_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_doubleProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<double >::ReturnType value =
      impl->double_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_doubleProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<double >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_double_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_unrestrictedDoubleProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  TypeTraits<double >::ReturnType value =
      impl->unrestricted_double_property();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_unrestrictedDoubleProperty(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<double >::ConversionType value;
  FromJSValue(context, vp, &exception_state, &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  NumericTypesTestInterface* impl =
      WrapperPrivate::GetWrappable<NumericTypesTestInterface>(object);
  impl->set_unrestricted_double_property(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}


const JSPropertySpec prototype_properties[] = {
  {  // Read/Write property
      "byteProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_byteProperty),
      JSOP_WRAPPER(&set_byteProperty),
  },
  {  // Read/Write property
      "octetProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_octetProperty),
      JSOP_WRAPPER(&set_octetProperty),
  },
  {  // Read/Write property
      "shortProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_shortProperty),
      JSOP_WRAPPER(&set_shortProperty),
  },
  {  // Read/Write property
      "unsignedShortProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_unsignedShortProperty),
      JSOP_WRAPPER(&set_unsignedShortProperty),
  },
  {  // Read/Write property
      "longProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_longProperty),
      JSOP_WRAPPER(&set_longProperty),
  },
  {  // Read/Write property
      "unsignedLongProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_unsignedLongProperty),
      JSOP_WRAPPER(&set_unsignedLongProperty),
  },
  {  // Read/Write property
      "doubleProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_doubleProperty),
      JSOP_WRAPPER(&set_doubleProperty),
  },
  {  // Read/Write property
      "unrestrictedDoubleProperty", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_unrestrictedDoubleProperty),
      JSOP_WRAPPER(&set_unrestrictedDoubleProperty),
  },
  JS_PS_END
};

const JSPropertySpec own_properties[] = {
  JS_PS_END
};

void InitializePrototypeAndInterfaceObject(
    InterfaceData* interface_data, JSContext* context) {
  DCHECK(!interface_data->prototype);
  DCHECK(!interface_data->interface_object);

  MozjsGlobalObjectProxy* global_object_proxy =
      static_cast<MozjsGlobalObjectProxy*>(JS_GetContextPrivate(context));
  JS::RootedObject global_object(context, global_object_proxy->global_object());
  DCHECK(global_object);
  JS::RootedObject parent_prototype(
      context, JS_GetObjectPrototype(context, global_object));
  DCHECK(parent_prototype);

  // Create the Prototype object.
  interface_data->prototype = JS_NewObjectWithGivenProto(
      context, &interface_data->prototype_class_definition, parent_prototype, NULL);
  bool success = JS_DefineProperties(
      context, interface_data->prototype, prototype_properties);
  DCHECK(success);


  JS::RootedObject function_prototype(
      context, JS_GetFunctionPrototype(context, global_object));
  DCHECK(function_prototype);
  // Create the Interface object.
  interface_data->interface_object = JS_NewObjectWithGivenProto(
      context, &interface_data->interface_object_class_definition,
      function_prototype, NULL);

  // Add the InterfaceObject.name property.
  JS::RootedObject rooted_interface_object(
      context, interface_data->interface_object);
  JS::RootedValue name_value(context);
  const char name[] = "NumericTypesTestInterface";
  name_value.setString(JS_NewStringCopyZ(context, "NumericTypesTestInterface"));
  success =
      JS_DefineProperty(context, rooted_interface_object, "name", name_value,
                        JS_PropertyStub, JS_StrictPropertyStub,
                        JSPROP_READONLY);
  DCHECK(success);

  // Set the Prototype.constructor and Constructor.prototype properties.
  DCHECK(interface_data->interface_object);
  DCHECK(interface_data->prototype);
  JS::RootedObject rooted_prototype(context, interface_data->prototype);
  success = JS_LinkConstructorAndPrototype(
      context,
      rooted_interface_object,
      rooted_prototype);
  DCHECK(success);
}

InterfaceData* GetInterfaceData(JSContext* context) {
  MozjsGlobalObjectProxy* global_object_proxy =
      static_cast<MozjsGlobalObjectProxy*>(JS_GetContextPrivate(context));
  // Use the address of the properties definition for this interface as a
  // unique key for looking up the InterfaceData for this interface.
  intptr_t key = reinterpret_cast<intptr_t>(&own_properties);
  InterfaceData* interface_data = global_object_proxy->GetInterfaceData(key);
  if (!interface_data) {
    interface_data = CreateCachedInterfaceData();
    DCHECK(interface_data);
    global_object_proxy->CacheInterfaceData(key, interface_data);
    DCHECK_EQ(interface_data, global_object_proxy->GetInterfaceData(key));
  }
  return interface_data;
}

}  // namespace

// static
JSObject* MozjsNumericTypesTestInterface::CreateInstance(
    JSContext* context, const scoped_refptr<Wrappable>& wrappable) {
  InterfaceData* interface_data = GetInterfaceData(context);
  JS::RootedObject prototype(context, GetPrototype(context));
  DCHECK(prototype);
  JS::RootedObject new_object(context, JS_NewObjectWithGivenProto(
      context, &interface_data->instance_class_definition, prototype, NULL));
  DCHECK(new_object);
  WrapperPrivate::AddPrivateData(new_object, wrappable);
  return new_object;
}

// static
JSObject* MozjsNumericTypesTestInterface::GetPrototype(JSContext* context) {
  InterfaceData* interface_data = GetInterfaceData(context);
  if (!interface_data->prototype) {
    // Create new prototype that has all the props and methods
    InitializePrototypeAndInterfaceObject(interface_data, context);
  }
  DCHECK(interface_data->prototype);
  return interface_data->prototype;
}

// static
JSObject* MozjsNumericTypesTestInterface::GetInterfaceObject(JSContext* context) {
  InterfaceData* interface_data = GetInterfaceData(context);
  if (!interface_data->interface_object) {
    InitializePrototypeAndInterfaceObject(interface_data, context);
  }
  DCHECK(interface_data->interface_object);
  return interface_data->interface_object;
}


namespace {
}  // namespace


}  // namespace testing
}  // namespace bindings
}  // namespace cobalt
