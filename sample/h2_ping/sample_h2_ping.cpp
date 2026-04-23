// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include <iostream>
#include <string>

#include "mh2c/mh2c.h"
#include "sample/common/sample_common.h"

int main(int argc, char* argv[]) {
  try {
    const auto options =
        sample::parse_cli_options(argc, argv, "", false, false);
    if (options.show_help) {
      sample::print_usage(std::cout, argv[0], "", false, false);
      return 0;
    }

    mh2c::http2_client h2_client{options.host, options.port,
                                 sample::select_verify_mode(options)};
    sample::perform_settings_handshake(&h2_client);

    const mh2c::fh_stream_id_t stream_id{0u};
    const mh2c::ping_frame pf{0u, {'m', 'h', '2', 'c', 'p', 'i', 'n', 'g'}};
    h2_client.send_frame(pf);

    const auto last_frame_type = sample::receive_until_ping_ack(&h2_client);
    sample::send_goaway_if_needed(&h2_client, last_frame_type, stream_id);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
