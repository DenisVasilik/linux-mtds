cmake_minimum_required (VERSION 2.8)

project(mtds_example)

set(APP_NAME "mtds_example")

if("$ENV{ARCH}" STREQUAL "arm")
  SET(CMAKE_FIND_ROOT_PATH $ENV{SDKTARGETSYSROOT})
endif()

include_directories(
    ${PROJECT_BINARY_DIR}
    include/
)

link_directories(
    /home/zed-dev/wsDigilent/libmtds/build
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall") # Enable C++11 standard

file(GLOB SOURCES
    src/*.cpp
    src/*.c
)

add_executable(${APP_NAME} ${SOURCES})

target_link_libraries(${APP_NAME} mtds)