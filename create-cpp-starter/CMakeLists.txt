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

set(CCS_TARGET_LOCATION_ROOT "${CMAKE_INSTALL_PREFIX}/.create-cpp-starter")

install(TARGETS ${PROJECT_NAME} DESTINATION "${CCS_TARGET_LOCATION_ROOT}/bin")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/templates" DESTINATION ${CCS_TARGET_LOCATION_ROOT})

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
