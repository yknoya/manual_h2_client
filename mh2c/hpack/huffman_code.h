// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_HUFFMAN_CODE_H_
#define MH2C_HPACK_HUFFMAN_CODE_H_

#include <cstdint>
#include <map>
#include <utility>
#include <vector>

namespace mh2c {

namespace huffman {

// cf. https://tools.ietf.org/html/rfc7541#appendix-B
using encode_target = uint16_t;
using encoded_value_t = uint32_t;
using encoded_length_t = uint8_t;
using encode_table_value = std::pair<encoded_value_t, encoded_length_t>;
using encode_table_t = std::map<encode_target, encode_table_value>;

using decoded_value_t = encode_target;
using decode_table_record_t =
    std::pair<encoded_length_t, std::map<encoded_value_t, decoded_value_t>>;
using decode_table_t = std::vector<decode_table_record_t>;

extern const encode_table_t encode_table;
extern const decode_table_t decode_table;

}  // namespace huffman

}  // namespace mh2c

#endif  // MH2C_HPACK_HUFFMAN_CODE_H_
