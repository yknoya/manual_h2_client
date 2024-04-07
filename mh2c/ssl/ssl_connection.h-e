// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_SSL_SSL_CONNECTION_H_
#define MH2C_SSL_SSL_CONNECTION_H_

#include <cstdint>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/ssl/ssl_bio.h"
#include "mh2c/ssl/ssl_ctx.h"
#include "mh2c/ssl/ssl_verify_mode.h"

namespace mh2c {

namespace ssl {

class ssl_connection {
 public:
  ssl_connection(const std::string& hostname, const uint16_t port,
                 const verify_mode mode);

  ssl_connection(const ssl_connection&) = delete;
  ssl_connection& operator=(const ssl_connection&) = delete;
  ssl_connection(ssl_connection&&) = delete;
  ssl_connection&& operator=(ssl_connection&&) = delete;

  void write(const uint8_t* data, const size_t length);
  void read(uint8_t* data, const size_t length);

 private:
  ssl_ctx m_ssl_ctx;
  ssl_bio m_ssl_bio;
};

}  // namespace ssl

}  // namespace mh2c

#endif  // MH2C_SSL_SSL_CONNECTION_H_
