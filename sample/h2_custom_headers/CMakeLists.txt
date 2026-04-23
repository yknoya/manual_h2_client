# Settings for sample code
add_executable(sample_h2_custom_headers "")

target_sources(sample_h2_custom_headers
  PRIVATE
    ../common/sample_common.cpp
    sample_h2_custom_headers.cpp
)

target_link_libraries(sample_h2_custom_headers
  PRIVATE
    mh2c
)
