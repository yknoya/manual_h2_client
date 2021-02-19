// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
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
