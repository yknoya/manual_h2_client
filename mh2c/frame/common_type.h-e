// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_COMMON_TYPE_H_
#define MH2C_FRAME_COMMON_TYPE_H_

#include <cstdint>
#include <memory>

namespace mh2c {

using reserved_t = uint8_t;

template <typename Header>
class i_frame;
struct frame_header;
using h2_frame_ptr = std::unique_ptr<i_frame<frame_header>>;

}  // namespace mh2c

#endif  // MH2C_FRAME_COMMON_TYPE_H_
