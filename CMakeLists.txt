cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(OpenGLCourse)

add_executable(main src/main.cpp)
