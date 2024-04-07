// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_RAW_FRAME_H_
#define MH2C_FRAME_RAW_FRAME_H_

#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

// ToDo: Remove later.
class raw_frame : public i_frame<frame_header> {
 public:
  raw_frame(const frame_header& header, const byte_array_t& payload);

  frame_header get_header() const override;
  byte_array_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  byte_array_t m_payload;
};

std::ostream& operator<<(std::ostream& out_stream, const raw_frame& sf);

}  // namespace mh2c

#endif  // MH2C_FRAME_RAW_FRAME_H_
