# setup
cmake_minimum_required(VERSION 2.8)
option(fssim_tests "Include Tests." ON)

set(PROJECT_NAME fssim)
project(${PROJECT_NAME} C)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
find_package(Readline REQUIRED)

# src
add_subdirectory(src/)

# global vars
SET(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
  CACHE INTERNAL
  "${PROJECT_NAME} Include Directories"
  FORCE
)

SET(${PROJECT_NAME}_LIBRARIES ${LIBRARIES}
  CACHE INTERNAL
  "${PROJECT_NAME} Link Libraries"
  FORCE
)

# test
if (${fssim_tests})
  add_subdirectory(tests/)
endif()


