# Settings for sample code
add_executable(sample_h2_ping "")

target_sources(sample_h2_ping
  PRIVATE
    sample_h2_ping.cpp
)

target_link_libraries(sample_h2_ping
  PRIVATE
    mh2c
)
