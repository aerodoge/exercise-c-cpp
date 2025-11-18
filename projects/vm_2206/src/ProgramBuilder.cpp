#include "vm/ProgramBuilder.h"
#include <stdexcept>

// 添加指令（链式调用）
ProgramBuilder& ProgramBuilder::addInstruction(int instruction) {
    if (currentAddress_ >= VMContext::MEMORY_SIZE) {
        throw std::out_of_range("程序太大");
    }
    program_[currentAddress_++] = instruction;  // 写入并递增地址
    return *this;  // 返回自身，支持链式调用
}

// 在指定地址设置数据
ProgramBuilder& ProgramBuilder::setData(size_t address, int value) {
    if (address >= VMContext::MEMORY_SIZE) {
        throw std::out_of_range("地址越界");
    }
    program_[address] = value;
    return *this;  // 返回自身，支持链式调用
}

// 构建程序数组
std::array<int, VMContext::MEMORY_SIZE> ProgramBuilder::build() const {
    return program_;
}

// 重置构建器
void ProgramBuilder::reset() {
    program_.fill(0);
    currentAddress_ = 0;
}
