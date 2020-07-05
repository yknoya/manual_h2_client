find_package(GTest 1.10 REQUIRED)

# Settings for unit test of libmh2c
include(GoogleTest)
add_executable(mh2c_test "")

target_sources(mh2c_test
  PRIVATE
    frame/continuation_frame_test.cpp
    frame/data_frame_test.cpp
    frame/frame_builder_test.cpp
    frame/frame_header_test.cpp
    frame/goaway_frame_test.cpp
    frame/headers_frame_test.cpp
    frame/ping_frame_test.cpp
    frame/priority_frame_test.cpp
    frame/push_promise_frame_test.cpp
    frame/rst_stream_frame_test.cpp
    frame/settings_frame_test.cpp
    frame/window_update_frame_test.cpp
    hpack/dynamic_table_test.cpp
    hpack/header_decoder_test.cpp
    hpack/header_encoder_test.cpp
    hpack/huffman_decoder_test.cpp
    hpack/huffman_encoder_test.cpp
    hpack/integer_representation_test.cpp
    hpack/static_table_definition_test.cpp
    util/bit_operation_test.cpp
    util/byte_order_test.cpp
)

target_include_directories(mh2c_test
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(mh2c_test
  PRIVATE
    mh2c
    pthread
    GTest::GTest
    GTest::Main
)

gtest_add_tests(
  TARGET
    mh2c_test
)
