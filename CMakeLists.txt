cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(chunkstream VERSION 0.1 LANGUAGES CXX)

find_package(benchmark REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bench benchmark.cpp)
target_link_libraries(bench benchmark::benchmark)