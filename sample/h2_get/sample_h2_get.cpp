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
#include <cstdint>
#include <iostream>
#include <string>

#include "mh2c/mh2c.h"

bool is_more_data(const mh2c::frame_header& fh) {
  auto result = true;
  switch (fh.m_type) {
    case static_cast<mh2c::fh_type_t>(mh2c::frame_type_registry::DATA):
      if (fh.m_flags & static_cast<mh2c::fh_flags_t>(mh2c::df_flag::END_STREAM)) {
        result = false;
      }
      break;
    case static_cast<mh2c::fh_type_t>(mh2c::frame_type_registry::GOAWAY):
      result = false;
      break;
    default:
      break;
  }

  return result;
}

mh2c::fh_type_t receive_frames(mh2c::http2_client* h2_client) {
  mh2c::fh_type_t last_frame_type{};
  try {
    while (auto frame = h2_client->receive_frame()) {
      std::cout << frame;

      const auto& frame_header = frame->get_header();
      last_frame_type = frame_header.m_type;
      if (is_more_data(frame_header) == false) {
        break;
      }
    }
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
    throw;
  }

  return last_frame_type;
}

int main() {
  std::string host{"nghttp2.org"};
  uint16_t port{443};
  mh2c::http2_client h2_client{host, port,
                               mh2c::ssl::verify_mode::VERIFY_SERVER_CERT};

  // Connection preface
  h2_client.send_connection_preface();

  // Send settings frame
  mh2c::fh_flags_t flags{0u};
  mh2c::fh_stream_id_t stream_id{0u};
  const size_t initial_table_size{8192u};
  const mh2c::sf_payload_t sf_payload{
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_ENABLE_PUSH, 0u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE,
                              65535u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE,
                              initial_table_size)};
  mh2c::settings_frame sf{flags, stream_id, sf_payload};
  h2_client.send_frame(sf);
  h2_client.update_request_dynamic_table(initial_table_size);

  // Receive settings frame
  const auto frame = h2_client.receive_frame();
  std::cout << frame;

  // Send settings frame (ack)
  flags = static_cast<mh2c::fh_flags_t>(mh2c::sf_flag::ACK);
  stream_id = 0u;
  const mh2c::settings_frame sf_ack{flags, stream_id, {}};
  h2_client.send_frame(sf_ack);

  // Send headers frame
  flags = static_cast<mh2c::fh_flags_t>(mh2c::hf_flag::END_STREAM) |
          static_cast<mh2c::fh_flags_t>(mh2c::hf_flag::END_HEADERS);
  stream_id = 1u;
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/httpbin/headers"},
                                  {":scheme", "https"},
                                  {":authority", "nghttp2.org"},
                              })};
  const auto mode = mh2c::header_encode_mode::HUFFMAN;
  mh2c::headers_frame hf{flags, stream_id, header_block, mode,
                         h2_client.get_request_dynamic_table()};
  h2_client.send_frame(hf);
  h2_client.update_request_dynamic_table(hf.get_payload());

  // Receive frames
  mh2c::fh_type_t last_frame_type = receive_frames(&h2_client);

  // Send GOAWAY frame
  if (last_frame_type !=
      static_cast<mh2c::fh_type_t>(mh2c::frame_type_registry::GOAWAY)) {
    const mh2c::goaway_payload gf_payload{
        0x0, stream_id, mh2c::error_codes::NO_ERROR, {}};
    const mh2c::goaway_frame gf{gf_payload};
    h2_client.send_frame(gf);
  }

  return 0;
}
