// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_SSL_SSL_BIO_H_
#define MH2C_SSL_SSL_BIO_H_

#include <openssl/ssl.h>

#include "mh2c/ssl/ssl_ctx.h"

namespace mh2c {

namespace ssl {

class ssl_bio {
 public:
  explicit ssl_bio(const ssl_ctx& ssl_ctx);
  ~ssl_bio();

  ssl_bio(const ssl_bio&) = delete;
  ssl_bio& operator=(const ssl_bio&) = delete;
  ssl_bio(ssl_bio&&) = delete;
  ssl_bio&& operator=(ssl_bio&&) = delete;

  operator BIO*() const;

 private:
  BIO* m_ssl_bio;
};

}  // namespace ssl

}  // namespace mh2c

#endif  // MH2C_SSL_SSL_BIO_H_
