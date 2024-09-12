cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build are not supported")
endif()

set(PROJECT_NAME create-cpp-starter)

project(${PROJECT_NAME} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(${PROJECT_NAME} create_app.cpp utils.cpp utils.hpp CLI11.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/templates" DESTINATION ${CMAKE_INSTALL_PREFIX})

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
