cmake_minimum_required(VERSION 3.21)
project(huffman-lib)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_CPP *.cpp)
file(GLOB_RECURSE LIB_H *.h)
add_library(huffman-lib SHARED ${LIB_CPP})
set_target_properties(huffman-lib PROPERTIES VERSION 1.0.0)
set_target_properties(huffman-lib PROPERTIES PUBLIC_HEADER "${LIB_H}")
include(GNUInstallDirs)
install(TARGETS huffman-lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
