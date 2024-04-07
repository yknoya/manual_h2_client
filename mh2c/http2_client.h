// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
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
