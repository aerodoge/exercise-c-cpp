#pragma once

#include <string>
#include "VMContext.h"

/**
 * @file IInstruction.h
 * @brief 指令接口定义
 *
 * 采用 Command 模式和 Strategy 模式
 * 每个指令都是一个独立的命令对象
 */

/**
 * @class IInstruction
 * @brief 指令接口（抽象基类）
 *
 * 所有具体指令类都必须实现此接口
 * 设计模式：
 * - Command 模式：将指令封装为对象
 * - Strategy 模式：不同的指令执行策略
 */
class IInstruction {
public:
    virtual ~IInstruction() = default;

    /**
     * @brief 执行指令
     *
     * @param context 虚拟机上下文（包含寄存器和内存）
     * @param operand 操作数（指令的参数，通常是内存地址）
     */
    virtual void execute(VMContext& context, int operand) = 0;

    /**
     * @brief 获取指令名称
     *
     * @return 指令的名称（如 "ADD", "LOAD" 等）
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief 检查指令是否改变程序计数器（PC）
     *
     * @return true 如果是跳转指令，false 否则
     *
     * 跳转指令（BRANCH, BRANCHNEG, BRANCHZERO, HALT）会改变 PC
     * 其他指令执行后 PC 自动递增
     */
    [[nodiscard]] virtual bool changesPC() const { return false; }
};
