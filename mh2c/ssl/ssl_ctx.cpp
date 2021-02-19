// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#include "mh2c/ssl/ssl_ctx.h"

#include <openssl/ssl.h>

#include <stdexcept>
#include <string>

namespace mh2c {

namespace ssl {

ssl_ctx::ssl_ctx() : m_ssl_ctx(SSL_CTX_new(TLS_client_method())) {
  if (m_ssl_ctx == nullptr) {
    throw std::runtime_error("SSL_CTX_new failed");
  }
}

ssl_ctx::~ssl_ctx() { SSL_CTX_free(m_ssl_ctx); }

ssl_ctx::operator SSL_CTX*() const { return m_ssl_ctx; }

}  // namespace ssl

}  // namespace mh2c
