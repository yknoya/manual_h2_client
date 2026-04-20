find_package(GTest 1.10 REQUIRED)
find_package(Threads REQUIRED)

# Settings for unit test of libmh2c
include(GoogleTest)
add_executable(mh2c_test "")

if (TARGET GTest::gtest)
  set(mh2c_gtest_target GTest::gtest)
  set(mh2c_gtest_main_target GTest::gtest_main)
else()
  set(mh2c_gtest_target GTest::GTest)
  set(mh2c_gtest_main_target GTest::Main)
endif()

get_target_property(mh2c_gtest_location ${mh2c_gtest_target} IMPORTED_LOCATION)
if (NOT mh2c_gtest_location)
  get_target_property(mh2c_gtest_location ${mh2c_gtest_target} IMPORTED_LOCATION_RELEASE)
endif()
if (NOT mh2c_gtest_location)
  get_target_property(mh2c_gtest_location ${mh2c_gtest_target} IMPORTED_LOCATION_NOCONFIG)
endif()

get_target_property(mh2c_gtest_main_location ${mh2c_gtest_main_target} IMPORTED_LOCATION)
if (NOT mh2c_gtest_main_location)
  get_target_property(mh2c_gtest_main_location ${mh2c_gtest_main_target} IMPORTED_LOCATION_RELEASE)
endif()
if (NOT mh2c_gtest_main_location)
  get_target_property(mh2c_gtest_main_location ${mh2c_gtest_main_target} IMPORTED_LOCATION_NOCONFIG)
endif()

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
    Threads::Threads
    ${mh2c_gtest_target}
    ${mh2c_gtest_main_target}
)

if (APPLE)
  set(mh2c_test_runtime_dirs)
  foreach(gtest_location ${mh2c_gtest_location} ${mh2c_gtest_main_location})
    if (gtest_location)
      get_filename_component(gtest_dir "${gtest_location}" DIRECTORY)
      list(APPEND mh2c_test_runtime_dirs "${gtest_dir}")
    endif()
  endforeach()
  list(REMOVE_DUPLICATES mh2c_test_runtime_dirs)
  if (mh2c_test_runtime_dirs)
    set_property(TARGET mh2c_test PROPERTY BUILD_RPATH "${mh2c_test_runtime_dirs}")
  endif()
  if (mh2c_gtest_location AND mh2c_gtest_main_location)
    add_custom_command(TARGET mh2c_test POST_BUILD
      COMMAND /usr/bin/install_name_tool
        -change libgtest.dylib "${mh2c_gtest_location}"
        -change libgtest_main.dylib "${mh2c_gtest_main_location}"
        "$<TARGET_FILE:mh2c_test>"
      VERBATIM
    )
  endif()
endif()

gtest_add_tests(
  TARGET
    mh2c_test
  TEST_LIST
    mh2c_test_cases
)
