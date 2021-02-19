// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#include "mh2c/http2_client.h"

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <queue>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/continuation_frame.h"
#include "mh2c/frame/frame_builder.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/frame/headers_frame.h"
#include "mh2c/frame/push_promise_frame.h"
#include "mh2c/frame/settings_frame.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/ssl/ssl_connection.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

namespace {

void update_dynamic_table(const header_block_t& header_block,
                          dynamic_table* dynamic_table) {
  std::for_each(
      header_block.begin(), header_block.end(),
      [dynamic_table](const auto& header_entry) {
        if (header_entry.get_prefix() ==
            header_prefix_pattern::INCREMENTAL_INDEXING) {
          dynamic_table->push(header_entry.get_header());
        } else if (header_entry.get_prefix() ==
                   header_prefix_pattern::SIZE_UPDATE) {
          dynamic_table->update_table_size(header_entry.get_max_size());
        }
      });

  return;
}

void update_dynamic_table(const h2_frame_ptr& frame_ptr,
                          dynamic_table* dynamic_table) {
  header_block_t header_block{};

  switch (static_cast<frame_type_registry>(frame_ptr->get_header().m_type)) {
    case frame_type_registry::HEADERS:
      header_block =
          dynamic_cast<const headers_frame*>(frame_ptr.get())->get_payload();
      break;
    case frame_type_registry::PUSH_PROMISE:
      header_block = dynamic_cast<const push_promise_frame*>(frame_ptr.get())
                         ->get_payload()
                         .m_header_block;
      break;
    case frame_type_registry::CONTINUATION:
      header_block = dynamic_cast<const continuation_frame*>(frame_ptr.get())
                         ->get_payload();
      break;
    case frame_type_registry::SETTINGS: {
      const auto sf_payload =
          dynamic_cast<const settings_frame*>(frame_ptr.get())->get_payload();
      const auto table_size_key =
          static_cast<std::underlying_type_t<sf_parameter>>(
              sf_parameter::SETTINGS_HEADER_TABLE_SIZE);
      if (sf_payload.find(table_size_key) != sf_payload.end()) {
        const auto table_size = sf_payload.at(table_size_key);
        dynamic_table->update_table_size(table_size);
      }
      return;
    }
    default:
      return;
  }

  update_dynamic_table(header_block, dynamic_table);
  return;
}

}  // namespace

class http2_client::impl {
 public:
  impl(const std::string& hostname, const uint16_t port,
       const ssl::verify_mode mode);

  void send_raw_data(const uint8_t* data, const size_t length);
  void receive_raw_data(uint8_t* data, const size_t length);

  void send_connection_preface();
  h2_frame_ptr receive_frame();

  void update_request_dynamic_table(const header_block_t& header_block);
  void update_request_dynamic_table(const size_t max_size);
  const dynamic_table& get_request_dynamic_table();

 private:
  ssl::ssl_connection m_ssl_connection;
  dynamic_table m_request_dynamic_table;
  dynamic_table m_response_dynamic_table;
};

http2_client::impl::impl(const std::string& hostname, uint16_t port,
                         const ssl::verify_mode mode)
    : m_ssl_connection{hostname, port, mode} {}

void http2_client::impl::send_raw_data(const uint8_t* data,
                                       const size_t length) {
  m_ssl_connection.write(data, length);
  return;
}

void http2_client::impl::receive_raw_data(uint8_t* data, const size_t length) {
  m_ssl_connection.read(data, length);
  return;
}

void http2_client::impl::send_connection_preface() {
  static const std::string client_connection_preface{
      "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"};
  send_raw_data(
      reinterpret_cast<const uint8_t*>(client_connection_preface.c_str()),
      client_connection_preface.length());
  return;
}

h2_frame_ptr http2_client::impl::receive_frame() {
  byte_array_t raw_fh(FRAME_HEADER_BYTES);
  receive_raw_data(&raw_fh[0], raw_fh.size());
  const auto fh = build_frame_header(raw_fh);

  byte_array_t raw_payload(fh.m_length);
  if (fh.m_length > 0) {
    receive_raw_data(&raw_payload[0], raw_payload.size());
  }

  auto frame_ptr = build_frame(fh, raw_payload, m_response_dynamic_table);
  update_dynamic_table(frame_ptr, &m_response_dynamic_table);

  return frame_ptr;
}

void http2_client::impl::update_request_dynamic_table(
    const header_block_t& header_block) {
  update_dynamic_table(header_block, &m_request_dynamic_table);
  return;
}

void http2_client::impl::update_request_dynamic_table(const size_t max_size) {
  m_request_dynamic_table.update_table_size(max_size);
  return;
}

const dynamic_table& http2_client::impl::get_request_dynamic_table() {
  return m_request_dynamic_table;
}

http2_client::http2_client(const std::string& hostname, const uint16_t port,
                           const ssl::verify_mode mode)
    : m_pimpl(std::make_unique<http2_client::impl>(hostname, port, mode)) {}

http2_client::~http2_client() = default;

void http2_client::send_raw_data(const uint8_t* data, const size_t length) {
  m_pimpl->send_raw_data(data, length);
  return;
}

void http2_client::receive_raw_data(uint8_t* data, const size_t length) {
  m_pimpl->receive_raw_data(data, length);
  return;
}

void http2_client::send_connection_preface() {
  m_pimpl->send_connection_preface();
  return;
}

h2_frame_ptr http2_client::receive_frame() { return m_pimpl->receive_frame(); }

void http2_client::update_request_dynamic_table(
    const header_block_t& header_block) {
  m_pimpl->update_request_dynamic_table(header_block);
  return;
}

void http2_client::update_request_dynamic_table(const size_t max_size) {
  m_pimpl->update_request_dynamic_table(max_size);
  return;
}

const dynamic_table& http2_client::get_request_dynamic_table() {
  return m_pimpl->get_request_dynamic_table();
}

std::ostream& operator<<(std::ostream& out_stream, const h2_frame_ptr& frame) {
  frame->dump(out_stream);
  return out_stream;
}

}  // namespace mh2c
