// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include <cstdint>
#include <iostream>
#include <string>

#include "mh2c/mh2c.h"

bool is_more_data(const mh2c::frame_header& fh) {
  auto result = true;
  switch (mh2c::cast_to_frame_type_registry(fh.m_type)) {
    case mh2c::frame_type_registry::DATA:
      if (mh2c::is_flag_set(fh.m_flags, mh2c::df_flag::END_STREAM)) {
        result = false;
      }
      break;
    case mh2c::frame_type_registry::GOAWAY:
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
  const auto sf_payload{mh2c::make_sf_payload(
      {{mh2c::sf_parameter::SETTINGS_ENABLE_PUSH, 0u},
       {mh2c::sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE, 65535u},
       {mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE, initial_table_size}})};
  mh2c::settings_frame sf{flags, stream_id, sf_payload};
  h2_client.send_frame(sf);
  h2_client.update_request_dynamic_table(initial_table_size);

  // Receive settings frame
  const auto frame = h2_client.receive_frame();
  std::cout << frame;

  // Send settings frame (ack)
  flags = make_frame_header_flags(mh2c::sf_flag::ACK);
  stream_id = 0u;
  const mh2c::settings_frame sf_ack{flags, stream_id, {}};
  h2_client.send_frame(sf_ack);

  // Send headers frame
  flags = make_frame_header_flags(mh2c::hf_flag::END_STREAM,
                                  mh2c::hf_flag::END_HEADERS);
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
      mh2c::underlying_cast(mh2c::frame_type_registry::GOAWAY)) {
    const mh2c::goaway_payload gf_payload{
        0x0, stream_id, mh2c::error_codes::NO_ERROR, {}};
    const mh2c::goaway_frame gf{gf_payload};
    h2_client.send_frame(gf);
  }

  return 0;
}
