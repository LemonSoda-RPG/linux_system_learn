cmake_minimum_required(VERSION 3.20)
project(server)
aux_source_directory(. DIR_SRCS)
include_directories(../../include)
add_executable(server ${DIR_SRCS})

# add_library(medialib medialib.c)
# add_library(mytbf mytbf.c)
# add_library(thr_list thr_list.c)
# add_library(thr_channel thr_channel.c)


