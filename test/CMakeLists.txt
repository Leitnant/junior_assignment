# /test/CMakeLists.txt

add_executable(DroneTest drone_test.cpp ${CMAKE_SOURCE_DIR}/src/new_udp_client.cpp ${CMAKE_SOURCE_DIR}/src/new_udp_server.cpp ${CMAKE_SOURCE_DIR}/src/drone.cpp ${CMAKE_SOURCE_DIR}/src/gcs.cpp)
target_link_libraries(DroneTest PRIVATE gtest_main SimpleUdp)
target_include_directories(DroneTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libs/mavlink/common ${CMAKE_SOURCE_DIR}/libs/simpleudp)

add_executable(GCSTest gcs_test.cpp ${CMAKE_SOURCE_DIR}/src/new_udp_client.cpp ${CMAKE_SOURCE_DIR}/src/new_udp_server.cpp ${CMAKE_SOURCE_DIR}/src/drone.cpp ${CMAKE_SOURCE_DIR}/src/gcs.cpp)
target_link_libraries(GCSTest PRIVATE gtest_main SimpleUdp)
target_include_directories(GCSTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libs/mavlink/common ${CMAKE_SOURCE_DIR}/libs/simpleudp)


include(GoogleTest)
gtest_discover_tests(DroneTest)
gtest_discover_tests(GCSTest)
