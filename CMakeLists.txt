cmake_minimum_required(VERSION 3.6)
project(data_encoding_p01)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp node.h)
add_executable(data_encoding_p01 ${SOURCE_FILES})