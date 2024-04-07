// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#ifndef MH2C_HTTP2_CLIENT_IPP_
#define MH2C_HTTP2_CLIENT_IPP_

namespace mh2c {

template <typename Frame>
void http2_client::send_frame(const Frame& frame) {
  const auto raw_frame = frame.serialize();
  send_raw_data(raw_frame.data(), raw_frame.size());
  return;
}

}  // namespace mh2c

#endif  // MH2C_HTTP2_CLIENT_IPP_
