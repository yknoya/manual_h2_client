# Settings for sample code
add_executable(sample_h2_post_chunked_data "")

target_sources(sample_h2_post_chunked_data
  PRIVATE
    sample_h2_post_chunked_data.cpp
)

target_link_libraries(sample_h2_post_chunked_data
  PRIVATE
    mh2c
)
