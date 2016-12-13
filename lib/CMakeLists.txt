set(tuplefunctor_sources
  tuplefunctor.cpp)

add_library(tuplefunctor STATIC ${tuplefunctor_sources})
target_include_directories(tuplefunctor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
