// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_SSL_SSL_VERIFY_MODE_H_
#define MH2C_SSL_SSL_VERIFY_MODE_H_

#include <cstdint>

namespace mh2c {

namespace ssl {

enum class verify_mode : uint8_t {
  VERIFY_NONE,
  VERIFY_SERVER_CERT,
};

}  // namespace ssl

}  // namespace mh2c

#endif  // MH2C_SSL_SSL_VERIFY_MODE_H_
