add_library(fractalelib
  color.cpp
  color.h

  draw_parallel.cpp
  draw_parallel.h

  draw_serial.cpp
  draw_serial.h

  modules.cpp
  modules.h
)
target_link_libraries(fractalelib PUBLIC Threads::Threads)
target_include_directories(fractalelib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_executable(fractale fractale.cpp)
target_link_libraries(fractale fractalelib)

add_executable(fractale_benchmark fractale_benchmark.cpp)
target_link_libraries(fractale_benchmark fractalelib)
