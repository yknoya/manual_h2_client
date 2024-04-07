// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_UTIL_CAST_H_
#define MH2C_UTIL_CAST_H_

#include <type_traits>

namespace mh2c {

template <typename T>
std::underlying_type_t<T> underlying_cast(T t);

}  // namespace mh2c

#include "mh2c/util/cast.ipp"

#endif  // MH2C_UTIL_CAST_H_
