#pragma once

#include "OpCode.h"
#include "IInstruction.h"
#include <unordered_map>
#include <memory>
#include <optional>

/**
 * @file InstructionFactory.h
 * @brief 指令工厂类定义
 *
 * 采用 Factory 模式 + Singleton 模式
 * 负责创建和管理所有指令对象
 */

/**
 * @class InstructionFactory
 * @brief 指令工厂 - Factory + Singleton 模式
 *
 * 单例模式确保全局只有一个工厂实例
 * 工厂模式负责创建所有指令对象
 *
 * 设计特点：
 * - 使用 std::unique_ptr 管理指令对象的生命周期
 * - 使用 std::optional 安全返回可能不存在的指令
 * - 禁用拷贝和移动，确保单例唯一性
 */
class InstructionFactory {
private:
    // 指令映射表：操作码 -> 指令对象
    std::unordered_map<OpCode, std::unique_ptr<IInstruction>> instructions_;

    /**
     * @brief 私有构造函数（Singleton 模式）
     *
     * 在构造时创建所有指令对象
     */
    InstructionFactory();

public:
    /**
     * @brief 获取工厂单例实例
     *
     * @return 工厂实例的引用
     */
    static InstructionFactory& getInstance();

    // 删除拷贝和移动构造/赋值函数，确保单例唯一性
    InstructionFactory(const InstructionFactory&) = delete;
    InstructionFactory& operator=(const InstructionFactory&) = delete;
    InstructionFactory(InstructionFactory&&) = delete;
    InstructionFactory& operator=(InstructionFactory&&) = delete;

    /**
     * @brief 根据操作码获取对应的指令对象
     *
     * @param opcode 操作码
     * @return std::optional<IInstruction*> 指令对象指针，如果操作码无效则返回 nullopt
     */
    [[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const;
};
