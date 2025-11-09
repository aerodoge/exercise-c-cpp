cmake_minimum_required(VERSION 3.20)
project(tortoise_hare_2206)

# 设置C++20标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加include目录
include_directories(include)

# 收集所有源文件
set(SOURCES
    src/RandomGenerator.cpp
    src/Racer.cpp
    src/Tortoise.cpp
    src/Hare.cpp
    main.cpp
)

# 收集所有头文件（用于IDE显示）
set(HEADERS
    include/Constants.h
    include/RacerConcept.h
    include/RandomGenerator.h
    include/Racer.h
    include/Tortoise.h
    include/Hare.h
    include/Race.h
)

# 创建可执行文件
add_executable(tortoise_hare_2206 ${SOURCES} ${HEADERS})

# 编译选项
if(MSVC)
    target_compile_options(tortoise_hare_2206 PRIVATE /W4)
else()
    target_compile_options(tortoise_hare_2206 PRIVATE -Wall -Wextra -Wpedantic)
endif()
