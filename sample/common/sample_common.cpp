// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "sample/common/sample_common.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

#include "mh2c/util/cast.h"

namespace sample {
namespace {

bool is_more_data(const mh2c::frame_header& fh) {
  switch (mh2c::cast_to_frame_type_registry(fh.m_type)) {
    case mh2c::frame_type_registry::DATA:
      return !mh2c::is_flag_set(fh.m_flags, mh2c::df_flag::END_STREAM);
    case mh2c::frame_type_registry::GOAWAY:
      return false;
    default:
      return true;
  }
}

bool is_ping_ack(const mh2c::frame_header& fh) {
  return mh2c::cast_to_frame_type_registry(fh.m_type) ==
             mh2c::frame_type_registry::PING &&
         mh2c::is_flag_set(fh.m_flags, mh2c::pf_flag::ACK);
}

const char* require_option_value(int* index, int argc, char* argv[],
                                 const std::string& option_name) {
  ++(*index);
  if (*index >= argc) {
    throw std::invalid_argument("missing value for " + option_name);
  }

  return argv[*index];
}

uint16_t parse_port(const std::string& raw_port) {
  size_t processed_chars{0u};
  const auto parsed_port = std::stoul(raw_port, &processed_chars, 10);
  if (processed_chars != raw_port.size()) {
    throw std::invalid_argument("invalid --port value: " + raw_port);
  }
  if (parsed_port > std::numeric_limits<uint16_t>::max()) {
    throw std::invalid_argument("port is out of range: " + raw_port);
  }

  return static_cast<uint16_t>(parsed_port);
}

}  // namespace

cli_options parse_cli_options(int argc, char* argv[],
                              const std::string& default_path, bool enable_path,
                              bool enable_authority) {
  cli_options options{};
  options.path = default_path;

  for (int i = 1; i < argc; ++i) {
    const std::string_view arg{argv[i]};
    if (arg == "--help") {
      options.show_help = true;
      continue;
    }
    if (arg == "--host") {
      options.host = require_option_value(&i, argc, argv, "--host");
      continue;
    }
    if (arg == "--port") {
      options.port = parse_port(require_option_value(&i, argc, argv, "--port"));
      continue;
    }
    if (enable_authority && arg == "--authority") {
      options.authority = require_option_value(&i, argc, argv, "--authority");
      continue;
    }
    if (arg == "--insecure") {
      options.insecure = true;
      continue;
    }
    if (enable_path && arg == "--path") {
      options.path = require_option_value(&i, argc, argv, "--path");
      continue;
    }

    throw std::invalid_argument("unknown option: " + std::string(arg));
  }

  if (options.authority.empty()) {
    options.authority = options.host;
  }

  return options;
}

void print_usage(std::ostream& out_stream, const char* program_name,
                 const std::string& default_path, bool enable_path,
                 bool enable_authority) {
  out_stream << "Usage: " << program_name << " [--host HOST] [--port PORT]";
  if (enable_authority) {
    out_stream << " [--authority AUTHORITY]";
  }
  if (enable_path) {
    out_stream << " [--path PATH]";
  }
  out_stream << " [--insecure] [--help]\n";
  out_stream << "Defaults:\n";
  out_stream << "  --host nghttp2.org\n";
  out_stream << "  --port 443\n";
  if (enable_authority) {
    out_stream << "  --authority <host>\n";
  }
  if (enable_path) {
    out_stream << "  --path " << default_path << '\n';
  }
  out_stream << "  TLS verification enabled\n";
}

mh2c::ssl::verify_mode select_verify_mode(const cli_options& options) {
  return options.insecure ? mh2c::ssl::verify_mode::VERIFY_NONE
                          : mh2c::ssl::verify_mode::VERIFY_SERVER_CERT;
}

void perform_settings_handshake(mh2c::http2_client* h2_client,
                                const size_t initial_table_size) {
  h2_client->send_connection_preface();

  const auto sf_payload{mh2c::make_sf_payload(
      {{mh2c::sf_parameter::SETTINGS_ENABLE_PUSH, 0u},
       {mh2c::sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE, 65535u},
       {mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE, initial_table_size}})};
  const mh2c::settings_frame sf{0u, 0u, sf_payload};
  h2_client->send_frame(sf);
  h2_client->update_request_dynamic_table(initial_table_size);

  const auto frame = h2_client->receive_frame();
  std::cout << frame;

  const mh2c::settings_frame sf_ack{
      mh2c::make_frame_header_flags(mh2c::sf_flag::ACK), 0u, {}};
  h2_client->send_frame(sf_ack);
}

mh2c::fh_type_t receive_frames(mh2c::http2_client* h2_client) {
  mh2c::fh_type_t last_frame_type{};
  try {
    while (auto frame = h2_client->receive_frame()) {
      std::cout << frame;

      const auto& frame_header = frame->get_header();
      last_frame_type = frame_header.m_type;
      if (!is_more_data(frame_header)) {
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    throw;
  }

  return last_frame_type;
}

mh2c::fh_type_t receive_until_ping_ack(mh2c::http2_client* h2_client) {
  mh2c::fh_type_t last_frame_type{};
  try {
    while (auto frame = h2_client->receive_frame()) {
      std::cout << frame;

      const auto& frame_header = frame->get_header();
      last_frame_type = frame_header.m_type;
      if (is_ping_ack(frame_header) ||
          mh2c::cast_to_frame_type_registry(frame_header.m_type) ==
              mh2c::frame_type_registry::GOAWAY) {
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    throw;
  }

  return last_frame_type;
}

void send_goaway_if_needed(mh2c::http2_client* h2_client,
                           const mh2c::fh_type_t last_frame_type,
                           const mh2c::fh_stream_id_t stream_id) {
  if (last_frame_type !=
      mh2c::underlying_cast(mh2c::frame_type_registry::GOAWAY)) {
    const mh2c::goaway_payload gf_payload{
        0x0, stream_id, mh2c::error_codes::NO_ERROR, {}};
    const mh2c::goaway_frame gf{gf_payload};
    h2_client->send_frame(gf);
  }
}

}  // namespace sample
