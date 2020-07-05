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
#ifndef MH2C_UTIL_BIT_OPERATION_H_
#define MH2C_UTIL_BIT_OPERATION_H_

#include <cstddef>
#include <type_traits>

namespace mh2c {

template <size_t N, typename T,
          typename = std::enable_if_t<std::is_integral_v<T>>,
          typename = std::enable_if_t<(N <= sizeof(T) * 8u)>>
T extract_low_bit(const T bits);

template <size_t N, typename T,
          typename = std::enable_if_t<std::is_integral_v<T>>,
          typename = std::enable_if_t<(N <= sizeof(T) * 8u)>>
T extract_high_bit(const T bits);

template <typename T, typename U,
          typename = std::enable_if_t<std::is_integral_v<T>>,
          typename = std::enable_if_t<std::is_convertible_v<U, T>>>
bool is_flag_set(const T flags, const U flag);

template <
    typename T, typename U, typename = std::enable_if_t<std::is_integral_v<T>>,
    typename = std::enable_if_t<std::is_enum_v<U>>,
    typename =
        std::enable_if_t<std::is_convertible_v<std::underlying_type_t<U>, T>>>
bool is_flag_set(const T flags, const U flag);

}  // namespace mh2c

#include "mh2c/util/bit_operation.ipp"

#endif  // MH2C_UTIL_BIT_OPERATION_H_
