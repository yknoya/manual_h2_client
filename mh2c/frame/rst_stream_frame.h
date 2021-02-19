// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_RST_STREAM_FRAME_H_
#define MH2C_FRAME_RST_STREAM_FRAME_H_

#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.4
class rst_stream_frame : public i_frame<frame_header> {
 public:
  rst_stream_frame(const fh_stream_id_t stream_id, const error_codes error_code);
  rst_stream_frame(const frame_header& fh, const byte_array_t& raw_payload);

  frame_header get_header() const override;
  error_codes get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  error_codes m_error_code;

  friend bool operator==(const rst_stream_frame& lhs,
                         const rst_stream_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const rst_stream_frame& rsf);
bool operator==(const rst_stream_frame& lhs, const rst_stream_frame& rhs);
bool operator!=(const rst_stream_frame& lhs, const rst_stream_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_RST_STREAM_FRAME_H_
