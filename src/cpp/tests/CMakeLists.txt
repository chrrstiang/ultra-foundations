add_executable(ring_buffer_tests ring_buffer_tests.cpp)

target_include_directories(ring_buffer_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(ring_buffer_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ring_buffer_tests)

add_executable(pgm_parser_tests
  pgm_parser_tests.cpp
  ../main/pgm_parser.cpp
  ../main/image.cpp
)

target_include_directories(pgm_parser_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(pgm_parser_tests
  GTest::gtest_main
)

gtest_discover_tests(pgm_parser_tests)

add_executable(intensity_normalization_tests
  intensity_normalization_tests.cpp
  ../main/filters/intensity_normalization.cpp
  ../main/filters/filter.cpp
  ../main/image.cpp
)

target_include_directories(intensity_normalization_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(intensity_normalization_tests
  GTest::gtest_main
)

gtest_discover_tests(intensity_normalization_tests)

add_executable(gaussian_tests
  gaussian_tests.cpp
  ../main/filters/gaussian.cpp
  ../main/filters/filter.cpp
  ../main/image.cpp
)

target_include_directories(gaussian_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(gaussian_tests
  GTest::gtest_main
)

gtest_discover_tests(gaussian_tests)

add_executable(sobel_edge_detection_tests
  sobel_edge_detection_tests.cpp
  ../main/filters/sobel_edge_detection.cpp
  ../main/filters/filter.cpp
  ../main/image.cpp
)

target_include_directories(sobel_edge_detection_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(sobel_edge_detection_tests
  GTest::gtest_main
)

gtest_discover_tests(sobel_edge_detection_tests)

add_executable(pgm_writer_tests
  pgm_writer_tests.cpp
  ../main/pgm_writer.cpp
  ../main/pgm_parser.cpp
  ../main/image.cpp
)

target_include_directories(pgm_writer_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(pgm_writer_tests
  GTest::gtest_main
)

gtest_discover_tests(pgm_writer_tests)

add_executable(image_tests
  image_tests.cpp
  ../main/image.cpp
)

target_include_directories(image_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(image_tests
  GTest::gtest_main
)

gtest_discover_tests(image_tests)

add_executable(parallelizer_tests
  parallelizer_tests.cpp
  ../main/parallelizer.cpp
  ../main/filters/gaussian.cpp
  ../main/filters/sobel_edge_detection.cpp
  ../main/filters/intensity_normalization.cpp
  ../main/filters/filter.cpp
  ../main/image.cpp
)

target_include_directories(parallelizer_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(parallelizer_tests
  GTest::gtest_main
)

gtest_discover_tests(parallelizer_tests)
