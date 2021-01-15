cmake_minimum_required(VERSION 3.19)

project(yanbt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(nbtread nbtread.cpp)
target_link_libraries(nbtread libboost_iostreams.so)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)