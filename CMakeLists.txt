project(ir-sea)
cmake_minimum_required(VERSION 3.10)
enable_testing()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-O2 -Wall -Wextra -Wpedantic)
endif()

add_subdirectory(network)
add_subdirectory(hamming)
