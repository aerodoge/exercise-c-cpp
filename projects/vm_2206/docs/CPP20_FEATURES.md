# C++20 特性详解

本文档详细解释项目中使用的 C++20 特性。

## 1. Concepts - 概念约束

### 什么是 Concepts？

Concepts 是 C++20 引入的重要特性，用于**约束模板参数**，提供编译期类型检查。

### 项目中的使用

```cpp
// VMContext.h
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;
```

**定义**: `Numeric` 概念要求类型 `T` 必须是整数或浮点数类型。

### 应用

```cpp
class VMContext {
public:
    // 使用 Concept 约束模板参数
    template<Numeric T>
    void setMemory(size_t address, T value) {
        if (address >= MEMORY_SIZE) {
            throw std::out_of_range("内存地址越界");
        }
        memory[address] = static_cast<int>(value);
    }
};
```

### 好处

#### 1. 类型安全

```cpp
// ✓ 允许
context.setMemory(0, 42);        // int
context.setMemory(1, 3.14);      // double
context.setMemory(2, 'A');       // char (整数类型)

// ✗ 编译错误（清晰的错误信息）
context.setMemory(3, "hello");   // 字符串不满足 Numeric
context.setMemory(4, myObject);  // 自定义类型不满足 Numeric
```

#### 2. 更好的错误信息

**没有 Concepts (C++17)**:
```
error: no matching function for call to 'setMemory'
  candidate template ignored: substitution failure...
  [150 行模板实例化错误信息]
```

**使用 Concepts (C++20)**:
```
error: constraints not satisfied for 'setMemory<std::string>'
  because 'std::string' does not satisfy 'Numeric'
```

#### 3. 文档化

Concepts 本身就是文档，清楚表明函数期望什么类型。

### 标准库 Concepts

```cpp
#include <concepts>

// 常用的标准 concepts
std::integral<T>           // 整数类型
std::floating_point<T>     // 浮点类型
std::same_as<T, U>         // 相同类型
std::convertible_to<T, U>  // 可转换类型
std::copy_constructible<T> // 可拷贝构造
```

### 定义自己的 Concepts

```cpp
// 简单 concept
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// 复杂 concept
template<typename T>
concept Container = requires(T t) {
    typename T::value_type;     // 必须有 value_type
    { t.size() } -> std::integral;  // 必须有 size() 返回整数
    { t.begin() };              // 必须有 begin()
    { t.end() };                // 必须有 end()
};
```

---

## 2. std::optional - 可选值

### 什么是 std::optional？

`std::optional<T>` 表示"可能有值，也可能没有值"的类型，是 C++17 引入、C++20 改进的特性。

### 项目中的使用

```cpp
// InstructionFactory.h
[[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const;
```

### 实现

```cpp
// InstructionFactory.cpp
std::optional<IInstruction*> InstructionFactory::getInstruction(OpCode opcode) const {
    if (auto it = instructions_.find(opcode); it != instructions_.end()) {
        return it->second.get();  // 有值
    }
    return std::nullopt;  // 无值
}
```

### 使用

```cpp
// VirtualMachine.cpp
auto instructionOpt = factory_.getInstruction(static_cast<OpCode>(opcode));

// 方法1: has_value() + value()
if (instructionOpt.has_value()) {
    IInstruction* instruction = instructionOpt.value();
    instruction->execute(context_, operand);
}

// 方法2: 隐式转换为 bool
if (instructionOpt) {
    auto instruction = *instructionOpt;  // 解引用
    instruction->execute(context_, operand);
}

// 方法3: value_or()
auto instruction = instructionOpt.value_or(nullptr);
```

### 为什么不用指针？

**传统方法 (指针)**:
```cpp
IInstruction* getInstruction(OpCode opcode) const {
    if (auto it = instructions_.find(opcode); it != instructions_.end()) {
        return it->second.get();
    }
    return nullptr;  // 容易忘记检查
}

// 使用
auto instruction = factory.getInstruction(opcode);
instruction->execute(...);  // ← 可能崩溃！忘记检查 nullptr
```

