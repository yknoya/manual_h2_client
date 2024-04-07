// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/settings_frame.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <map>
#include <ostream>
#include <string>
#include <unordered_map>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

/*
 * Anonymous namespace
 */
namespace {

const std::unordered_map<sf_parameter, std::string> sf_parameter_str_map{
    {sf_parameter::SETTINGS_HEADER_TABLE_SIZE, "SETTINGS_HEADER_TABLE_SIZE"},
    {sf_parameter::SETTINGS_ENABLE_PUSH, "SETTINGS_ENABLE_PUSH"},
    {sf_parameter::SETTINGS_MAX_CONCURRENT_STREAM,
     "SETTINGS_MAX_CONCURRENT_STREAM"},
    {sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE,
     "SETTINGS_INITIAL_WINDOW_SIZE"},
    {sf_parameter::SETTINGS_MAX_FRAME_SIZE, "SETTINGS_MAX_FRAME_SIZE"},
    {sf_parameter::SETTINGS_MAX_HEADER_LIST_SIZE,
     "SETTINGS_MAX_HEADER_LIST_SIZE"},
    {sf_parameter::SETTINGS_UNKNOWN, "UNKNOWN"},
};

frame_header construct_frame_header(fh_flags_t flags, fh_stream_id_t stream_id,
                                    sf_payload_t payload) {
  const fh_length_t length =
      (sizeof(sf_id_t) + sizeof(sf_value_t)) * payload.size();

  return {length, underlying_cast(frame_type_registry::SETTINGS), flags, 0,
          stream_id};
}

}  // namespace

/*
 * definitions of settings_frame
 */
settings_frame::settings_frame(const fh_flags_t flags,
                               const fh_stream_id_t stream_id,
                               const sf_payload_t& payload)
    : m_header{construct_frame_header(flags, stream_id, payload)},
      m_payload{payload} {}

frame_header settings_frame::get_header() const { return m_header; }

sf_payload_t settings_frame::get_payload() const { return m_payload; }

byte_array_t settings_frame::serialize() const {
  byte_array_t serialzed_sf = mh2c::serialize(m_header);

  std::for_each(m_payload.begin(), m_payload.end(),
                [&serialzed_sf](const sf_payload_t::value_type& elem) {
                  // "first" is id of setting
                  const sf_id_t id = cvt_host2net(elem.first);
                  const auto begin_id = reinterpret_cast<const uint8_t*>(&id);
                  std::copy(begin_id, begin_id + sizeof(id),
                            std::back_inserter(serialzed_sf));
                  // "second" is value of setting
                  const sf_value_t value = cvt_host2net(elem.second);
                  const auto begin_value =
                      reinterpret_cast<const uint8_t*>(&value);
                  std::copy(begin_value, begin_value + sizeof(value),
                            std::back_inserter(serialzed_sf));
                });

  return serialzed_sf;
}

void settings_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== SETTINGS FRAME ===\n" << m_header << "[PAYLOAD]\n";

  for (const auto& setting_pair : m_payload) {
    if (setting_pair.first <
            underlying_cast(sf_parameter::SETTINGS_HEADER_TABLE_SIZE) ||
        setting_pair.first >= underlying_cast(sf_parameter::SETTINGS_UNKNOWN)) {
      out_stream << "  "
                 << sf_parameter_str_map.at(sf_parameter::SETTINGS_UNKNOWN)
                 << '(' << std::to_string(setting_pair.first)
                 << "): " << setting_pair.second << '\n';
      continue;
    }

    const auto key = cast_to_sf_parameter(setting_pair.first);
    out_stream << "  " << sf_parameter_str_map.at(key) << ": "
               << setting_pair.second << '\n';
  }
}

/*
 * Definitions of function for settings_frame
 */
std::ostream& operator<<(std::ostream& out_stream, const settings_frame& sf) {
  sf.dump(out_stream);
  return out_stream;
}

bool operator==(const settings_frame& lhs, const settings_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_payload == rhs.m_payload;
}

bool operator!=(const settings_frame& lhs, const settings_frame& rhs) {
  return !(lhs == rhs);
}

sf_payload_t make_sf_payload(const sf_parameters& parameters) {
  sf_payload_t sf_payload{};
  std::for_each(parameters.begin(), parameters.end(),
                [&sf_payload](const auto& parameter) {
                  sf_payload.emplace(
                      make_sf_parameter(parameter.first, parameter.second));
                });
  return sf_payload;
}

}  // namespace mh2c
