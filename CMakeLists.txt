# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(HuffmanProject LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the executable name
set(EXECUTABLE_NAME huffman)

# Add source files
set(SOURCES
    main.cpp
    huffman_tree.cpp
)

# Add header files (optional but good practice)
set(HEADERS
    huffman_tree.h
    huffman_node.h
)

# Create the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})

# Enable warnings for better code quality
target_compile_options(${EXECUTABLE_NAME} PRIVATE
    -Wall
    -Wextra
    -pedantic
)
