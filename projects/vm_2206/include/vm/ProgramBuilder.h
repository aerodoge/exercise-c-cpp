#pragma once

#include "VMContext.h"
#include <array>

/**
 * @file ProgramBuilder.h
 * @brief 程序构建器
 *
 * 采用 Builder 模式，提供流式 API 构建程序
 */

/**
 * @class ProgramBuilder
 * @brief 程序构建器 - Builder 模式
 *
 * 提供链式调用（Fluent API）方便地构建虚拟机程序
 *
 * 使用示例：
 * @code
 * auto program = ProgramBuilder()
 *     .addInstruction(+1007)  // READ A
 *     .addInstruction(+1008)  // READ B
 *     .addInstruction(+2007)  // LOAD A
 *     .addInstruction(+3008)  // ADD B
 *     .addInstruction(+4300)  // HALT
 *     .build();
 * @endcode
 */
class ProgramBuilder {
private:
    std::array<int, VMContext::MEMORY_SIZE> program_{};  // 程序数组
    size_t currentAddress_{0};                            // 当前写入地址

public:
    /**
     * @brief 添加一条指令（自动递增地址）
     *
     * @param instruction 指令的完整编码（操作码 + 操作数）
     * @return 自身引用，支持链式调用
     * @throws std::out_of_range 如果程序太大
     */
    ProgramBuilder& addInstruction(int instruction);

    /**
     * @brief 在指定地址设置数据
     *
     * @param address 内存地址
     * @param value 数据值
     * @return 自身引用，支持链式调用
     * @throws std::out_of_range 如果地址越界
     */
    ProgramBuilder& setData(size_t address, int value);

    /**
     * @brief 构建并返回程序数组
     *
     * @return 完整的程序数组
     */
    [[nodiscard]] std::array<int, VMContext::MEMORY_SIZE> build() const;

    /**
     * @brief 重置构建器
     *
     * 清空程序数组，重置当前地址
     */
    void reset();
};
