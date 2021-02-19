// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_INTEGER_REPRESENTATION_IPP_
#define MH2C_HPACK_INTEGER_REPRESENTATION_IPP_

#include <cstdint>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7541#section-5.1
template <header_index_t N>
byte_array_t encode_integer_value(size_t value) {
  byte_array_t encoded_value{};
  constexpr byte_array_t::value_type max_value = (1u << N) - 1;
  constexpr decltype(max_value) max_subsequent_value = 128;

  if (value < max_value) {
    encoded_value.push_back(value);
    return encoded_value;
  }

  encoded_value.push_back(max_value);
  value -= max_value;
  while (value >= max_subsequent_value) {
    encoded_value.push_back(value % max_subsequent_value + max_subsequent_value);
    value /= max_subsequent_value;
  }
  encoded_value.push_back(value);

  return encoded_value;
}

// cf. https://tools.ietf.org/html/rfc7541#section-5.1
template <header_index_t N>
uint32_t decode_integer_value(const byte_array_t& encoded_value) {
  constexpr byte_array_t::value_type max_value{(1u << N) - 1};
  decltype(max_value) max_subsequent_value{128};

  auto iter = encoded_value.begin();
  uint32_t decoded_value = *iter & max_value;
  if (decoded_value < max_value) {
    return decoded_value;
  }

  decltype(decoded_value) m{};
  byte_array_t::value_type next_byte{};
  do {
    next_byte = *(++iter);
    decoded_value += ((next_byte & (max_subsequent_value - 1)) * (1 << m));
    m += 7;
  } while ((next_byte & max_subsequent_value) == max_subsequent_value);

  return decoded_value;
}

}  // namespace mh2c

#endif  // MH2C_HPACK_INTEGER_REPRESENTATION_IPP_
