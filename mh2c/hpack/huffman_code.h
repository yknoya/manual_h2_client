/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, k-noya
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
