#project(C)
cmake_minimum_required( VERSION 3.10 )

project(oglc VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(oglc ${SRC_FILES})
target_include_directories(oglc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/**/*)
