cmake_minimum_required(VERSION 3.6)
message("当前工作目录：${CMAKE_CURRENT_SOURCE_DIR}")
project(mysem)

add_library(semlib  src/mysem.c)
# include_directories(include)
# add_executable(mysem main.c)
# target_link_libraries(mysem semlib)


add_executable(mysem main.c)
target_include_directories(mysem PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(semlib PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/include)



target_link_libraries(mysem semlib) 
add_executable(stdsem main_sem.c)

