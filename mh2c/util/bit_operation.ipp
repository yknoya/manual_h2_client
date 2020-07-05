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
#ifndef MH2C_UTIL_BIT_OPERATION_IPP_
#define MH2C_UTIL_BIT_OPERATION_IPP_

#include <cstddef>

namespace mh2c {

template <size_t N, typename T, typename, typename>
T extract_low_bit(const T bits) {
  return bits & ((1u << N) - 1u);
}

template <size_t N, typename T, typename, typename>
T extract_high_bit(const T bits) {
  return bits >> (sizeof(T) * 8u - N);
}

template <typename T, typename U, typename, typename>
bool is_flag_set(const T flags, const U flag) {
  return flag == 0u ? flags == static_cast<T>(flag)
                    : (flags & static_cast<T>(flag)) == static_cast<T>(flag);
}

template <typename T, typename U, typename, typename, typename>
bool is_flag_set(const T flags, const U flag) {
  return is_flag_set(flags, static_cast<std::underlying_type_t<U>>(flag));
}

}  // namespace mh2c

#endif  // MH2C_UTIL_BIT_OPERATION_H_
