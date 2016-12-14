set(tupleutil_sources
  tupleutil.cpp)

add_library(tupleutil STATIC ${tupleutil_sources})
target_include_directories(tupleutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
