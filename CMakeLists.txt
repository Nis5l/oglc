#project(C)
cmake_minimum_required(VERSION 3.10)

project(oglc VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# main
file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/**/*)

#opengl
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# glfw
set(GLFW_DIR lib/glfw-3.3.8)
add_subdirectory(${GLFW_DIR})
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
target_link_libraries(${PROJECT_NAME} glfw)

# glad
set(GLAD_DIR lib/glad)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} glad)

# stb_image
set(STB_DIR lib/stb_image)
add_subdirectory(${STB_DIR})
target_link_libraries(${PROJECT_NAME} stb_image)
