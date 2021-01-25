cmake_minimum_required(VERSION 3.19)

project(yanbt VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(nbtparse STATIC nbtparse.cpp)
add_library(nbttag STATIC nbttag.cpp)
add_library(nbtfile STATIC nbtfile.cpp)
add_library(nbtread STATIC nbtread.cpp)
target_link_libraries(nbtfile libboost_iostreams.so)
target_link_libraries(nbtparse nbttag nbtfile)
target_link_libraries(nbtread libboost_iostreams.so nbtparse)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)