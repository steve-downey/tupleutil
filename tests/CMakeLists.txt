include_directories(${gtest_SOURCE_DIR}/include)
set(test_sources
  test1.cpp
  tuplefunctor.t.cpp
)
add_executable(tests ${test_sources})
target_link_libraries(tests gtest_main tuplefunctor)
add_test(NAME Test COMMAND tests)
