# Settings for sample code
add_executable(sample_h2_get "")

target_sources(sample_h2_get
  PRIVATE
    ../common/sample_common.cpp
    sample_h2_get.cpp
)

target_link_libraries(sample_h2_get
  PRIVATE
    mh2c
)
