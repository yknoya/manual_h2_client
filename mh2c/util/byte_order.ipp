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
#ifndef MH2C_UTIL_BYTE_ORDER_IPP_
#define MH2C_UTIL_BYTE_ORDER_IPP_

#include <cstdint>

#include "bit_operation.h"

namespace mh2c {

namespace {

constexpr uint8_t BITS_IN_BYTE{8u};

}  // namespace

template <typename IntegralType, typename RandomAccessIterator, typename>
IntegralType bytes2integral(const RandomAccessIterator begin) {
  IntegralType result{};
  constexpr size_t size{sizeof(IntegralType)};
  const auto end = begin + size;
  auto shift_bit = (size - 1) * BITS_IN_BYTE;

  for (auto ite = begin; ite != end; ++ite) {
    result += *ite << shift_bit;
    shift_bit -= BITS_IN_BYTE;
  }

  return result;
}

template <typename BytesTytpe, typename IntegralType, typename>
BytesTytpe integral2bytes(const IntegralType integer) {
  BytesTytpe result{};
  constexpr size_t size{sizeof(IntegralType)};

  for (int shift_bit = (size - 1) * BITS_IN_BYTE; shift_bit >= 0;
       shift_bit -= BITS_IN_BYTE) {
    const typename BytesTytpe::value_type val{static_cast<decltype(val)>(
        extract_low_bit<BITS_IN_BYTE>(integer >> shift_bit))};
    result.push_back(val);
  }

  return result;
}

}  // namespace mh2c

#endif  // MH2C_UTIL_BYTE_ORDER_IPP_
