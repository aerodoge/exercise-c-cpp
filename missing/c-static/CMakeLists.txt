cmake_minimum_required(VERSION 4.0)
project(c_static C)

set(CMAKE_C_STANDARD 11)

add_executable(static_local_var static_local_var.c
        static_global_var_func.c)
