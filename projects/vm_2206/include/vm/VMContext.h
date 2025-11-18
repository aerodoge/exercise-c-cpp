#pragma once

#include <array>
#include <concepts>
#include <stdexcept>

/**
 * @file VMContext.h
 * @brief 虚拟机上下文和状态管理
 *
 * 封装虚拟机的所有状态信息，包括寄存器和内存
 */
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

/**
 * @class VMContext
 * @brief 虚拟机上下文类
 *
 * 管理虚拟机的所有状态，包括：
 * - 寄存器（accumulator, instructionCounter, instructionRegister）
 * - 内存（100个单元）
 * - 运行状态
 */
class VMContext {
public:
    static constexpr size_t MEMORY_SIZE = 100;  // 内存大小：100个单元

    // 寄存器
    int accumulator{0};          // 累加器：用于算术运算
    int instructionCounter{0};   // 指令计数器：当前执行的指令地址
    int instructionRegister{0};  // 指令寄存器：当前指令的完整内容
    bool running{false};         // 运行状态：虚拟机是否正在运行
    std::array<int, MEMORY_SIZE> memory{};  // 内存：存储指令和数据

    /**
     * @brief 重置虚拟机状态
     *
     * 将所有寄存器和内存清零，停止运行
     */
    void reset() {
        accumulator = 0;
        instructionCounter = 0;
        instructionRegister = 0;
        running = false;
        memory.fill(0);
    }

    /**
     * @brief 设置内存值
     *
     * @tparam T 数值类型（整数或浮点数）
     * @param address 内存地址 (0-99)
     * @param value 要设置的值
     * @throws std::out_of_range 如果地址越界
     */
    template<Numeric T>
    void setMemory(size_t address, T value) {
        if (address >= MEMORY_SIZE) {
            throw std::out_of_range("内存地址越界");
        }
        memory[address] = static_cast<int>(value);
    }

    /**
     * @brief 获取内存值
     *
     * @param address 内存地址 (0-99)
     * @return 内存中的值
     * @throws std::out_of_range 如果地址越界
     */
    [[nodiscard]] int getMemory(size_t address) const {
        if (address >= MEMORY_SIZE) {
            throw std::out_of_range("内存地址越界");
        }
        return memory[address];
    }
};
