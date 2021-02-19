// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_HEADER_DECODER_H_
#define MH2C_HPACK_HEADER_DECODER_H_

#include <utility>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

using decoded_header_t = std::pair<header_block_entry, size_t>;

decoded_header_t decode_header(const byte_array_t& encoded_header,
                               const dynamic_table& dynamic_table);

}  // namespace mh2c

#endif  // MH2C_HPACK_HEADER_DECODER_H_
