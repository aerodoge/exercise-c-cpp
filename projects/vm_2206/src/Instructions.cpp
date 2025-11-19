#include "../include/Instructions.h"

#include <iostream>
#include <stdexcept>

// ==================== I/O 指令实现 ====================

// READ 指令：从标准输入读取一个整数
void ReadInstruction::execute(VMContext& context, int operand)
{
    std::cout << "请输入一个整数: ";
    int value;
    std::cin >> value;
    context.setMemory(operand, value); // 将读取的值存入指定内存地址
}

std::string ReadInstruction::getName() const
{
    return "READ";
}

// WRITE 指令：将内存值输出到标准输出
void WriteInstruction::execute(VMContext& context, int operand)
{
    std::cout << context.getMemory(operand) << std::endl;
}

std::string WriteInstruction::getName() const
{
    return "WRITE";
}

// ==================== 加载/存储指令实现 ====================

// LOAD 指令：将内存值加载到累加器
void LoadInstruction::execute(VMContext& context, int operand)
{
    context.accumulator = context.getMemory(operand);
}

std::string LoadInstruction::getName() const
{
    return "LOAD";
}

// STORE 指令：将累加器值存储到内存
void StoreInstruction::execute(VMContext& context, int operand)
{
    context.setMemory(operand, context.accumulator);
}

std::string StoreInstruction::getName() const
{
    return "STORE";
}

// ==================== 算术指令实现 ====================

// 定义通用流程：读取内存 -> 计算 -> 写回累加器
void ArithmeticInstruction::execute(VMContext& context, int operand)
{
    int value = context.getMemory(operand);                    // 从内存读取操作数
    context.accumulator = compute(context.accumulator, value); // 执行运算
}

// ADD 指令：加法运算
int AddInstruction::compute(const int accumulator, const int operand) const
{
    return accumulator + operand;
}

std::string AddInstruction::getName() const
{
    return "ADD";
}

// SUBTRACT 指令：减法运算
int SubtractInstruction::compute(const int accumulator, const int operand) const
{
    return accumulator - operand;
}

std::string SubtractInstruction::getName() const
{
    return "SUBTRACT";
}

// MULTIPLY 指令：乘法运算
int MultiplyInstruction::compute(const int accumulator, const int operand) const
{
    return accumulator * operand;
}

std::string MultiplyInstruction::getName() const
{
    return "MULTIPLY";
}

// DIVIDE 指令：除法运算（带除零检查）
int DivideInstruction::compute(const int accumulator, const int operand) const
{
    if (operand == 0)
    {
        throw std::runtime_error("除数为零");
    }
    return accumulator / operand;
}

std::string DivideInstruction::getName() const
{
    return "DIVIDE";
}

// ==================== 控制流指令实现 ====================

// 控制流指令都会改变程序计数器
bool ControlFlowInstruction::changesPC() const
{
    return true;
}

// BRANCH 指令：无条件跳转
void JumpInstruction::execute(VMContext& context, int operand)
{
    context.instructionCounter = operand; // 直接设置 PC 为目标地址
}

std::string JumpInstruction::getName() const
{
    return "BRANCH";
}

// JMPNEG 指令：累加器为负时跳转
void JumpNegInstruction::execute(VMContext& context, int operand)
{
    if (context.accumulator < 0)
    {
        context.instructionCounter = operand; // 跳转
    }
    else
    {
        context.instructionCounter++; // 继续执行下一条
    }
}

std::string JumpNegInstruction::getName() const
{
    return "BRANCHNEG";
}

// JUMPZERO 指令：累加器为零时跳转
void JumpZeroInstruction::execute(VMContext& context, int operand)
{
    if (context.accumulator == 0)
    {
        context.instructionCounter = operand; // 跳转
    }
    else
    {
        context.instructionCounter++; // 继续执行下一条
    }
}

std::string JumpZeroInstruction::getName() const
{
    return "BRANCHZERO";
}

// HALT 指令：停止虚拟机
void HaltInstruction::execute(VMContext& context, [[maybe_unused]] int operand)
{
    std::cout << "程序执行完毕。" << std::endl;
    context.running = false; // 停止运行
}

std::string HaltInstruction::getName() const
{
    return "HALT";
}
