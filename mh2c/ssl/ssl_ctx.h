// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_SSL_SSL_CTX_H_
#define MH2C_SSL_SSL_CTX_H_

#include <openssl/ssl.h>

namespace mh2c {

namespace ssl {

class ssl_ctx {
 public:
  ssl_ctx();
  ~ssl_ctx();

  ssl_ctx(const ssl_ctx&) = delete;
  ssl_ctx& operator=(const ssl_ctx&) = delete;
  ssl_ctx(ssl_ctx&&) = delete;
  ssl_ctx&& operator=(ssl_ctx&&) = delete;

  operator SSL_CTX*() const;

 private:
  SSL_CTX* m_ssl_ctx;
};

}  // namespace ssl

}  // namespace mh2c

#endif  // MH2C_SSL_SSL_CTX_H_
