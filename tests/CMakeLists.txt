add_executable(ring_buffer_tests ring_buffer_tests.cpp)

target_include_directories(ring_buffer_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(ring_buffer_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ring_buffer_tests)