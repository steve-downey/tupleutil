include_directories(${gtest_SOURCE_DIR}/include)
set(test_sources
  test1.cpp
  tupleutil.t.cpp
)
add_executable(tests ${test_sources})
target_link_libraries(tests gtest_main tupleutil)
gtest_discover_tests(tests)
