cmake_minimum_required(VERSION 3.10)
project(file_zipper)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_library(huffman STATIC src/Huffman.cpp src/BitWriter.cpp src/BitReader.cpp)
add_executable(zipper src/main.cpp)
target_link_libraries(zipper huffman)

enable_testing()
add_executable(test_huffman tests/test_huffman.cpp)
target_link_libraries(test_huffman huffman)
add_test(NAME huffman_test COMMAND test_huffman)