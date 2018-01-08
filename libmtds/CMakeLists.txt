cmake_minimum_required (VERSION 2.8)

project(libmtds)

set(LIBRARY_NAME "mtds")

if("$ENV{ARCH}" STREQUAL "arm")
  SET(CMAKE_FIND_ROOT_PATH $ENV{SDKTARGETSYSROOT})
endif()

include_directories(
    ${PROJECT_BINARY_DIR}
    include/
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall") # Enable C++11 standard

file(GLOB SOURCES
    src/*.cpp
    src/*.c
)

add_library (${LIBRARY_NAME} SHARED
    src/mtds.cpp
    src/MtdsCore.cpp
    src/MtdsFs.cpp
    src/MtdsGdi.cpp
    src/MtdsHal.cpp
    src/MtdsUtil.cpp
    src/MtdsWin.cpp
    src/MyDisp.cpp
)

target_include_directories (${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
