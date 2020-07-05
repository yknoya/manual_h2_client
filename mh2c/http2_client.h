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
#ifndef MH2C_HTTP2_CLIENT_H_
#define MH2C_HTTP2_CLIENT_H_

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/ssl/ssl_verify_mode.h"

namespace mh2c {

class http2_client {
 public:
  http2_client(const std::string& hostname, const uint16_t port,
               const ssl::verify_mode mode);
  ~http2_client();

  void send_raw_data(const uint8_t* data, const size_t length);
  void receive_raw_data(uint8_t* data, const size_t length);

  void send_connection_preface();
  template <typename Frame>
  void send_frame(const Frame& frame);
  h2_frame_ptr receive_frame();

  void update_request_dynamic_table(const header_block_t& header_block);
  void update_request_dynamic_table(const size_t max_size);
  const dynamic_table& get_request_dynamic_table();

 private:
  class impl;
  std::unique_ptr<impl> m_pimpl;
};

std::ostream& operator<<(std::ostream& out_stream, const h2_frame_ptr& frame);

}  // namespace mh2c

#include "mh2c/http2_client.ipp"

#endif  // MH2C_HTTP2_CLIENT_H_
