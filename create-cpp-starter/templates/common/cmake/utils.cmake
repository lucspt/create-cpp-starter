include(CheckCXXCompilerFlag)

####################################################################
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

####################################################################
# add coverage compile options if config is debug
function(target_compile_coverage target)
  set(COVERAGE_${target}_COMPILE_FLAGS "--coverage")
  append_cxx_flag_if_supported("-fprofile-abs-path" COVERAGE_${target}_COMPILE_FLAGS)
  target_compile_options(${target} PRIVATE "$<$<CONFIG:Debug>:${COVERAGE_${target}_COMPILE_FLAGS}>")
  target_link_options(${target} PRIVATE "$<$<CONFIG:Debug>:${COVERAGE_${target}_COMPILE_FLAGS}>")
endfunction()

####################################################################
# Add common development compile options like -Werror to a target
function(target_compile_debug target)
  target_compile_options(
    ${target}
    PRIVATE
    $<$<CONFIG:Debug>:
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Werror -pedantic-errors -Wall -Wextra>
      $<$<CXX_COMPILER_ID:MSVC>: /Wall>>
  )
    
  get_target_property(FLAGS ${target} COMPILE_OPTIONS)
endfunction()
