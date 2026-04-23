find_program(clang_format_path clang-format)

if (clang_format_path AND NOT WIN32)
  message(STATUS "Found clang-format: ${clang_format_path}")
  message(STATUS "Add 'format' custom target")
  add_custom_target(format
    COMMAND
      find ${PROJECT_SOURCE_DIR} -type f -name "*.cpp" -o -name "*.h" -o -name "*.ipp" | grep -v ${CMAKE_BINARY_DIR} | xargs clang-format -i
    WORKING_DIRECTORY
      ${PROJECT_SOURCE_DIR}
  )
endif()