**std::optional 方法**:
```cpp
std::optional<IInstruction*> getInstruction(OpCode opcode) const;

// 使用
auto instructionOpt = factory.getInstruction(opcode);
// 必须显式检查，否则 value() 会抛出异常
if (instructionOpt.has_value()) {
    instructionOpt.value()->execute(...);  // 安全
}
```

### 好处

1. **明确语义**: "可能没有值"写在类型里
2. **强制检查**: 编译器和静态分析工具会提示
3. **异常安全**: `value()` 在无值时抛出 `std::bad_optional_access`

---

## 3. [[nodiscard]] - 不可丢弃属性

### 什么是 [[nodiscard]]？

C++17 引入的属性，标记函数返回值不应该被忽略。

### 项目中的使用

```cpp
// VMContext.h
[[nodiscard]] int getMemory(size_t address) const;

// ProgramBuilder.h
[[nodiscard]] std::array<int, MEMORY_SIZE> build() const;

// InstructionFactory.h
[[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const;
```

### 效果

```cpp
// ✗ 编译警告
context.getMemory(5);  // warning: ignoring return value marked 'nodiscard'

// ✓ 正确使用
int value = context.getMemory(5);  // OK
```

### 为什么需要？

#### 问题场景

```cpp
// 程序员的错误
builder.addInstruction(+1007)
       .addInstruction(+1008);
builder.build();  // ← 忘记接收返回值！

vm.loadProgram(builder.build());  // ← 又构建了一次
```

#### 使用 [[nodiscard]]

```cpp
[[nodiscard]] std::array<int, MEMORY_SIZE> build() const;

// 编译警告
builder.build();  // warning: ignoring return value of 'build'

// 正确
auto program = builder.build();
vm.loadProgram(program);
```

### 带消息的 [[nodiscard]]（C++20 增强）

```cpp
// C++20 可以添加自定义消息
[[nodiscard("程序未被使用")]]
std::array<int, MEMORY_SIZE> build() const;

// 警告信息
warning: ignoring return value: 程序未被使用
```

---

## 4. enum class - 强类型枚举

### 传统 enum 的问题

```cpp
enum Color { RED = 1, GREEN = 2 };
enum Status { RED = 1, RUNNING = 2 };  // 错误！RED 重定义

int x = RED;  // OK，但容易混淆
if (RED == RUNNING) { }  // 毫无意义的比较，但能编译
```

### enum class 解决方案

```cpp
// OpCode.h
enum class OpCode : int {
    READ = 10,
    WRITE = 11,
    // ...
};
```

### 特点

#### 1. 有作用域

```cpp
enum class OpCode { READ = 10 };
enum class Color { RED = 1 };  // OK，不冲突

// 使用必须加作用域
OpCode op = OpCode::READ;  // ✓
OpCode op = READ;          // ✗ 错误
```

#### 2. 类型安全

```cpp
enum class OpCode : int { READ = 10 };

// 不能隐式转换
int x = OpCode::READ;  // ✗ 错误
int x = static_cast<int>(OpCode::READ);  // ✓ 必须显式转换

// 不能跨枚举比较
enum class Color { RED = 10 };
if (OpCode::READ == Color::RED) { }  // ✗ 错误
```

#### 3. 可指定底层类型

```cpp
enum class OpCode : int { /* ... */ };       // 4 字节
enum class TinyCode : uint8_t { /* ... */ }; // 1 字节
enum class BigCode : int64_t { /* ... */ };  // 8 字节

sizeof(OpCode) == 4;
sizeof(TinyCode) == 1;
sizeof(BigCode) == 8;
```

### `: int` 不是继承！

**常见误解**:
```cpp
enum class OpCode : int { ... };  // ← 这不是继承！
```

