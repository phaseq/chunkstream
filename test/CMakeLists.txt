cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(chunkstream VERSION 0.1 LANGUAGES CXX)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests test.cpp)
target_include_directories(tests PUBLIC .. PRIVATE GTest::GTest)
target_link_libraries(tests GTest::GTest)