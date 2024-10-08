cmake_minimum_required(VERSION 3.1)
project(JunTestTask)

set(CMAKE_CXX_STANDARD 17)

find_package(jsoncpp REQUIRED)

file(GLOB_RECURSE SOURCES src/*)

add_executable(JunTestTask ${SOURCES})

target_link_libraries(JunTestTask jsoncpp_lib)
target_include_directories(JunTestTask PRIVATE src)