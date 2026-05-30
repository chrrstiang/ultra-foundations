cmake_minimum_required(VERSION 3.14)
project(ultra-foundations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(ultra-foundations
  src/cpp/main/main.cpp
  src/cpp/main/image.cpp
  src/cpp/main/pgm_parser.cpp
  src/cpp/main/pgm_writer.cpp
  src/cpp/main/filter_pipeline.cpp
  src/cpp/main/filters/filter.cpp
  src/cpp/main/filters/gaussian.cpp
  src/cpp/main/filters/sobel_edge_detection.cpp
  src/cpp/main/filters/intensity_normalization.cpp
)

target_include_directories(ultra-foundations PRIVATE src/cpp/main)

enable_testing()
add_subdirectory(src/cpp/tests)