// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_INTEGER_REPRESENTATION_H_
#define MH2C_HPACK_INTEGER_REPRESENTATION_H_

#include <cstdint>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

template <header_index_t N>
byte_array_t encode_integer_value(size_t value);

template <header_index_t N>
uint32_t decode_integer_value(const byte_array_t& encoded_value);

}  // namespace mh2c

#include "mh2c/hpack/integer_representation.ipp"

#endif  // MH2C_HPACK_INTEGER_REPRESENTATION_H_
