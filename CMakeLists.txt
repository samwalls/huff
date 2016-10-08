cmake_minimum_required(VERSION 2.8)
project(data_encoding_p01)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Node.hpp src/HuffmanTree.hpp src/NodeData.hpp src/FGKTree.hpp src/BitWriter.hpp src/HuffmanEncoder.hpp src/HuffmanCoder.hpp src/HuffmanDecoder.hpp src/Optional.hpp src/BitReader.hpp)
add_executable(huff ${SOURCE_FILES})
