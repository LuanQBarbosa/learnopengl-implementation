# cmake version to be used
cmake_minimum_required( VERSION 3.0 )

# project name
project( leanopengl-implementation )

#flags

#files

#include
include_directories( ./include ./src )

# target
add_executable( binary ./src/main.cpp ./src/glad.c ./src/shader.cpp )

# external libraries
target_link_libraries( binary -ldl -lglfw )