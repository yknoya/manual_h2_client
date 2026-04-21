# Settings for sample code
add_executable(sample_h2_post "")

target_sources(sample_h2_post
  PRIVATE
    sample_h2_post.cpp
)

target_link_libraries(sample_h2_post
  PRIVATE
    mh2c
)
