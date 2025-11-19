#pragma once

#include "InstructionFactory.h"
#include "VMContext.h"

#include <array>

/**
 * @file VirtualMachine.h
 * @brief 虚拟机主控制器
 *
 * 负责虚拟机的执行流程控制
 */

/**
 * @class VirtualMachine
 * @brief 虚拟机类 - 主控制器
 *
 * 虚拟机的核心类，负责：
 * - 加载程序到内存
 * - 执行指令（取指、解码、执行）
 * - 管理虚拟机状态
 * - 提供状态查询接口
 *
 * 执行流程：
 * 1. 取指（Fetch）：从内存读取指令
 * 2. 解码（Decode）：分离操作码和操作数
 * 3. 执行（Execute）：调用对应的指令对象
 * 4. 更新 PC：跳转到下一条指令
 */
class VirtualMachine
{
private:
    VMContext context_;                 // 虚拟机上下文（寄存器和内存）
    const InstructionFactory& factory_; // 指令工厂引用

    /**
     * @brief 执行单条指令（取指-解码-执行循环）
     *
     * 实现经典的 Fetch-Decode-Execute 循环
     */
    void executeSingleInstruction();

public:
    /**
     * @brief 构造函数
     *
     * 初始化虚拟机，获取指令工厂单例
     */
    VirtualMachine();

    /**
     * @brief 加载程序到内存
     *
     * @param program 程序数组（包含指令和数据）
     */
    void loadProgram(const std::array<int, VMContext::MEMORY_SIZE>& program);

    /**
     * @brief 执行程序
     *
     * 从地址 0 开始执行，直到遇到 HALT 指令或发生错误
     */
    void execute();

    // ==================== 状态查询接口 ====================

    /**
     * @brief 转储内存内容（用于调试）
     *
     * 以格式化方式显示所有内存单元的值
     */
    void dumpMemory() const;

    /**
     * @brief 转储寄存器状态（用于调试）
     *
     * 显示累加器、指令计数器、指令寄存器的值
     */
    void dumpRegisters() const;
};
