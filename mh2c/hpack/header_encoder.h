// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_HEADER_ENCODER_H_
#define MH2C_HPACK_HEADER_ENCODER_H_

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

byte_array_t encode_header(const header_block_entry& header,
                           const header_encode_mode mode,
                           const dynamic_table& dynamic_table);

}  // namespace mh2c

#endif  // MH2C_HPACK_HEADER_ENCODER_H_
