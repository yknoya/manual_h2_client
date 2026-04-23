# Settings for sample code
add_executable(sample_h2_window_update "")

target_sources(sample_h2_window_update
  PRIVATE
    ../common/sample_common.cpp
    sample_h2_window_update.cpp
)

target_link_libraries(sample_h2_window_update
  PRIVATE
    mh2c
)
