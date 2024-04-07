// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/frame_builder.h"

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/continuation_frame.h"
#include "mh2c/frame/data_frame.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/frame/goaway_frame.h"
#include "mh2c/frame/headers_frame.h"
#include "mh2c/frame/i_frame.h"
#include "mh2c/frame/ping_frame.h"
#include "mh2c/frame/priority_frame.h"
#include "mh2c/frame/push_promise_frame.h"
#include "mh2c/frame/raw_frame.h"
#include "mh2c/frame/rst_stream_frame.h"
#include "mh2c/frame/settings_frame.h"
#include "mh2c/frame/window_update_frame.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

namespace {

h2_frame_ptr build_raw_frame(const frame_header& fh,
                             const byte_array_t& raw_payload,
                             const dynamic_table&) {
  return std::make_unique<raw_frame>(fh, raw_payload);
}

h2_frame_ptr build_continuation_frame(const frame_header& fh,
                                      const byte_array_t& raw_payload,
                                      const dynamic_table& dynamic_table) {
  return std::make_unique<continuation_frame>(fh, raw_payload, dynamic_table);
}

h2_frame_ptr build_data_frame(const frame_header& fh,
                              const byte_array_t& raw_payload,
                              const dynamic_table&) {
  return std::make_unique<data_frame>(fh.m_flags, fh.m_stream_id, raw_payload);
}

h2_frame_ptr build_headers_frame(const frame_header& fh,
                                 const byte_array_t& raw_payload,
                                 const dynamic_table& dynamic_table) {
  try {
    return std::make_unique<headers_frame>(fh, raw_payload, dynamic_table);
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
  }

  return build_raw_frame(fh, raw_payload, dynamic_table);
}

h2_frame_ptr build_goaway_frame(const frame_header& fh,
                                const byte_array_t& raw_payload,
                                const dynamic_table&) {
  return std::make_unique<goaway_frame>(fh, raw_payload);
}

h2_frame_ptr build_ping_frame(const frame_header& fh,
                              const byte_array_t& raw_payload,
                              const dynamic_table&) {
  return std::make_unique<ping_frame>(fh, raw_payload);
}

h2_frame_ptr build_priority_frame(const frame_header& fh,
                                  const byte_array_t& raw_payload,
                                  const dynamic_table&) {
  return std::make_unique<priority_frame>(fh, raw_payload);
}

h2_frame_ptr build_push_promise_frame(const frame_header& fh,
                                      const byte_array_t& raw_payload,
                                      const dynamic_table& dynamic_table) {
  return std::make_unique<push_promise_frame>(fh, raw_payload, dynamic_table);
}

h2_frame_ptr build_rst_stream_frame(const frame_header& fh,
                                    const byte_array_t& raw_payload,
                                    const dynamic_table&) {
  return std::make_unique<rst_stream_frame>(fh, raw_payload);
}

h2_frame_ptr build_settings_frame(const frame_header& fh,
                                  const byte_array_t& raw_payload,
                                  const dynamic_table&) {
  sf_payload_t payload;
  for (auto ite = raw_payload.begin(); ite != raw_payload.end();) {
    auto id = bytes2integral<sf_id_t>(ite);
    ite += sizeof(id);
    auto value = bytes2integral<sf_value_t>(ite);
    ite += sizeof(value);
    payload.insert(make_sf_parameter(id, value));
  }

  return std::make_unique<settings_frame>(fh.m_flags, fh.m_stream_id, payload);
}

h2_frame_ptr build_window_update_frame(const frame_header& fh,
                                       const byte_array_t& raw_payload,
                                       const dynamic_table&) {
  return std::make_unique<window_update_frame>(fh, raw_payload);
}

using builder_func_t = h2_frame_ptr (*)(const frame_header&,
                                        const byte_array_t&,
                                        const dynamic_table&);

const std::unordered_map<frame_type_registry, builder_func_t> builder_func_map{
    {frame_type_registry::DATA, build_data_frame},
    {frame_type_registry::HEADERS, build_headers_frame},
    {frame_type_registry::PRIORITY, build_priority_frame},
    {frame_type_registry::RST_STREAM, build_rst_stream_frame},
    {frame_type_registry::SETTINGS, build_settings_frame},
    {frame_type_registry::PUSH_PROMISE, build_push_promise_frame},
    {frame_type_registry::PING, build_ping_frame},
    {frame_type_registry::GOAWAY, build_goaway_frame},
    {frame_type_registry::WINDOW_UPDATE, build_window_update_frame},
    {frame_type_registry::CONTINUATION, build_continuation_frame},
};

}  // namespace

h2_frame_ptr build_frame(const frame_header& fh, const byte_array_t& payload,
                         const dynamic_table& dynamic_table) {
  const auto key = cast_to_frame_type_registry(fh.m_type);
  const auto builder_func = builder_func_map.at(key);
  return builder_func(fh, payload, dynamic_table);
}

}  // namespace mh2c
