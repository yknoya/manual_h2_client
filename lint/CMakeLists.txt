find_program(clang_format_path clang-format)
find_program(cpplint_path cpplint)

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

if (cpplint_path)
  message(STATUS "Found cpplint: ${cpplint_path}")
  message(STATUS "Add 'lint' custom target")
  add_custom_target(lint
    COMMAND
      cpplint --filter=-build/c++11 --recursive ${PROJECT_SOURCE_DIR}/mh2c ${PROJECT_SOURCE_DIR}/sample ${PROJECT_SOURCE_DIR}/test
    WORKING_DIRECTORY
      ${PROJECT_SOURCE_DIR}
  )
endif()
