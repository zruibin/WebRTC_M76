// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/typed_arrays/dom_typed_array.h"

#include "third_party/blink/renderer/bindings/core/v8/v8_array_buffer.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_big_int_64_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_big_uint_64_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_float32_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_float64_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_int16_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_int32_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_int8_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_uint16_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_uint32_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_uint8_array.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_uint8_clamped_array.h"
#include "third_party/blink/renderer/platform/bindings/dom_data_store.h"

namespace blink {

template <typename WTFTypedArray, typename V8TypedArray>
v8::Local<v8::Object> DOMTypedArray<WTFTypedArray, V8TypedArray>::Wrap(
    v8::Isolate* isolate,
    v8::Local<v8::Object> creation_context) {
  DCHECK(!DOMDataStore::ContainsWrapper(this, isolate));

  const WrapperTypeInfo* wrapper_type_info = this->GetWrapperTypeInfo();
  DOMArrayBufferBase* buffer = this->BufferBase();
  v8::Local<v8::Value> v8_buffer = ToV8(buffer, creation_context, isolate);
  if (v8_buffer.IsEmpty())
    return v8::Local<v8::Object>();
  DCHECK_EQ(IsShared(), v8_buffer->IsSharedArrayBuffer());

  v8::Local<v8::Object> wrapper;
  if (IsShared()) {
    wrapper = V8TypedArray::New(v8_buffer.As<v8::SharedArrayBuffer>(),
                                byteOffset(), length());
  } else {
    wrapper = V8TypedArray::New(v8_buffer.As<v8::ArrayBuffer>(), byteOffset(),
                                length());
  }

  return AssociateWithWrapper(isolate, wrapper_type_info, wrapper);
}

// TODO(tasak): The following traits should be auto-generated by binding
// script and should be placed in bindings/core/v8/V8*Array.h.
template <typename ArrayType>
struct DOMTypedArrayTraits {};

#define DEFINE_DOMTYPEDARRAY_TRAITS(ArrayType, V8BindingType) \
  template <>                                                 \
  struct DOMTypedArrayTraits<ArrayType> {                     \
    typedef V8BindingType Type;                               \
  }

DEFINE_DOMTYPEDARRAY_TRAITS(DOMInt8Array, V8Int8Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMInt16Array, V8Int16Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMInt32Array, V8Int32Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMUint8Array, V8Uint8Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMUint8ClampedArray, V8Uint8ClampedArray);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMUint16Array, V8Uint16Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMUint32Array, V8Uint32Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMBigInt64Array, V8BigInt64Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMBigUint64Array, V8BigUint64Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMFloat32Array, V8Float32Array);
DEFINE_DOMTYPEDARRAY_TRAITS(DOMFloat64Array, V8Float64Array);

template <typename WTFTypedArray, typename V8TypedArray>
const WrapperTypeInfo*
DOMTypedArray<WTFTypedArray, V8TypedArray>::GetWrapperTypeInfo() const {
  return DOMTypedArrayTraits<
      DOMTypedArray<WTFTypedArray, V8TypedArray>>::Type::GetWrapperTypeInfo();
}

template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Int8Array, v8::Int8Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Int16Array, v8::Int16Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Int32Array, v8::Int32Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Uint8Array, v8::Uint8Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Uint8ClampedArray, v8::Uint8ClampedArray>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Uint16Array, v8::Uint16Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Uint32Array, v8::Uint32Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::BigInt64Array, v8::BigInt64Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::BigUint64Array, v8::BigUint64Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Float32Array, v8::Float32Array>;
template class CORE_TEMPLATE_EXPORT
    DOMTypedArray<WTF::Float64Array, v8::Float64Array>;

}  // namespace blink