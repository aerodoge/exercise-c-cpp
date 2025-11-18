#include "vm/VirtualMachine.h"
#include "vm/ProgramBuilder.h"
#include <iostream>

int main() {
    // 显示虚拟机支持的指令集
    std::cout << "\n支持的指令集:" << std::endl;
    std::cout << "  I/O: READ(10), WRITE(11)" << std::endl;
    std::cout << "  存储: LOAD(20), STORE(21)" << std::endl;
    std::cout << "  运算: ADD(30), SUBTRACT(31), DIVIDE(32), MULTIPLY(33)" << std::endl;
    std::cout << "  控制: BRANCH(40), BRANCHNEG(41), BRANCHZERO(42), HALT(43)" << std::endl;

    std::cout << "\n请选择示例程序:" << std::endl;
    std::cout << "1. 两数相加" << std::endl;
    std::cout << "2. 两数相减（含条件分支）" << std::endl;
    std::cout << "3. 乘法运算" << std::endl;
    std::cout << "请输入选择 (1-3): ";

    int choice;
    std::cin >> choice;
    std::cout << std::endl;

    // 创建虚拟机和程序构建器
    VirtualMachine vm;
    ProgramBuilder builder;

    switch (choice) {
        case 1: {
            // ==================== 示例程序 1：两数相加 ====================
            std::cout << "=== 示例程序 1: 两数相加 ===" << std::endl;
            std::cout << "程序将读取两个整数，计算它们的和并输出结果。\n" << std::endl;

            // 使用 Builder 模式构建程序
            // 指令格式：XXYY (XX=操作码, YY=操作数/地址)
            auto program = builder
                .addInstruction(+1007)  // READ 07: 读取第一个数到地址 7
                .addInstruction(+1008)  // READ 08: 读取第二个数到地址 8
                .addInstruction(+2007)  // LOAD 07: 将地址 7 的值加载到累加器
                .addInstruction(+3008)  // ADD 08: 累加器 += 地址 8 的值
                .addInstruction(+2109)  // STORE 09: 将累加器的值存到地址 9
                .addInstruction(+1109)  // WRITE 09: 输出地址 9 的值（结果）
                .addInstruction(+4300)  // HALT: 停机
                .build();

            vm.loadProgram(program);
            break;
        }

        case 2: {
            // ==================== 示例程序 2：两数相减（含条件分支）====================
            std::cout << "=== 示例程序 2: 两数相减（含条件分支）===" << std::endl;
            std::cout << "程序将读取两个整数，计算 A - B。" << std::endl;
            std::cout << "如果结果为负，输出 B；否则输出 A。\n" << std::endl;

            // 条件跳转
            auto program = builder
                .addInstruction(+1009)  // READ 09: 读取 A 到地址 9
                .addInstruction(+1010)  // READ 10: 读取 B 到地址 10
                .addInstruction(+2009)  // LOAD 09: 加载 A 到累加器
                .addInstruction(+3110)  // SUB 10: 累加器 -= B
                .addInstruction(+4107)  // JMPNEG 07: 如果累加器为负，跳转到地址 7
                .addInstruction(+1109)  // WRITE 09: 输出 A（结果为正）
                .addInstruction(+4300)  // HALT: 停机
                .addInstruction(+1110)  // WRITE 10: 输出 B（结果为负）
                .addInstruction(+4300)  // HALT: 停机
                .build();

            vm.loadProgram(program);
            break;
        }

        case 3: {
            // ==================== 示例程序 3：乘法运算 ====================
            std::cout << "=== 示例程序 3: 乘法运算 ===" << std::endl;
            std::cout << "程序将读取两个整数，计算它们的乘积并输出结果。\n" << std::endl;

            // 乘法
            auto program = builder
                .addInstruction(+1020)  // READ 20: 读取第一个数到地址 20
                .addInstruction(+1021)  // READ 21: 读取第二个数到地址 21
                .addInstruction(+2020)  // LOAD 20: 加载第一个数到累加器
                .addInstruction(+3321)  // MUL 21: 累加器 *= 地址 21 的值
                .addInstruction(+2122)  // STORE 22: 将结果存到地址 22
                .addInstruction(+1122)  // WRITE 22: 输出结果
                .addInstruction(+4300)  // HALT: 停机
                .build();

            vm.loadProgram(program);
            break;
        }

        default:
            std::cerr << "无效的选择！" << std::endl;
            return 1;
    }

    // 执行程序
    vm.execute();

    // 显示执行完成后的虚拟机状态
    std::cout << "\n执行完成后的状态:" << std::endl;
    vm.dumpRegisters();  // 显示寄存器状态
    vm.dumpMemory();     // 显示内存内容

    return 0;
}
