find_program(clang_format_path clang-format)
find_program(cpplint_path cpplint)

if (NOT WIN32)
  if (clang_format_path)
    message(STATUS "Found clang-format: ${clang_format_path}")
    message(STATUS "Add 'format' custom target")
    add_custom_target(format
      COMMAND
        find ${PROJECT_SOURCE_DIR} -regex \".*\.\(cpp\|h\|ipp\)\" | xargs clang-format -i
      WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    )
  endif()

  if (cpplint_path)
    message(STATUS "Found cpplint: ${cpplint_path}")
    message(STATUS "Add 'lint' custom target")
    add_custom_target(lint
      COMMAND
        cpplint --filter=-whitespace/line_length,-build/c++11 --recursive ${PROJECT_SOURCE_DIR}/mh2c ${PROJECT_SOURCE_DIR}/sample ${PROJECT_SOURCE_DIR}/test
      WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    )
  endif()
endif()
