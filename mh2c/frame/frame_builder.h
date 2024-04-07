// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_FRAME_BUILDER_H_
#define MH2C_FRAME_FRAME_BUILDER_H_

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

h2_frame_ptr build_frame(const frame_header& fh,
                         const byte_array_t& raw_payload,
                         const dynamic_table& dynamic_table);

}  // namespace mh2c

#endif  // MH2C_FRAME_FRAME_BUILDER_H_
