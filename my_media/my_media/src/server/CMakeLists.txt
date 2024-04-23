cmake_minimum_required(VERSION 3.10)
project(server)
SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

aux_source_directory(. DIR_SRCS)
include_directories(../../include)
add_executable(server ${DIR_SRCS})
target_link_libraries(server pthread)

# add_library(medialib medialib.c)
# add_library(mytbf mytbf.c)
# add_library(thr_list thr_list.c)
# add_library(thr_channel thr_channel.c)


