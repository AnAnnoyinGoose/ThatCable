cmake_minimum_required(VERSION 3.23)
project(ThatCable)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses")

add_executable(ThatCableServer Server/server.cpp Server/headers/server.h)
add_executable(ThatCableClient Client/client.cpp Client/headers/client.h Client/GUI.cpp Client/headers/GUI.h Client/main.cpp Client/headers/main.h)
