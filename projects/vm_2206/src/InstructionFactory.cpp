#include "vm/InstructionFactory.h"
#include "vm/Instructions.h"

/**
 * @file InstructionFactory.cpp
 * @brief 指令工厂类实现
 */

// 构造函数：初始化所有指令对象
InstructionFactory::InstructionFactory() {
    instructions_.emplace(OpCode::READ, std::make_unique<ReadInstruction>());
    instructions_.emplace(OpCode::WRITE, std::make_unique<WriteInstruction>());
    instructions_.emplace(OpCode::LOAD, std::make_unique<LoadInstruction>());
    instructions_.emplace(OpCode::STORE, std::make_unique<StoreInstruction>());
    instructions_.emplace(OpCode::ADD, std::make_unique<AddInstruction>());
    instructions_.emplace(OpCode::SUB, std::make_unique<SubtractInstruction>());
    instructions_.emplace(OpCode::MUL, std::make_unique<MultiplyInstruction>());
    instructions_.emplace(OpCode::DIV, std::make_unique<DivideInstruction>());
    instructions_.emplace(OpCode::JMP, std::make_unique<BranchInstruction>());
    instructions_.emplace(OpCode::JMPNEG, std::make_unique<BranchNegInstruction>());
    instructions_.emplace(OpCode::JMPZERO, std::make_unique<BranchZeroInstruction>());
    instructions_.emplace(OpCode::HALT, std::make_unique<HaltInstruction>());
}

// 获取单例实例
InstructionFactory& InstructionFactory::getInstance() {
    static InstructionFactory instance;  // 静态局部变量，保证线程安全的单例
    return instance;
}

// 根据操作码获取指令对象
std::optional<IInstruction*> InstructionFactory::getInstruction(const OpCode opcode) const {
    if (auto it = instructions_.find(opcode); it != instructions_.end()) {
        return it->second.get();  // 返回 unique_ptr 管理的裸指针
    }
    return std::nullopt;  // 操作码无效时返回空
}
