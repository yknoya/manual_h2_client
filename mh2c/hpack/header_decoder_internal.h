#ifndef MH2C_HPACK_HEADER_DECODER_INTERNAL_H_
#define MH2C_HPACK_HEADER_DECODER_INTERNAL_H_

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/header_decoder.h"

namespace mh2c {

decoded_header_t decode_header_range(byte_array_t::const_iterator begin,
                                     byte_array_t::const_iterator end,
                                     const dynamic_table& dynamic_table);

}  // namespace mh2c

#endif  // MH2C_HPACK_HEADER_DECODER_INTERNAL_H_
