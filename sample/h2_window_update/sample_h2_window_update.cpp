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
        sample::parse_cli_options(argc, argv, "/httpbin/headers");
    if (options.show_help) {
      sample::print_usage(std::cout, argv[0], "/httpbin/headers");
      return 0;
    }

    mh2c::http2_client h2_client{options.host, options.port,
                                 sample::select_verify_mode(options)};
    sample::perform_settings_handshake(&h2_client);

    const mh2c::fh_flags_t flags{make_frame_header_flags(
        mh2c::hf_flag::END_STREAM, mh2c::hf_flag::END_HEADERS)};
    const mh2c::fh_stream_id_t stream_id{1u};
    const mh2c::header_block_t header_block{
        mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                                mh2c::headers_t{
                                    {":method", "GET"},
                                    {":path", options.path},
                                    {":scheme", "https"},
                                    {":authority", options.authority},
                                })};
    const mh2c::headers_frame hf{flags, stream_id, header_block,
                                 mh2c::header_encode_mode::HUFFMAN,
                                 h2_client.get_request_dynamic_table()};
    h2_client.send_frame(hf);
    h2_client.update_request_dynamic_table(hf.get_payload());

    h2_client.send_frame(mh2c::window_update_frame{0u, 1024u});
    h2_client.send_frame(mh2c::window_update_frame{stream_id, 1024u});

    const auto last_frame_type = sample::receive_frames(&h2_client);
    sample::send_goaway_if_needed(&h2_client, last_frame_type, stream_id);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