**正确理解**: `: int` 指定**底层存储类型**

```cpp
// 底层类型决定了：
// 1. 枚举值的范围
// 2. 内存占用大小
// 3. 与整数的转换规则

enum class SmallCode : uint8_t {  // 底层类型 uint8_t
    MAX = 255  // 不能超过 255
};

enum class BigCode : int64_t {    // 底层类型 int64_t
    HUGE = 9999999999999  // 可以很大
};
```

### 项目中的使用

```cpp
// 定义
enum class OpCode : int {
    READ = 10,
    WRITE = 11,
    // ...
};

// 作为 key 使用
std::unordered_map<OpCode, std::unique_ptr<IInstruction>> instructions_;

// 转换
int opcode = context_.instructionRegister / 100;
auto op = static_cast<OpCode>(opcode);  // int → OpCode

// 查找
auto it = instructions_.find(op);
```

---

## 5. 其他现代 C++ 特性

### 5.1 结构化绑定 (C++17)

```cpp
// InstructionFactory.cpp
if (auto it = instructions_.find(opcode); it != instructions_.end()) {
    return it->second.get();
}
```

**分解**:
```cpp
// 等价于
auto it = instructions_.find(opcode);
if (it != instructions_.end()) {
    return it->second.get();
}
```

### 5.2 if/switch 初始化语句 (C++17)

```cpp
// 在 if 条件中初始化变量
if (auto it = map.find(key); it != map.end()) {
    // it 只在这个作用域内有效
    return it->second;
}
// it 在这里不可见
```

### 5.3 [[maybe_unused]] (C++17)

```cpp
// HaltInstruction
void execute(VMContext& context, [[maybe_unused]] int operand) override {
    std::cout << "程序执行完毕。" << std::endl;
    context.running = false;
    // operand 没有使用，但加了 [[maybe_unused]] 不会警告
}
```

### 5.4 constexpr (增强)

```cpp
// VMContext.h
static constexpr size_t MEMORY_SIZE = 100;

// 编译期常量，可用于数组大小等
std::array<int, VMContext::MEMORY_SIZE> memory;
```

---

## 特性对比总结

| 特性 | C++11/14/17 | C++20 | 本项目使用 |
|------|-------------|-------|-----------|
| **Concepts** | ✗ | ✓ | `Numeric` 约束 |
| **std::optional** | ✗ (C++17) | 改进 | 指令查找 |
| **[[nodiscard]]** | ✗ (C++17) | 增强 | 防止忽略返回值 |
| **enum class** | ✓ (C++11) | - | `OpCode` 定义 |
| **constexpr** | 基础 (C++11) | 增强 | 编译期常量 |
| **[[maybe_unused]]** | ✗ (C++17) | - | 未使用参数 |

---

## 编译器支持

### 要求

- **GCC**: >= 10
- **Clang**: >= 10
- **MSVC**: >= 19.26 (Visual Studio 2019 16.6)

### CMake 配置

```cmake
cmake_minimum_required(VERSION 3.20)
project(vm_2206)

set(CMAKE_CXX_STANDARD 20)           # 使用 C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 必须支持 C++20
```

### 编译选项

```bash
# GCC/Clang
g++ -std=c++20 -Wall -Wextra main.cpp

# 检查 C++20 支持
g++ --version
clang++ --version
```

---

## 延伸学习

### 其他 C++20 特性（未在本项目使用）

1. **Modules**: 替代头文件
2. **Ranges**: 强大的范围操作
3. **Coroutines**: 协程支持
4. **三路比较 (<=>)**: 太空船运算符
5. **consteval**: 强制编译期求值
6. **constinit**: 常量初始化

### 推荐资源

- [C++ Reference - C++20](https://en.cppreference.com/w/cpp/20)
- [C++20 Features](https://github.com/AnthonyCalandra/modern-cpp-features)
- [Concepts Tutorial](https://www.modernescpp.com/index.php/c-20-concepts)
