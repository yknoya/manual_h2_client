// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_FRAME_TYPE_REGISTRY_H_
#define MH2C_FRAME_FRAME_TYPE_REGISTRY_H_

#include <cstdint>

#include "mh2c/frame/frame_header.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-11.2
enum class frame_type_registry : fh_type_t {
  DATA,
  HEADERS,
  PRIORITY,
  RST_STREAM,
  SETTINGS,
  PUSH_PROMISE,
  PING,
  GOAWAY,
  WINDOW_UPDATE,
  CONTINUATION
};

template <typename T>
inline frame_type_registry cast_to_frame_type_registry(T frame_type);

}  // namespace mh2c

#include "mh2c/frame/frame_type_registry.ipp"

#endif  // MH2C_FRAME_FRAME_TYPE_REGISTRY_H_
