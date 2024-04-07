// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#include "mh2c/ssl/ssl_bio.h"

#include <openssl/ssl.h>

#include <stdexcept>
#include <string>

#include "mh2c/ssl/ssl_ctx.h"

namespace mh2c {

namespace ssl {

ssl_bio::ssl_bio(const ssl_ctx& ssl_ctx)
    : m_ssl_bio(BIO_new_ssl_connect(ssl_ctx)) {
  if (m_ssl_bio == nullptr) {
    throw std::runtime_error("BIO_new_ssl_connect failed");
  }
}

ssl_bio::~ssl_bio() { BIO_free_all(m_ssl_bio); }

ssl_bio::operator BIO*() const { return m_ssl_bio; }

}  // namespace ssl

}  // namespace mh2c
