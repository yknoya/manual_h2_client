// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_STATIC_TABLE_DEFINITION_H_
#define MH2C_HPACK_STATIC_TABLE_DEFINITION_H_

#include "mh2c/hpack/header_type.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7541#appendix-A
extern const header_table_t static_table_entries;

// This is used for header encoding
extern const reverse_header_table_t reverse_static_table_entries;

}  // namespace mh2c

#endif  // MH2C_HPACK_STATIC_TABLE_DEFINITION_H_
