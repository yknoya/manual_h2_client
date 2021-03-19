// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_FRAME_TYPE_REGISTRY_IPP_
#define MH2C_FRAME_FRAME_TYPE_REGISTRY_IPP_

namespace mh2c {

template <typename T>
inline frame_type_registry cast_to_frame_type_registry(T frame_type) {
  return static_cast<frame_type_registry>(frame_type);
}

}  // namespace mh2c

#endif  // MH2C_FRAME_FRAME_TYPE_REGISTRY_IPP_
