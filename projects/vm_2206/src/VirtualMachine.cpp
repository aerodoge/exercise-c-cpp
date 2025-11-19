#include "../include/VirtualMachine.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

// 构造函数：初始化虚拟机
VirtualMachine::VirtualMachine() : factory_(InstructionFactory::getInstance())
{
    context_.reset(); // 重置所有状态
}

// 加载程序到内存
void VirtualMachine::loadProgram(const std::array<int, VMContext::MEMORY_SIZE>& program)
{
    context_.memory = program;
}

// 执行程序（主循环）
void VirtualMachine::execute()
{
    context_.running = true;         // 启动虚拟机
    context_.instructionCounter = 0; // PC从0开始

    // 主执行循环
    while (context_.running)
    {
        try
        {
            executeSingleInstruction(); // 执行一条指令
        }
        catch (const std::exception& e)
        {
            // 捕获运行时错误（如除零、未知操作码等）
            std::cerr << "运行时错误: " << e.what() << std::endl;
            context_.running = false;
        }
    }
}

// 执行单条指令（Fetch-Decode-Execute 循环）
void VirtualMachine::executeSingleInstruction()
{
    // 1. 取指（Fetch）：从内存读取当前指令
    context_.instructionRegister = context_.memory[context_.instructionCounter];

    // 2. 解码（Decode）：分离操作码和操作数
    // 指令格式：XXYY，XX 是操作码，YY 是操作数
    const int opcode = context_.instructionRegister / 100;  // 前两位
    const int operand = context_.instructionRegister % 100; // 后两位

    // 3. 获取指令对象
    auto const instructionOpt = factory_.getInstruction(static_cast<OpCode>(opcode));

    if (!instructionOpt.has_value())
    {
        // 操作码无效
        throw std::runtime_error("未知的操作码: " + std::to_string(opcode));
    }

    IInstruction* instruction = instructionOpt.value();

    // 4. 执行（Execute）：调用指令的execute方法
    instruction->execute(context_, operand);

    // 5. 更新 PC：如果不是跳转指令，则 PC 递增
    if (!instruction->changesPC())
    {
        context_.instructionCounter++;
    }
}

void VirtualMachine::dumpMemory() const
{
    std::cout << "\n内存转储:\n";
    std::cout << "       0     1     2     3     4     5     6     7     8     9\n";

    for (size_t i = 0; i < VMContext::MEMORY_SIZE; i += 10)
    {
        std::cout << std::setw(2) << i << " ";
        for (size_t j = 0; j < 10 && i + j < VMContext::MEMORY_SIZE; ++j)
        {
            std::cout << std::setw(5) << std::showpos << context_.memory[i + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::noshowpos;
}

void VirtualMachine::dumpRegisters() const
{
    std::cout << "\n寄存器状态:\n";
    std::cout << "累加器: " << std::showpos << context_.accumulator << std::endl;
    std::cout << std::noshowpos;
    std::cout << "指令计数器: " << context_.instructionCounter << std::endl;
    std::cout << "指令寄存器: " << std::showpos << context_.instructionRegister << std::endl;
    std::cout << std::noshowpos;
}
