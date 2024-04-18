
aux_source_directory(. DIR_SRCS)
add_library(mytbflib mytbf.c mytbf.h)
add_executable(server ${DIR_SRCS})
