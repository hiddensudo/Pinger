cmake_minimum_required(VERSION 3.16)

project(Pinger)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${PROJECT_SOURCE_DIR}/ICMP
        ${PROJECT_SOURCE_DIR}/Pinger
        ${PROJECT_SOURCE_DIR}/SignalHandler
    )

add_executable(pinger pinger.cpp ICMP/ICMP.cpp Pinger/Pinger.cpp)
