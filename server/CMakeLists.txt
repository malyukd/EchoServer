cmake_minimum_required(VERSION 3.10)
project(ServerClient CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server
    server.cpp
    TCPacceptor.cpp
    TCPacceptor.h
    StringHandler.cpp
    StringHandler.h
)

target_link_libraries(server PRIVATE stream)