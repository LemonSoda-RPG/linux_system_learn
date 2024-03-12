cmake_minimum_required(VERSION 3.6)
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
project(keybox)
aux_source_directory(. DIR_SRCS)

# 添加 math 子目录
add_subdirectory(tbf)

# 指定生成目标
add_executable(mytbf ${DIR_SRCS})

# 添加链接库
target_link_libraries(mytbf  tbffunctions)