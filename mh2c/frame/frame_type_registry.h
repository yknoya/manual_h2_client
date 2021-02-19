// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_FRAME_TYPE_REGISTRY_H_
#define MH2C_FRAME_FRAME_TYPE_REGISTRY_H_

#include <cstdint>

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-11.2
enum class frame_type_registry : uint8_t {
  DATA = 0x00,
  HEADERS = 0x01,
  PRIORITY = 0x02,
  RST_STREAM = 0x03,
  SETTINGS = 0x04,
  PUSH_PROMISE = 0x05,
  PING = 0x06,
  GOAWAY = 0x07,
  WINDOW_UPDATE = 0x08,
  CONTINUATION = 0x09
};

}  // namespace mh2c

#endif  // MH2C_FRAME_FRAME_TYPE_REGISTRY_H_
