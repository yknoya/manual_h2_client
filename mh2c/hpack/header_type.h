// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_HEADER_TYPE_H_
#define MH2C_HPACK_HEADER_TYPE_H_

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7541#section-6.2.1
//     https://tools.ietf.org/html/rfc7541#section-6.2.2
//     https://tools.ietf.org/html/rfc7541#section-6.2.3
//     https://tools.ietf.org/html/rfc7541#section-6.3
enum class header_prefix_pattern : uint8_t {
  WITHOUT_INDEXING = 0x00,
  NEVER_INDEXED = 0x10,
  SIZE_UPDATE = 0x20,
  INCREMENTAL_INDEXING = 0x40,
  INDEXED = 0x80,
};

enum class header_encode_mode : uint8_t {
  NONE = 0x00,
  HUFFMAN = 0x80,
};

// cf. https://tools.ietf.org/html/rfc7541#section-5.2
//     https://tools.ietf.org/html/rfc7541#section-6.1
//     https://tools.ietf.org/html/rfc7541#section-6.2
using header_index_t = uint8_t;
using header_name_length_t = uint8_t;
using header_name_t = std::string;
using header_value_length_t = uint8_t;
using header_value_t = std::string;
using header_t = std::pair<header_name_t, header_value_t>;
using headers_t = std::vector<header_t>;
using header_table_t = std::map<header_index_t, header_t>;

class header_block_entry {
 public:
  explicit header_block_entry(const header_t& header);
  header_block_entry(const header_prefix_pattern prefix,
                     const header_t& header);
  header_block_entry(const header_prefix_pattern prefix, const size_t max_size);

  header_prefix_pattern get_prefix() const;
  header_t get_header() const;
  size_t get_max_size() const;
  std::variant<header_t, size_t> get_entry() const;

  void set_header(const header_t& header);

 private:
  header_prefix_pattern m_prefix;
  std::variant<header_t, size_t> m_entry;
};
using header_block_t = std::vector<header_block_entry>;

bool operator==(const header_block_entry& lhs, const header_block_entry& rhs);
bool operator!=(const header_block_entry& lhs, const header_block_entry& rhs);

header_block_t make_header_block(const header_prefix_pattern prefix,
                                 const header_t& header);
header_block_t make_header_block(const header_prefix_pattern prefix,
                                 const headers_t& headers);

// This is used for header encoding
using reverse_header_table_t = std::map<header_t, header_index_t>;

}  // namespace mh2c

#endif  // MH2C_HPACK_HEADER_TYPE_H_
