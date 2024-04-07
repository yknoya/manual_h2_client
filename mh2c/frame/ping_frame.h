// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_PING_FRAME_H_
#define MH2C_FRAME_PING_FRAME_H_

#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.7
enum class pf_flag : fh_flags_t {
  ACK = 0x1,
};

class ping_frame : public i_frame<frame_header> {
 public:
  ping_frame(const fh_flags_t flags, const byte_array_t& opaque_data);
  ping_frame(const frame_header& fh, const byte_array_t& raw_payload);

  frame_header get_header() const override;
  byte_array_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  byte_array_t m_opaque_data;

  friend bool operator==(const ping_frame& lhs, const ping_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const ping_frame& pf);
bool operator==(const ping_frame& lhs, const ping_frame& rhs);
bool operator!=(const ping_frame& lhs, const ping_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_PING_FRAME_H_
