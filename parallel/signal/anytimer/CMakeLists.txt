cmake_minimum_required(VERSION 3.6)

project(anytimer)
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(Demo ${DIR_SRCS})

