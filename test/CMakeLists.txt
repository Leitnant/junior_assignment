enable_testing()

add_executable(Main_test main_test.cpp)

target_link_libraries(Main_test PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(Main_test)
