cmake_minimum_required(VERSION 3.20)
project(vm_2206)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 include 目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 收集所有源文件
set(SOURCES
    src/main.cpp
    src/Instructions.cpp
    src/InstructionFactory.cpp
    src/VirtualMachine.cpp
    src/ProgramBuilder.cpp
)

# 收集所有头文件（可选，用于 IDE 显示）
set(HEADERS
    include/vm/OpCode.h
    include/vm/VMContext.h
    include/vm/IInstruction.h
    include/vm/Instructions.h
    include/vm/InstructionFactory.h
    include/vm/VirtualMachine.h
    include/vm/ProgramBuilder.h
)

# 创建可执行文件
add_executable(vm_2206 ${SOURCES} ${HEADERS})
