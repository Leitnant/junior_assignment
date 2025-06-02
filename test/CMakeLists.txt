# /test/CMakeLists.txt

add_executable(NetTest test_net_manager.cpp ${CMAKE_SOURCE_DIR}/src/udp_client.cpp ${CMAKE_SOURCE_DIR}/src/udp_server.cpp ${CMAKE_SOURCE_DIR}/src/listener_client.cpp ${CMAKE_SOURCE_DIR}/src/listener_server.cpp ${CMAKE_SOURCE_DIR}/src/gcs.cpp ${CMAKE_SOURCE_DIR}/src/drone.cpp )

target_link_libraries(NetTest PRIVATE gtest_main lnl)

target_include_directories(NetTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libs/mavlink/common)

include(GoogleTest)
gtest_discover_tests(NetTest)
