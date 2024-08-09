cmake_minimum_required(VERSION 3.10)
project(CompressionProject C)

set(CMAKE_C_STANDARD 11)

include_directories(include common)

add_executable(CompressionProject
        src/main.c
        src/compress.c
        src/decompress.c
        common/common.c
)

