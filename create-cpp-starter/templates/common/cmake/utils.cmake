include(CheckCXXCompilerFlag)

# Check if the given flag is supported and append it 
# to output var if it is
# https://github.com/pytorch/pytorch/blob/main/cmake/public/utils.cmake#L473
function(append_cxx_flag_if_supported flag outputvar)
  string(TOUPPER "HAS${flag}" _FLAG_NAME)
  string(REGEX REPLACE "[=-]" "_" _FLAG_NAME "${_FLAG_NAME}")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    string(REGEX REPLACE "Wno-" "W" new_flag "${flag}")
  else()
    set(new_flag ${flag})
  endif()
  check_cxx_compiler_flag("${new_flag}" ${_FLAG_NAME})
  if(${_FLAG_NAME})
    string(APPEND ${outputvar} " ${flag}")
    set(${outputvar} "${${outputvar}}" PARENT_SCOPE)
  endif()
endfunction()

# add coverage compile options if config is debug
function(target_compile_coverage target)
  set(COVERAGE_${target}_COMPILE_FLAGS -O0 --coverage -g)
  append_cxx_flag_if_supported("-fprofile-abs-path" COVERAGE_${target}_COMPILE_FLAGS)
  target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:${COVERAGE_${target}_COMPILE_FLAGS}>)
  target_link_options(${target} PRIVATE $<$<CONFIG:Debug>:${COVERAGE_${target}_COMPILE_FLAGS}>)
endfunction()

# Add common development compiler options like -Werror to a target
function(target_handle_compile_options target)
  target_compile_options(
    ${target}
    PRIVATE
    $<$<CONFIG:Debug>:
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Werror -pedantic-errors -Wall -Wextra>
      $<$<CXX_COMPILER_ID:MSVC>: /Wall>>
  )
    
  get_target_property(FLAGS ${target} COMPILE_OPTIONS)
endfunction()


# create tests from a list of sources. SOURCES should be a list of files, 
# each file while map to an output executable of test test_${filestem}
# for example, tests/example.cpp will turn into the executable test_example.
# LIBRARIES should be the list of libraries to link with the test. 
# `gtest`, `gmock` and `gtest_main` are all  included automatically. 
# This list this can be empty.
#
# create_tests_from_files(
#   FILES test_dir/file1.test.cpp test_dir/file2.test.cpp     # the test filenames
#   SOURCES src_dir/src_file.hpp src_dir/src_file.cpp         # the sources to include when adding the executable
#   LIBRARIES randomlib                                       # the libraries to link to each target generated
# )
function(create_tests_from_files)
  cmake_parse_arguments(
    Args
    ""
    ""
    "FILES;SOURCES;LIBRARIES"
    ${ARGN}
  )
  foreach(test_src ${Args_FILES})
    cmake_path(GET test_src STEM test_filename)
    set(test_name "test_${test_filename}")
    add_executable(${test_name} "${test_src}" ${Args_SOURCES})
    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
    target_include_directories(${test_name} PRIVATE ${PROJECT_NAME})
    target_link_libraries(${test_name} ${Args_LIBRARIES} gtest gmock gtest_main)
    target_compile_coverage(${test_name})
  endforeach()
endfunction()