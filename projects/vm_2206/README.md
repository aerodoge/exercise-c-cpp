# 现代 C++20 虚拟机项目文档

## 项目概述

这是一个基于现代 C++20 标准实现的教学型虚拟机，采用冯·诺依曼架构，实现了完整的取指-解码-执行（Fetch-Decode-Execute）循环。

### 技术特点

- **语言标准**: C++20
- **设计模式**: Command、Factory、Singleton、Builder、Template Method
- **架构特点**: 模块化设计，头文件与实现分离
- **现代特性**: Concepts、std::optional、[[nodiscard]]、强类型枚举

## 项目结构

```
vm_2206/
├── CMakeLists.txt              # CMake 构建配置
├── docs/                       # 文档目录
│   ├── README.md              # 项目总览
│   ├── DESIGN.md              # 设计文档
│   └── INSTRUCTION_SET.md     # 指令集说明
├── include/vm/                 # 头文件
│   ├── OpCode.h               # 操作码枚举
│   ├── VMContext.h            # 虚拟机上下文
│   ├── IInstruction.h         # 指令接口
│   ├── Instructions.h         # 指令类声明
│   ├── InstructionFactory.h   # 指令工厂
│   ├── VirtualMachine.h       # 虚拟机主控制器
│   └── ProgramBuilder.h       # 程序构建器
└── src/                        # 实现文件
    ├── Instructions.cpp       # 指令实现
    ├── InstructionFactory.cpp # 工厂实现
    ├── VirtualMachine.cpp     # 虚拟机实现
    ├── ProgramBuilder.cpp     # 构建器实现
    └── main.cpp               # 主程序入口
```

## 核心组件

### 1. VMContext - 虚拟机上下文

管理虚拟机的所有状态：

- **寄存器**: 累加器、指令计数器、指令寄存器
- **内存**: 100个存储单元（0-99）
- **状态**: 运行标志

**特性**:

- 使用 C++20 Concepts 约束模板参数
- 提供类型安全的内存访问接口

### 2. IInstruction - 指令接口

采用 **Command 模式** 和 **Strategy 模式**：

- 每个指令是独立的命令对象
- 统一的执行接口
- 支持查询指令是否改变程序计数器

### 3. Instructions - 指令实现

#### 指令分类

**I/O 指令**:

- `ReadInstruction`: 从终端读取输入
- `WriteInstruction`: 向终端输出

**加载/存储指令**:

- `LoadInstruction`: 内存 → 累加器
- `StoreInstruction`: 累加器 → 内存

**算术指令** (Template Method 模式):

- `ArithmeticInstruction`: 抽象基类，定义通用流程
- `AddInstruction`: 加法
- `SubtractInstruction`: 减法
- `MultiplyInstruction`: 乘法
- `DivideInstruction`: 除法（带除零检查）

**控制流指令**:

- `BranchInstruction`: 无条件跳转
- `BranchNegInstruction`: 负数条件跳转
- `BranchZeroInstruction`: 零条件跳转
- `HaltInstruction`: 停机

### 4. InstructionFactory - 指令工厂

采用 **Factory 模式** + **Singleton 模式**：

- 单例确保全局唯一
- 管理所有指令对象的生命周期
- 使用 `std::unique_ptr` 自动管理内存
- 使用 `std::optional` 安全返回指令指针

### 5. VirtualMachine - 虚拟机主控制器

实现经典的**冯·诺依曼架构**：

```cpp
while (running) {
    // 1. 取指 (Fetch)
    instructionRegister = memory[instructionCounter];

    // 2. 解码 (Decode)
    opcode = instructionRegister / 100;
    operand = instructionRegister % 100;

    // 3. 执行 (Execute)
    instruction->execute(context, operand);

    // 4. 更新 PC
    if (!instruction->changesPC()) {
        instructionCounter++;
    }
}
```

### 6. ProgramBuilder - 程序构建器

采用 **Builder 模式**，提供流式 API：

```cpp
auto program = ProgramBuilder()
    .addInstruction(+1007)  // READ A
    .addInstruction(+1008)  // READ B
    .addInstruction(+2007)  // LOAD A
    .addInstruction(+3008)  // ADD B
    .addInstruction(+1109)  // WRITE result
    .addInstruction(+4300)  // HALT
    .build();
```

## C++20 特性应用

### 1. Concepts - 概念约束

```cpp
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// 使用
template<Numeric T>
void setMemory(size_t address, T value) {
    memory[address] = static_cast<int>(value);
}
```

