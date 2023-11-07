cmake_minimum_required(VERSION 3.16)

project(Pinger)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/ICMP)

add_executable(server ICMP/ICMP.cpp server.cpp)
add_executable(client ICMP/ICMP.cpp main.cpp)

add_custom_target(server_run COMMAND server)
add_custom_target(client_run COMMAND client)

add_custom_target(clear COMMAND rm -rf client server )