// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/priority_frame.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"
#include "mh2c/util/cast.h"

namespace mh2c {

namespace {

constexpr fh_length_t PRIORITY_PAYLOAD_SIZE = 5u;
constexpr uint8_t EXCLUSIVE_BITS{1u};

frame_header construct_frame_header(const fh_stream_id_t stream_id) {
  return {PRIORITY_PAYLOAD_SIZE, underlying_cast(frame_type_registry::PRIORITY),
          0u, 0u, stream_id};
}

priority_payload construct_payload(const byte_array_t& raw_payload) {
  const auto payload_size = raw_payload.size();
  if (payload_size < PRIORITY_PAYLOAD_SIZE) {
    const auto msg = "invalid payload size: " + std::to_string(payload_size);
    throw std::invalid_argument(msg);
  }

  const uint8_t exclusive_flag =
      extract_high_bit<EXCLUSIVE_BITS>(raw_payload[0]);
  const auto stream_dependency = extract_low_bit<STREAM_ID_BITS>(
      bytes2integral<fh_stream_id_t>(raw_payload.begin()));
  const uint8_t weight = raw_payload[4];

  return {exclusive_flag, stream_dependency, weight};
}

void validate_payload(const priority_payload& payload) {
  if (payload.m_exclusive > 1u) {
    const auto msg =
        "invalid exclusive: " + std::to_string(payload.m_exclusive);
    throw std::invalid_argument(msg);
  }

  if (payload.m_stream_dependency > MAX_STREAM_ID) {
    const auto msg = "invalid stream dependency: " +
                     std::to_string(payload.m_stream_dependency);
    throw std::invalid_argument(msg);
  }

  return;
}

}  // namespace

bool operator==(const priority_payload& lhs, const priority_payload& rhs) {
  return lhs.m_exclusive == rhs.m_exclusive &&
         lhs.m_stream_dependency == rhs.m_stream_dependency &&
         lhs.m_weight == rhs.m_weight;
}

bool operator!=(const priority_payload& lhs, const priority_payload& rhs) {
  return !(lhs == rhs);
}

priority_frame::priority_frame(const fh_stream_id_t stream_id,
                               const priority_payload& payload)
    : m_header{construct_frame_header(stream_id)}, m_payload{payload} {
  validate_payload(m_payload);
}

priority_frame::priority_frame(const frame_header& fh,
                               const byte_array_t& raw_payload)
    : m_header{fh}, m_payload{construct_payload(raw_payload)} {
  validate_payload(m_payload);
}

frame_header priority_frame::get_header() const { return m_header; }

priority_payload priority_frame::get_payload() const { return m_payload; }

byte_array_t priority_frame::serialize() const {
  byte_array_t serialized_pf = mh2c::serialize(m_header);

  // Exclusive flag and Stream Dependency
  decltype(m_payload.m_stream_dependency) exclusive_and_stream_dependency =
      cvt_host2net((extract_low_bit<EXCLUSIVE_BITS>(m_payload.m_exclusive)
                    << STREAM_ID_BITS) |
                   m_payload.m_stream_dependency);

  const auto begin =
      reinterpret_cast<uint8_t*>(&exclusive_and_stream_dependency);
  std::copy(begin, begin + sizeof(exclusive_and_stream_dependency),
            std::back_inserter(serialized_pf));

  // Weight
  serialized_pf.push_back(m_payload.m_weight);

  return serialized_pf;
}

void priority_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== PRIORITY FRAME ===\n";
  out_stream << m_header;
  out_stream << "[PAYLOAD]\n";
  out_stream << "  E: " << std::to_string(m_payload.m_exclusive) << '\n';
  out_stream << "  Stream Dependency: "
             << std::to_string(m_payload.m_stream_dependency) << '\n';
  out_stream << "  Weight: " << std::to_string(m_payload.m_weight) << '\n';
  return;
}

std::ostream& operator<<(std::ostream& out_stream, const priority_frame& pf) {
  pf.dump(out_stream);
  return out_stream;
}

bool operator==(const priority_frame& lhs, const priority_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_payload == rhs.m_payload;
}

bool operator!=(const priority_frame& lhs, const priority_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c
