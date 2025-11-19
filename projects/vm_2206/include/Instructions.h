#pragma once

#include "IInstruction.h"
#include "VMContext.h"

#include <string>

/**
 * @file Instructions.h
 * @brief 所有具体指令类的声明
 *
 * 包含虚拟机支持的所有指令实现
 * 采用 Template Method 模式和继承层次结构
 */

// ==================== I/O 指令 ====================

/**
 * @class ReadInstruction
 * @brief READ指令 - 从终端读取输入
 *
 * 从标准输入读取一个整数，存储到指定内存地址
 */
class ReadInstruction : public IInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class WriteInstruction
 * @brief WRITE指令 - 向终端输出
 *
 * 将指定内存地址的值输出到标准输出
 */
class WriteInstruction : public IInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

// ==================== 加载/存储指令 ====================

/**
 * @class LoadInstruction
 * @brief LOAD 指令 - 加载到累加器
 *
 * 将指定内存地址的值加载到累加器
 */
class LoadInstruction : public IInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class StoreInstruction
 * @brief STORE 指令 - 从累加器存储
 *
 * 将累加器的值存储到指定内存地址
 */
class StoreInstruction : public IInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

// ==================== 算术指令 ====================

/**
 * @class ArithmeticInstruction
 * @brief 算术指令基类 - Template Method 模式
 *
 * 定义算术运算的通用流程：
 * 1. 从内存读取操作数
 * 2. 执行具体运算（由子类实现）
 * 3. 将结果存回累加器
 */
class ArithmeticInstruction : public IInstruction
{
protected:
    /**
     * @brief 执行具体的算术运算（由子类实现）
     *
     * @param accumulator 累加器的当前值
     * @param operand 内存中的操作数
     * @return 运算结果
     */
    virtual int compute(int accumulator, int operand) const = 0;

public:
    void execute(VMContext& context, int operand) override;
};

/**
 * @class AddInstruction
 * @brief ADD 指令 - 加法运算
 *
 * 累加器 = 累加器 + 内存[地址]
 */
class AddInstruction : public ArithmeticInstruction
{
protected:
    int compute(int accumulator, int operand) const override;

public:
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class SubtractInstruction
 * @brief SUB指令 - 减法运算
 *
 * 累加器 = 累加器 - 内存[地址]
 */
class SubtractInstruction : public ArithmeticInstruction
{
protected:
    int compute(int accumulator, int operand) const override;

public:
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class MultiplyInstruction
 * @brief MUL指令 - 乘法运算
 *
 * 累加器 = 累加器 * 内存[地址]
 */
class MultiplyInstruction : public ArithmeticInstruction
{
protected:
    int compute(int accumulator, int operand) const override;

public:
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class DivideInstruction
 * @brief DIV指令 - 除法运算
 *
 * 累加器 = 累加器 / 内存[地址]
 * 注意：除数为零时抛出异常
 */
class DivideInstruction : public ArithmeticInstruction
{
protected:
    int compute(int accumulator, int operand) const override;

public:
    [[nodiscard]] std::string getName() const override;
};

// ==================== 控制流指令 ====================

/**
 * @class ControlFlowInstruction
 * @brief 控制流指令基类
 *
 * 所有控制流指令（跳转、停机）都会改变程序计数器
 */
class ControlFlowInstruction : public IInstruction
{
public:
    [[nodiscard]] bool changesPC() const override;
};

/**
 * @class JumpInstruction
 * @brief JMP指令 - 无条件跳转
 *
 * 无条件跳转到指定内存地址
 */
class JumpInstruction : public ControlFlowInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class JumpNegInstruction
 * @brief JMPNEG指令 - 负数条件跳转
 *
 * 如果累加器的值为负数，则跳转到指定地址
 * 否则继续执行下一条指令
 */
class JumpNegInstruction : public ControlFlowInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class JumpZeroInstruction
 * @brief JMPZERO 指令 - 零条件跳转
 *
 * 如果累加器的值为零，则跳转到指定地址
 * 否则继续执行下一条指令
 */
class JumpZeroInstruction : public ControlFlowInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};

/**
 * @class HaltInstruction
 * @brief HALT指令 - 停机
 *
 * 停止虚拟机运行，程序结束
 */
class HaltInstruction : public ControlFlowInstruction
{
public:
    void execute(VMContext& context, int operand) override;
    [[nodiscard]] std::string getName() const override;
};
