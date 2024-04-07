// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_FRAME_HEADER_H_
#define MH2C_FRAME_FRAME_HEADER_H_

#include <cstdint>
#include <ostream>

#include "mh2c/common/byte_array.h"

namespace mh2c {

/*
 * [Frame Fromat]
 * +-----------------------------------------------+
 * |                 Length (24)                   |
 * +---------------+---------------+---------------+
 * |   Type (8)    |   Flags (8)   |
 * +-+-+-----------+---------------+-------------------------------+
 * |R|                 Stream Identifier (31)                      |
 * +=+=============================================================+
 * |                   Frame Payload (0...)                      ...
 * +---------------------------------------------------------------+
 * cf. https://tools.ietf.org/html/rfc7540#section-4.1
 */
using fh_length_t = uint32_t;
using fh_type_t = uint8_t;
using fh_flags_t = uint8_t;
using fh_reserved_t = uint8_t;
using fh_stream_id_t = uint32_t;

constexpr uint8_t FRAME_HEADER_BYTES{9u};
constexpr uint8_t LENGTH_BITS{24u};
constexpr uint8_t RESERVED_BITS{1u};
constexpr uint8_t STREAM_ID_BITS{31u};
constexpr fh_stream_id_t MAX_STREAM_ID{0x7fffffff};

struct frame_header {
  fh_length_t m_length;  // Actually 24 bit
  fh_type_t m_type;
  fh_flags_t m_flags;
  fh_reserved_t m_reserved;    // Actually 1 bit
  fh_stream_id_t m_stream_id;  // Actually 31 bit
};

byte_array_t serialize(const frame_header& fh);
frame_header build_frame_header(const byte_array_t& raw_data);

std::ostream& operator<<(std::ostream& out_stream, const frame_header& fh);
bool operator==(const frame_header& lhs, const frame_header& rhs);
bool operator!=(const frame_header& lhs, const frame_header& rhs);

template <typename T>
inline fh_length_t cast_to_fh_length(T length);

template <typename... Args>
inline fh_flags_t make_frame_header_flags(Args... args);

}  // namespace mh2c

#include "mh2c/frame/frame_header.ipp"

#endif  // MH2C_FRAME_FRAME_HEADER_H_
