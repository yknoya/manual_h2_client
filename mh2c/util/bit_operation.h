// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
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
