// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/frame_header.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

byte_array_t serialize(const frame_header& fh) {
  byte_array_t serialized_fh;

  // Convert byte order of length.
  const auto length = cvt_host2net(fh.m_length);

  // Serialize length. Notice that length is actually 24 bit.
  auto begin = reinterpret_cast<const uint8_t*>(&length);
  std::copy(begin + 1, begin + sizeof(length), std::back_inserter(serialized_fh));

  // Serialize type.
  serialized_fh.push_back(fh.m_type);

  // Serialize flags.
  serialized_fh.push_back(fh.m_flags);

  // Convert byte order of reserved and length.
  // Notice that reserved is actually 1 bit.
  const uint32_t reserved_and_stream_id = cvt_host2net(
      (extract_low_bit<RESERVED_BITS>(fh.m_reserved) << STREAM_ID_BITS) |
      fh.m_stream_id);

  // Serialize reserved and stream_id.
  begin = reinterpret_cast<const uint8_t*>(&reserved_and_stream_id);
  std::copy(begin, begin + sizeof(reserved_and_stream_id),
            std::back_inserter(serialized_fh));

  return serialized_fh;
}

frame_header build_frame_header(const byte_array_t& raw_fh) {
  const fh_length_t length =
      extract_high_bit<LENGTH_BITS>(bytes2integral<fh_length_t>(raw_fh.begin()));
  const fh_type_t type = raw_fh[3];
  const fh_flags_t flags = raw_fh[4];
  const fh_stream_id_t reserved_and_stream_id =
      bytes2integral<fh_stream_id_t>(&raw_fh[5]);
  const fh_reserved_t reserved =
      extract_high_bit<RESERVED_BITS>(reserved_and_stream_id);
  const fh_stream_id_t stream_id =
      extract_low_bit<STREAM_ID_BITS>(reserved_and_stream_id);

  return {length, type, flags, reserved, stream_id};
}

std::ostream& operator<<(std::ostream& out_stream, const frame_header& fh) {
  return out_stream << "[Frame Header]\n"
                    << "  length    : " << std::to_string(fh.m_length) << '\n'
                    << "  type      : " << std::to_string(fh.m_type) << '\n'
                    << "  flags     : " << std::to_string(fh.m_flags) << '\n'
                    << "  reserved  : " << std::to_string(fh.m_reserved) << '\n'
                    << "  stream id : " << std::to_string(fh.m_stream_id) << '\n';
}

bool operator==(const frame_header& lhs, const frame_header& rhs) {
  return lhs.m_length == rhs.m_length && lhs.m_type == rhs.m_type &&
         lhs.m_flags == rhs.m_flags && lhs.m_reserved == rhs.m_reserved &&
         lhs.m_stream_id == rhs.m_stream_id;
}

bool operator!=(const frame_header& lhs, const frame_header& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c
