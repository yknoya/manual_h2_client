// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_DATA_FRAME_H_
#define MH2C_FRAME_DATA_FRAME_H_

#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.1
enum class df_flag : fh_flags_t {
  END_STREAM = 0x01,
  PADDED = 0x08,
};

class data_frame : public i_frame<frame_header> {
 public:
  data_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
             const byte_array_t& payload);
  data_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
             const std::string& payload);

  frame_header get_header() const override;
  byte_array_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  byte_array_t m_payload;

  friend bool operator==(const data_frame& lhs, const data_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const data_frame& sf);
bool operator==(const data_frame& lhs, const data_frame& rhs);
bool operator!=(const data_frame& lhs, const data_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_DATA_FRAME_H_
