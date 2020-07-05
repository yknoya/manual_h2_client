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
