// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_HUFFMAN_DECODER_H_
#define MH2C_HPACK_HUFFMAN_DECODER_H_

#include "mh2c/common/byte_array.h"

namespace mh2c {

namespace huffman {

byte_array_t decode(const byte_array_t& encoded_data);

}  // namespace huffman

}  // namespace mh2c

#endif  // MH2C_HPACK_HUFFMAN_DECODER_H_
