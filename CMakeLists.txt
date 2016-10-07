cmake_minimum_required(VERSION 3.4)
project(data_encoding_p01)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Node.hpp HuffmanTree.hpp NodeData.hpp FGKTree.hpp BitWriter.hpp HuffmanEncoder.hpp HuffmanCoder.hpp HuffmanDecoder.hpp Optional.hpp BitReader.hpp)
add_executable(huff ${SOURCE_FILES})
