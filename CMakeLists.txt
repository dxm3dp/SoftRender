cmake_minimum_required(VERSION 3.20)
project(SoftRender)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/geometry/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/model/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/pipeline/*.cpp
    )

add_executable(SoftRender ${SRC})



