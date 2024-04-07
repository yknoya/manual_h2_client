// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_UTIL_BYTE_ORDER_H_
#define MH2C_UTIL_BYTE_ORDER_H_

#include <cstdint>
#include <type_traits>

namespace mh2c {

uint16_t cvt_net2host(uint16_t data);
uint32_t cvt_net2host(uint32_t data);

uint16_t cvt_host2net(uint16_t data);
uint32_t cvt_host2net(uint32_t data);

template <typename IntegralType, typename RandomAccessIterator,
          typename = std::enable_if_t<std::is_integral_v<IntegralType>>>
IntegralType bytes2integral(const RandomAccessIterator ite);

template <typename BytesTytpe, typename IntegralType,
          typename = std::enable_if_t<std::is_integral_v<IntegralType>>>
BytesTytpe integral2bytes(const IntegralType integer);

}  // namespace mh2c

#include "mh2c/util/byte_order.ipp"

#endif  // MH2C_UTIL_BYTE_ORDER_H_
