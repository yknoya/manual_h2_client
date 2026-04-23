// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_HTTP2_CLIENT_INTERNAL_H_
#define MH2C_HTTP2_CLIENT_INTERNAL_H_

#include "mh2c/frame/common_type.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {
namespace detail {

void update_dynamic_table(const header_block_t& header_block,
                          dynamic_table* dynamic_table);
void update_dynamic_table(const h2_frame_ptr& frame_ptr,
                          dynamic_table* dynamic_table);

}  // namespace detail
}  // namespace mh2c

#endif  // MH2C_HTTP2_CLIENT_INTERNAL_H_
