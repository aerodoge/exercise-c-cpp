#include "vm/VirtualMachine.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// 构造函数：初始化虚拟机
VirtualMachine::VirtualMachine() : _factory(InstructionFactory::getInstance()) {
    _context.reset();  // 重置所有状态
}

// 加载程序到内存
void VirtualMachine::loadProgram(const std::array<int, VMContext::MEMORY_SIZE>& program) {
    _context.memory = program;
}

// 执行程序（主循环）
void VirtualMachine::execute() {
    _context.running = true;           // 启动虚拟机
    _context.instructionCounter = 0;   // PC从0开始

    // 主执行循环
    while (_context.running) {
        try {
            executeSingleInstruction();  // 执行一条指令
        } catch (const std::exception& e) {
            // 捕获运行时错误（如除零、未知操作码等）
            std::cerr << "运行时错误: " << e.what() << std::endl;
            _context.running = false;
        }
    }
}

// 执行单条指令（Fetch-Decode-Execute 循环）
void VirtualMachine::executeSingleInstruction() {
    // 1. 取指（Fetch）：从内存读取当前指令
    _context.instructionRegister = _context.memory[_context.instructionCounter];

    // 2. 解码（Decode）：分离操作码和操作数
    // 指令格式：XXYY，XX 是操作码，YY 是操作数
    int opcode = _context.instructionRegister / 100;   // 前两位
    int operand = _context.instructionRegister % 100;  // 后两位

    // 3. 获取指令对象（通过工厂）
    auto instructionOpt = _factory.getInstruction(static_cast<OpCode>(opcode));

    if (!instructionOpt.has_value()) {
        // 操作码无效
        throw std::runtime_error("未知的操作码: " + std::to_string(opcode));
    }

    IInstruction* instruction = instructionOpt.value();

    // 4. 执行（Execute）：调用指令的 execute 方法
    instruction->execute(_context, operand);

    // 5. 更新 PC：如果不是跳转指令，则 PC 递增
    if (!instruction->changesPC()) {
        _context.instructionCounter++;
    }
}

void VirtualMachine::dumpMemory() const {
    std::cout << "\n内存转储:\n";
    std::cout << "       0     1     2     3     4     5     6     7     8     9\n";

    for (size_t i = 0; i < VMContext::MEMORY_SIZE; i += 10) {
        std::cout << std::setw(2) << i << " ";
        for (size_t j = 0; j < 10 && (i + j) < VMContext::MEMORY_SIZE; ++j) {
            std::cout << std::setw(5) << std::showpos << _context.memory[i + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::noshowpos;
}

void VirtualMachine::dumpRegisters() const {
    std::cout << "\n寄存器状态:\n";
    std::cout << "累加器: " << std::showpos << _context.accumulator << std::endl;
    std::cout << std::noshowpos;
    std::cout << "指令计数器: " << _context.instructionCounter << std::endl;
    std::cout << "指令寄存器: " << std::showpos << _context.instructionRegister << std::endl;
    std::cout << std::noshowpos;
}
