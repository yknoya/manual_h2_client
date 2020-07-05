/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, k-noya
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
