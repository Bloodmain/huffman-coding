cmake_minimum_required(VERSION 3.21)
project(huffman-unit-tests)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
add_definitions(-DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(tests tree_tests.cpp wrappers_tests.cpp test_utils.cpp io_tests.cpp)
target_link_libraries(tests GTest::gtest GTest::gtest_main)
