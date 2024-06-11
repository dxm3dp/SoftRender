cmake_minimum_required(VERSION 3.20)
project(SoftRender)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/geometry/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/model/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/pipeline/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/shader/*.cpp
    )

add_executable(SoftRender ${SRC})



