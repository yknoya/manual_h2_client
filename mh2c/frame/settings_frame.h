// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_SETTINGS_FRAME_H_
#define MH2C_FRAME_SETTINGS_FRAME_H_

#include <cstdint>
#include <initializer_list>
#include <map>
#include <ostream>
#include <utility>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

enum class sf_flag : fh_flags_t {
  ACK = 0x01,
};

// cf. https://tools.ietf.org/html/rfc7540#section-6.5.1
using sf_id_t = uint16_t;
using sf_value_t = uint32_t;
using sf_pair_t = std::pair<sf_id_t, sf_value_t>;
using sf_payload_t = std::map<sf_id_t, sf_value_t>;

// cf. https://tools.ietf.org/html/rfc7540#section-6.5.2
enum class sf_parameter : sf_id_t {
  SETTINGS_HEADER_TABLE_SIZE = 0x01,
  SETTINGS_ENABLE_PUSH = 0x02,
  SETTINGS_MAX_CONCURRENT_STREAM = 0x03,
  SETTINGS_INITIAL_WINDOW_SIZE = 0x04,
  SETTINGS_MAX_FRAME_SIZE = 0x05,
  SETTINGS_MAX_HEADER_LIST_SIZE = 0x06,
  SETTINGS_UNKNOWN = 0x07,
};

class settings_frame : public i_frame<frame_header> {
 public:
  settings_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
                 const sf_payload_t& payload);

  frame_header get_header() const override;
  sf_payload_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  sf_payload_t m_payload;

  friend bool operator==(const settings_frame& lhs, const settings_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const settings_frame& sf);
bool operator==(const settings_frame& lhs, const settings_frame& rhs);
bool operator!=(const settings_frame& lhs, const settings_frame& rhs);

template <typename T>
inline sf_parameter cast_to_sf_parameter(T param);

template <typename T>
inline sf_pair_t make_sf_parameter(T id, const sf_value_t value);

using sf_parameters =
    std::initializer_list<std::pair<sf_parameter, sf_value_t>>;
sf_payload_t make_sf_payload(const sf_parameters& parameters);

}  // namespace mh2c

#include "mh2c/frame/settings_frame.ipp"

#endif  // MH2C_FRAME_SETTINGS_FRAME_H_
