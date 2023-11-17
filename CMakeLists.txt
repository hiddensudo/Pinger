cmake_minimum_required(VERSION 3.16)

project(Pinger)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${PROJECT_SOURCE_DIR}/lib
        ${PROJECT_SOURCE_DIR}/include
    )

add_executable(pinger pinger.cpp lib/ICMP.cpp lib/Pinger.cpp lib/RawSocket.cpp)
