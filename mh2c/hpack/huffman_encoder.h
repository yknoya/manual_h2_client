// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_HUFFMAN_ENCODER_H_
#define MH2C_HPACK_HUFFMAN_ENCODER_H_

#include "mh2c/common/byte_array.h"

namespace mh2c {

namespace huffman {

byte_array_t encode(const byte_array_t& raw_data);

}  // namespace huffman

}  // namespace mh2c

#endif  // MH2C_HPACK_HUFFMAN_ENCODER_H_
