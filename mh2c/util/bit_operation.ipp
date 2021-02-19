// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
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
