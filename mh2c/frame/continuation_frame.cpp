// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/continuation_frame.h"

#include <algorithm>
#include <iterator>
#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_decoder.h"
#include "mh2c/hpack/header_encoder.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

/*
 * Anonymous namespace
 */
namespace {

byte_array_t construct_encoded_payload(const fh_flags_t flags,
                                       const header_block_t& header_block,
                                       const header_encode_mode mode,
                                       const dynamic_table& dynamic_table) {
  byte_array_t encoded_payload{};

  // Header Block
  std::for_each(
      header_block.begin(), header_block.end(),
      [&encoded_payload, mode, &dynamic_table](const auto& header_entry) {
        const auto encoded_header =
            encode_header(header_entry, mode, dynamic_table);
        std::copy(encoded_header.begin(), encoded_header.end(),
                  std::back_inserter(encoded_payload));
      });

  return encoded_payload;
}

frame_header construct_frame_header(fh_flags_t flags, fh_stream_id_t stream_id,
                                    byte_array_t encoded_payload) {
  return {static_cast<fh_length_t>(encoded_payload.size()),
          static_cast<fh_type_t>(frame_type_registry::CONTINUATION), flags, 0,
          stream_id};
}

header_block_t decode_payload(const frame_header& fh,
                              const byte_array_t& raw_payload,
                              const dynamic_table& dynamic_table) {
  auto raw_data{raw_payload};
  header_block_t header_block{};

  // Header Block
  while (raw_data.size() > 0) {
    const auto decoded_header = decode_header(raw_data, dynamic_table);
    header_block.push_back(decoded_header.first);
    raw_data.erase(raw_data.begin(), raw_data.begin() + decoded_header.second);
  }

  return header_block;
}

}  // namespace

/*
 * definitions of settings_frame
 */
continuation_frame::continuation_frame(const fh_flags_t flags,
                                       const fh_stream_id_t stream_id,
                                       const header_block_t& header_block,
                                       const header_encode_mode mode,
                                       const dynamic_table& dynamic_table)
    : m_encoded_payload{construct_encoded_payload(flags, header_block, mode,
                                                  dynamic_table)},
      m_header{construct_frame_header(flags, stream_id, m_encoded_payload)},
      m_header_block{header_block} {}

continuation_frame::continuation_frame(const frame_header& fh,
                                       const byte_array_t& raw_payload,
                                       const dynamic_table& dynamic_table)
    : m_encoded_payload{raw_payload},
      m_header{fh},
      m_header_block{decode_payload(fh, raw_payload, dynamic_table)} {}

frame_header continuation_frame::get_header() const { return m_header; }

header_block_t continuation_frame::get_payload() const { return m_header_block; }

byte_array_t continuation_frame::serialize() const {
  byte_array_t serialized_cf = mh2c::serialize(m_header);
  std::copy(m_encoded_payload.begin(), m_encoded_payload.end(),
            std::back_inserter(serialized_cf));
  return serialized_cf;
}

void continuation_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== CONTINUATION FRAME ===\n" << m_header << "[PAYLOAD]\n";

  out_stream << "  Header Block:\n";
  std::for_each(
      m_header_block.begin(), m_header_block.end(),
      [&out_stream](const auto& header_entry) {
        if (header_entry.get_prefix() == header_prefix_pattern::SIZE_UPDATE) {
          out_stream << "    " << std::to_string(header_entry.get_max_size())
                     << " (dynamic table size update)\n";
          return;
        }
        const auto header = header_entry.get_header();
        out_stream << "    " << header.first << ": " << header.second << '\n';
      });

  return;
}

std::ostream& operator<<(std::ostream& out_stream, const continuation_frame& hf) {
  hf.dump(out_stream);
  return out_stream;
}

bool operator==(const continuation_frame& lhs, const continuation_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_header_block == rhs.m_header_block;
}

bool operator!=(const continuation_frame& lhs, const continuation_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c
