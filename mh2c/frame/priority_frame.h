// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_PRIORITY_FRAME_H_
#define MH2C_FRAME_PRIORITY_FRAME_H_

#include <cstdint>
#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.3
struct priority_payload {
  uint8_t m_exclusive;
  fh_stream_id_t m_stream_dependency;
  uint8_t m_weight;
};

bool operator==(const priority_payload& lhs, const priority_payload& rhs);
bool operator!=(const priority_payload& lhs, const priority_payload& rhs);

class priority_frame : public i_frame<frame_header> {
 public:
  priority_frame(const fh_stream_id_t stream_id, const priority_payload& payload);
  priority_frame(const frame_header& fh, const byte_array_t& raw_payload);

  frame_header get_header() const override;
  priority_payload get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  priority_payload m_payload;

  friend bool operator==(const priority_frame& lhs, const priority_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const priority_frame& pf);
bool operator==(const priority_frame& lhs, const priority_frame& rhs);
bool operator!=(const priority_frame& lhs, const priority_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_PRIORITY_FRAME_H_
