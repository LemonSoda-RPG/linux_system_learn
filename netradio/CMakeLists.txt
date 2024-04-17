cmake_minimum_required(VERSION 3.6)
project(netradio)

add_subdirectory(src/client)
add_subdirectory(src/server)
target_include_directories(client PRIVATE include)
target_include_directories(server PRIVATE include)

