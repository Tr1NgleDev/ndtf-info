cmake_minimum_required(VERSION 3.10)
project(ndtf-info)

add_subdirectory("libs/ndtf")

set(CMAKE_CXX_STANDARD 20)

add_executable(ndtf-info main.cpp)

target_link_libraries(ndtf-info PRIVATE ndtf)
