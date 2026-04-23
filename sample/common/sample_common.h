// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef SAMPLE_COMMON_SAMPLE_COMMON_H_
#define SAMPLE_COMMON_SAMPLE_COMMON_H_

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>

#include "mh2c/mh2c.h"

namespace sample {

struct cli_options {
  std::string host{"nghttp2.org"};
  uint16_t port{443};
  std::string path{};
  std::string authority{};
  bool insecure{false};
  bool show_help{false};
};

cli_options parse_cli_options(int argc, char* argv[],
                              const std::string& default_path,
                              bool enable_path = true,
                              bool enable_authority = true);
void print_usage(std::ostream& out_stream, const char* program_name,
                 const std::string& default_path, bool enable_path = true,
                 bool enable_authority = true);

mh2c::ssl::verify_mode select_verify_mode(const cli_options& options);
void perform_settings_handshake(mh2c::http2_client* h2_client,
                                size_t initial_table_size = 8192u);
mh2c::fh_type_t receive_frames(mh2c::http2_client* h2_client);
mh2c::fh_type_t receive_until_ping_ack(mh2c::http2_client* h2_client);
void send_goaway_if_needed(mh2c::http2_client* h2_client,
                           mh2c::fh_type_t last_frame_type,
                           mh2c::fh_stream_id_t stream_id);

}  // namespace sample

#endif  // SAMPLE_COMMON_SAMPLE_COMMON_H_
