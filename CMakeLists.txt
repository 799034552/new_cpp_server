cmake_minimum_required(VERSION 3.10)
project(test_cpp_server)

add_executable(test_cpp_server)
file(GLOB src main.cpp)
target_sources(test_cpp_server PUBLIC ${src})
target_link_libraries(test_cpp_server PUBLIC cpp_server)
subdirs(cpp_server)