**优点**: 编译期类型检查，更清晰的错误信息

### 2. std::optional - 可选值

```cpp
[[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const {
    if (auto it = instructions_.find(opcode); it != instructions_.end()) {
        return it->second.get();
    }
    return std::nullopt;  // 安全地表示"没有找到"
}
```

**优点**: 避免空指针，明确表达"可能不存在"的语义

### 3. [[nodiscard]] - 不可丢弃

```cpp
[[nodiscard]] int getMemory(size_t address) const;
[[nodiscard]] std::array<int, MEMORY_SIZE> build() const;
```

**优点**: 编译器警告忽略返回值，防止逻辑错误

### 4. enum class - 强类型枚举

```cpp
enum class OpCode : int {
    READ = 10,
    WRITE = 11,
    // ...
};
```

**优点**:

- 类型安全，不能隐式转换
- 避免命名冲突
- 可指定底层存储类型

## 设计模式详解

### 1. Command 模式

**意图**: 将请求封装为对象

```cpp
// 指令接口
class IInstruction {
public:
    virtual void execute(VMContext& context, int operand) = 0;
};

// 具体命令
class AddInstruction : public IInstruction {
    void execute(VMContext& context, int operand) override {
        context.accumulator += context.getMemory(operand);
    }
};
```

**优点**:

- 解耦调用者和接收者
- 易于扩展新指令
- 支持撤销/重做（可扩展）

### 2. Factory 模式

**意图**: 创建对象的接口，延迟实例化

```cpp
class InstructionFactory {
    std::unordered_map<OpCode, std::unique_ptr<IInstruction>> instructions_;

public:
    std::optional<IInstruction*> getInstruction(OpCode opcode) const;
};
```

**优点**:

- 集中管理对象创建
- 隐藏创建细节
- 易于维护

### 3. Singleton 模式

**意图**: 确保类只有一个实例

```cpp
class InstructionFactory {
    InstructionFactory() { /* ... */ }  // 私有构造

public:
    static InstructionFactory& getInstance() {
        static InstructionFactory instance;  // C++11 线程安全
        return instance;
    }

    // 删除拷贝和移动
    InstructionFactory(const InstructionFactory&) = delete;
    InstructionFactory& operator=(const InstructionFactory&) = delete;
};
```

**优点**:

- 全局访问点
- 节省资源
- C++11 保证线程安全

### 4. Template Method 模式

**意图**: 定义算法骨架，子类实现细节

```cpp
// 抽象类定义流程
class ArithmeticInstruction : public IInstruction {
protected:
    virtual int compute(int acc, int operand) const = 0;  // 子类实现

public:
    void execute(VMContext& context, int operand) override {
        int value = context.getMemory(operand);  // 1. 读取
        context.accumulator = compute(context.accumulator, value);  // 2. 计算
    }
};

// 子类实现具体计算
class AddInstruction : public ArithmeticInstruction {
protected:
    int compute(int acc, int operand) const override {
        return acc + operand;
    }
};
```

**优点**:

- 复用通用流程
- 扩展点明确
- 减少代码重复

### 5. Builder 模式

**意图**: 分步骤构建复杂对象

```cpp
class ProgramBuilder {
    std::array<int, MEMORY_SIZE> program_{};
    size_t currentAddress_{0};

public:
    ProgramBuilder& addInstruction(int instruction) {
        program_[currentAddress_++] = instruction;
        return *this;  // 链式调用
    }

    std::array<int, MEMORY_SIZE> build() const {
        return program_;
    }
};
```

**优点**:

- 流式 API，可读性强
- 构建过程灵活
- 不可变结果对象

## 编译和运行

### 编译

```bash
# 创建构建目录
cmake -S . -B build

# 编译
cmake --build build
```

### 运行

```bash
./build/vm_2206
```

### 测试

```bash
# 测试加法
echo -e "1\n10\n20" | ./build/vm_2206

# 测试减法（条件分支）
echo -e "2\n15\n5" | ./build/vm_2206

# 测试乘法
echo -e "3\n6\n7" | ./build/vm_2206
```

## 示例程序

### 程序 1: 两数相加

```cpp
+1007  // READ 07: 读取 A 到内存[7]
+1008  // READ 08: 读取 B 到内存[8]
+2007  // LOAD 07: 内存[7] → 累加器
+3008  // ADD 08:  累加器 += 内存[8]
+2109  // STORE 09: 累加器 → 内存[9]
+1109  // WRITE 09: 输出内存[9]
+4300  // HALT: 停机
```

