# /test/CMakeLists.txt

add_executable(GreetingTest test_main.cpp)

target_link_libraries(GreetingTest PRIVATE src_lib gtest_main lnl)

target_include_directories(GreetingTest PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(GreetingTest)
