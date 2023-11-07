cmake_minimum_required(VERSION 3.16)

project(Pinger)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/ICMP)

add_executable(client main.cpp)
