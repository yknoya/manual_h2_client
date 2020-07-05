/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, k-noya
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
