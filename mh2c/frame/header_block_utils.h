#ifndef MH2C_FRAME_HEADER_BLOCK_UTILS_H_
#define MH2C_FRAME_HEADER_BLOCK_UTILS_H_

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_decoder_internal.h"
#include "mh2c/hpack/header_encoder.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {
namespace detail {

inline void append_encoded_header_block(byte_array_t* encoded_payload,
                                        const header_block_t& header_block,
                                        const header_encode_mode mode,
                                        const dynamic_table& dynamic_table) {
  std::for_each(
      header_block.begin(), header_block.end(),
      [encoded_payload, mode, &dynamic_table](const auto& header_entry) {
        const auto encoded_header =
            encode_header(header_entry, mode, dynamic_table);
        std::copy(encoded_header.begin(), encoded_header.end(),
                  std::back_inserter(*encoded_payload));
      });
}

inline header_block_t decode_header_block(byte_array_t::const_iterator begin,
                                          byte_array_t::const_iterator end,
                                          const dynamic_table& dynamic_table) {
  header_block_t header_block{};
  for (auto iter = begin; iter != end;) {
    const auto decoded_header = decode_header_range(iter, end, dynamic_table);
    header_block.push_back(decoded_header.first);
    iter += decoded_header.second;
  }
  return header_block;
}

inline void dump_header_block(std::ostream& out_stream,
                              const header_block_t& header_block) {
  std::for_each(
      header_block.begin(), header_block.end(),
      [&out_stream](const auto& header_entry) {
        if (header_entry.get_prefix() == header_prefix_pattern::SIZE_UPDATE) {
          out_stream << "    " << std::to_string(header_entry.get_max_size())
                     << " (dynamic table size update)\n";
          return;
        }
        const auto header = header_entry.get_header();
        out_stream << "    " << header.first << ": " << header.second << '\n';
      });
}

inline void dump_hex_bytes(std::ostream& out_stream, const byte_array_t& bytes,
                           const char* indent = "    ") {
  uint32_t counter{0};
  out_stream << indent;
  std::for_each(bytes.begin(), bytes.end(),
                [&out_stream, &counter, indent](const auto& elem) {
                  out_stream << std::hex << std::setw(2) << std::setfill('0')
                             << static_cast<int>(elem);
                  if (++counter % 8 == 0) {
                    out_stream << '\n' << indent;
                  } else {
                    out_stream << ' ';
                  }
                });
  out_stream << '\n';
}

}  // namespace detail
}  // namespace mh2c

#endif  // MH2C_FRAME_HEADER_BLOCK_UTILS_H_