**执行流程**:

1. 读取两个数 (如 10, 20)
2. 加载第一个数到累加器 (acc = 10)
3. 累加器加上第二个数 (acc = 30)
4. 存储结果到内存 (mem[9] = 30)
5. 输出结果 (打印 30)

### 程序 2: 条件分支

```cpp
+1009  // READ 09: 读取 A
+1010  // READ 10: 读取 B
+2009  // LOAD 09: 加载 A
+3110  // SUBTRACT 10: acc = A - B
+4107  // BRANCHNEG 07: 如果 acc < 0，跳转到地址 7
+1109  // WRITE 09: 输出 A (结果为正)
+4300  // HALT
+1110  // WRITE 10: 输出 B (结果为负)
+4300  // HALT
```

**逻辑**: 输出两个数中较大的那个

## 扩展建议

### 1. 新增指令

```cpp
// 1. 在 OpCode.h 中定义操作码
enum class OpCode : int {
    // ... 现有指令
    MOD = 34,  // 取模运算
};

// 2. 创建指令类
class ModInstruction : public ArithmeticInstruction {
protected:
    int compute(int acc, int operand) const override {
        return acc % operand;
    }
public:
    std::string getName() const override { return "MOD"; }
};

// 3. 在工厂中注册
instructions_.emplace(OpCode::MOD, std::make_unique<ModInstruction>());
```

### 2. 添加调试器

```cpp
class Debugger {
    VirtualMachine& vm_;

public:
    void step();          // 单步执行
    void breakpoint(int address);  // 设置断点
    void printStack();    // 打印调用栈
};
```

### 3. 支持汇编语言

```cpp
class Assembler {
public:
    std::array<int, MEMORY_SIZE> assemble(const std::string& source) {
        // 解析汇编代码
        // READ A
        // READ B
        // LOAD A
        // ADD B
        // ...
    }
};
```

### 4. 添加更多寄存器

```cpp
class VMContext {
    int accumulator;
    int registerB;    // 新增通用寄存器
    int registerC;
    // ...
};
```

## 常见问题 (FAQ)

### Q1: READ 指令和 cin 的关系？

**A**:

- **READ 指令** 是虚拟机的抽象命令："从外部读取数据"
- **cin** 是实现这个命令的具体工具
- 关系：READ 指令触发 cin 的调用

类比：遥控器按钮(READ) 触发电视机(cin)工作

### Q2: enum class 中的 `: int` 是什么意思？

**A**:

- **不是继承**！是指定底层存储类型
- `enum class OpCode : int` 表示用 `int` 类型存储枚举值
- 可以改成其他类型：`uint8_t`、`int64_t` 等

### Q3: 为什么用 enum class 而不是 enum？

**A**: `enum class` 更安全：

- 类型安全，不能隐式转换
- 有自己的作用域，避免命名冲突
- 可以指定底层类型

```cpp
// enum - 不安全
enum Color { RED = 1 };
int x = RED;  // OK，但容易出错

// enum class - 安全
enum class Color { RED = 1 };
int x = Color::RED;  // 错误！必须显式转换
```

### Q4: 为什么用这么多设计模式？

**A**:

- **Command**: 指令对象化，易于扩展
- **Factory**: 集中管理创建，解耦
- **Singleton**: 工厂唯一实例
- **Builder**: 流式构建，可读性强
- **Template Method**: 复用算术指令流程

### Q5: 如何添加新指令？

**A**: 三步走：

1. `OpCode.h` 定义操作码
2. 创建指令类（继承 `IInstruction`）
3. `InstructionFactory` 中注册

## 学习资源

### 相关概念

- 冯·诺依曼架构
- 指令集架构 (ISA)
- 取指-解码-执行循环
- 虚拟机实现原理

### C++ 特性

- C++20 Concepts
- std::optional
- std::unique_ptr
- enum class
- [[nodiscard]]

### 设计模式

- Command Pattern
- Factory Pattern
- Singleton Pattern
- Builder Pattern
- Template Method Pattern

## 贡献指南

欢迎贡献！可以：

- 添加新指令
- 优化性能
- 改进文档
- 报告 Bug
- 提出建议

## 许可证

MIT License

## 作者

实现时间: 2025
基于: PPT 设计文档 (vm.pptx)
