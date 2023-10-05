add_executable(test_fractale test_fractale.cpp)
target_link_libraries(test_fractale PRIVATE fractalelib)

add_test(NAME test_fractale COMMAND test_fractale)
