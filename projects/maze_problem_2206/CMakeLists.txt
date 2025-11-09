cmake_minimum_required(VERSION 3.20)
project(maze_problem_2206)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR})

# 添加可执行文件
add_executable(maze_problem_2206 main.cpp)
