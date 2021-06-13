find_package(OpenSSL 1.1 REQUIRED)

# Settings for libmh2c
add_library(mh2c "")

target_sources(mh2c
  PRIVATE
    frame/continuation_frame.cpp
    frame/data_frame.cpp
    frame/frame_builder.cpp
    frame/frame_header.cpp
    frame/goaway_frame.cpp
    frame/headers_frame.cpp
    frame/ping_frame.cpp
    frame/priority_frame.cpp
    frame/push_promise_frame.cpp
    frame/raw_frame.cpp
    frame/rst_stream_frame.cpp
    frame/settings_frame.cpp
    frame/window_update_frame.cpp
    hpack/dynamic_table.cpp
    hpack/header_decoder.cpp
    hpack/header_encoder.cpp
    hpack/header_type.cpp
    hpack/huffman_code.cpp
    hpack/huffman_decoder.cpp
    hpack/huffman_encoder.cpp
    hpack/static_table_definition.cpp
    http2_client.cpp
    ssl/ssl_connection.cpp
    ssl/ssl_ctx.cpp
    ssl/ssl_bio.cpp
    util/byte_order.cpp
)

target_include_directories(mh2c
  PUBLIC
    ${PROJECT_SOURCE_DIR}
)

target_compile_options(mh2c
  PRIVATE
    "-Werror"
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-g"
)

set_target_properties(mh2c
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    SOVERSION ${PROJECT_VERSION}
    OUTPUT_NAME "mh2c"
    DEBUG_POSTFIX "_d"
    MACOSX_RPATH OFF
)

target_link_libraries(mh2c
  PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

install(TARGETS mh2c
  LIBRARY
    COMPONENT lib
    DESTINATION ${INSTALL_LIBDIR}
)

set(mh2c_private_headers
  "frame/frame_builder.h"
  "hpack/header_decoder.h"
  "hpack/header_encoder.h"
  "hpack/huffman_code.h"
  "hpack/huffman_decoder.h"
  "hpack/huffman_encoder.h"
  "hpack/integer_representation.h"
  "hpack/static_table_definition.h"
  "ssl/ssl_bio.h"
  "ssl/ssl_connection.h"
  "ssl/ssl_ctx.h"
  "util/byte_order.h"
)

message(STATUS "private headers: ${mh2c_private_headers}")

string(REPLACE ";" "|" mh2c_exclude_regex "${mh2c_private_headers}")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/mh2c
  DESTINATION ${INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.ipp" REGEX ${mh2c_exclude_regex} EXCLUDE
)
