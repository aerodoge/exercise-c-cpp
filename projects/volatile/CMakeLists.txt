cmake_minimum_required(VERSION 3.2)
project(volatile C)
set(CMAKE_C_STANDARD 11)

#option(ENABLE_OPTIMIZATION "Enable compiler optimizations" OFF)
#if(ENABLE_OPTIMIZATION)
#    if(CMAKE_BUILD_TYPE STREQUAL "")
#        set(CMAKE_BUILD_TYPE "Release")
#    endif()
#    message(STATUS "Optimization enabled")
#else()
#    if(CMAKE_BUILD_TYPE STREQUAL "")
#        set(CMAKE_BUILD_TYPE "Debug")
#    endif()
#    message(STATUS "Optimization disabled")
#endif()

# 显示可用的优化选项
message(STATUS "Available optimization levels: O0, O1, O2, O3, Os, Ofast")

# 优化级别选择
set(OPT_LEVEL "O2" CACHE STRING "Optimization level (O0=no opt, O1=basic, O2=standard, O3=aggressive, Os=size, Ofast=fast math)")
set_property(CACHE OPT_LEVEL PROPERTY STRINGS "O0;O1;O2;O3;Os;Ofast")

# 额外选项
option(ENABLE_DEBUG_SYMBOLS "Include debug symbols even in optimized builds" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

add_executable(${PROJECT_NAME} volatile.c)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

# 设置优化级别
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # 主要优化标志
    target_compile_options(${PROJECT_NAME} PRIVATE "-${OPT_LEVEL}")

    # 调试符号
    if(ENABLE_DEBUG_SYMBOLS)
        target_compile_options(${PROJECT_NAME} PRIVATE "-g")
    endif()

    # 警告
    if(ENABLE_WARNINGS)
        target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")
    endif()

elseif(MSVC)
    # MSVC 配置
    if(OPT_LEVEL STREQUAL "O0")
        target_compile_options(${PROJECT_NAME} PRIVATE "/Od")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE "/O2")
    endif()

    if(ENABLE_DEBUG_SYMBOLS)
        target_compile_options(${PROJECT_NAME} PRIVATE "/Zi")
    endif()

    if(ENABLE_WARNINGS)
        target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
    endif()
endif()

# 显示配置摘要
message(STATUS "=== Build Configuration ===")
message(STATUS "Optimization level: ${OPT_LEVEL}")
message(STATUS "Debug symbols: ${ENABLE_DEBUG_SYMBOLS}")
message(STATUS "Warnings: ${ENABLE_WARNINGS}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID}")



