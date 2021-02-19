// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#include "mh2c/ssl/ssl_connection.h"

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/ssl/ssl_bio.h"
#include "mh2c/ssl/ssl_ctx.h"
#include "mh2c/ssl/ssl_verify_mode.h"

namespace mh2c {

namespace ssl {

namespace {
std::once_flag load_once;

void load_ssl_lib() {
  SSL_library_init();
  SSL_load_error_strings();
}

}  // namespace

ssl_connection::ssl_connection(const std::string& hostname, const uint16_t port,
                               const verify_mode mode)
    : m_ssl_ctx(ssl::ssl_ctx{}), m_ssl_bio(m_ssl_ctx) {
  // Setup
  std::call_once(load_once, load_ssl_lib);

  const auto host_port = hostname + ":" + std::to_string(port);
  BIO_set_conn_hostname(m_ssl_bio, host_port.c_str());

  SSL* ssl{};
  BIO_get_ssl(m_ssl_bio, &ssl);
  if (ssl == nullptr) {
    throw std::runtime_error("BIO_get_ssl failed");
  }
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  static const byte_array_t alpn_protos{0x02, 'h', '2'};
  auto err = SSL_set_alpn_protos(ssl, alpn_protos.data(), alpn_protos.size());
  if (err != 0) {
    throw std::runtime_error("SSL_set_alpn_protos failed: err=" +
                             std::to_string(err));
  }

  err = SSL_CTX_set_default_verify_paths(m_ssl_ctx);
  if (err != 1) {
    throw std::runtime_error("SSL_CTX_set_default_verify_paths failed: err=" +
                             std::to_string(err));
  }

  // Connect
  err = BIO_do_connect(m_ssl_bio);
  if (err <= 0) {
    int err_code = errno;
    throw std::runtime_error("BIO_do_connect failed: err_code=" +
                             std::to_string(err_code));
  }

  err = BIO_do_handshake(m_ssl_bio);
  if (err <= 0) {
    int err_code = errno;
    throw std::runtime_error("BIO_do_handshake failed: err_code=" +
                             std::to_string(err_code));
  }

  // Check verification result
  if (mode != verify_mode::VERIFY_NONE) {
    const auto verify_result = SSL_get_verify_result(ssl);
    if (verify_result != X509_V_OK) {
      throw std::runtime_error("SSL_get_verify_result failed: verify_result=" +
                               std::to_string(verify_result));
    }
  }

  // Check ALPN result
  const unsigned char* alpn_result;
  unsigned int alpn_result_len;
  SSL_get0_alpn_selected(ssl, &alpn_result, &alpn_result_len);

  std::string alpn_str{reinterpret_cast<const char*>(alpn_result),
                       alpn_result_len};
  const bool is_expected_result =
      (alpn_result_len == alpn_protos.size() - 1) &&
      std::equal(alpn_str.begin(), alpn_str.end(), &alpn_protos[1]);
  if (is_expected_result == false) {
    throw std::runtime_error("unexpected alpn result: alpn_result=" + alpn_str);
  }

  return;
}

void ssl_connection::write(const uint8_t* data, const size_t length) {
  const auto result = BIO_write(m_ssl_bio, data, length);
  if (result <= 0 || static_cast<size_t>(result) != length) {
    throw std::runtime_error(
        "BIO_write failed: result=" + std::to_string(result) +
        ", length=" + std::to_string(length));
  }
  return;
}

void ssl_connection::read(uint8_t* data, const size_t length) {
  const auto result = BIO_read(m_ssl_bio, data, length);
  if (result <= 0 || static_cast<size_t>(result) != length) {
    throw std::runtime_error(
        "BIO_read failed: result=" + std::to_string(result) +
        ", length=" + std::to_string(length));
  }
  return;
}

}  // namespace ssl

}  // namespace mh2c
