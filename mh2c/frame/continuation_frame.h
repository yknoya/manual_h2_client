// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_CONTINUATION_FRAME_H_
#define MH2C_FRAME_CONTINUATION_FRAME_H_

#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.10
enum class cf_flag : fh_flags_t {
  END_HEADERS = 0x04,
};

class continuation_frame : public i_frame<frame_header> {
 public:
  continuation_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
                     const header_block_t& header_block,
                     const header_encode_mode mode,
                     const dynamic_table& dynamic_table);
  continuation_frame(const frame_header& fh, const byte_array_t& raw_payload,
                     const dynamic_table& dynamic_table);

  frame_header get_header() const override;
  header_block_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  byte_array_t m_encoded_payload;
  frame_header m_header;
  header_block_t m_header_block;

  friend bool operator==(const continuation_frame& lhs,
                         const continuation_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream,
                         const continuation_frame& hf);
bool operator==(const continuation_frame& lhs, const continuation_frame& rhs);
bool operator!=(const continuation_frame& lhs, const continuation_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_CONTINUATION_FRAME_H_
