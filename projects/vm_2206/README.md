# 现代 C++20 虚拟机项目

基于现代 C++20 标准实现的教学型虚拟机，采用冯·诺依曼架构。

## 快速开始

### 编译

```bash
cmake -S . -B build
cmake --build build
```

### 运行

```bash
./build/vm_2206
```

## 项目特点

- ✅ **C++20 标准**: 使用 Concepts、std::optional、[[nodiscard]] 等现代特性
- ✅ **设计模式**: Command、Factory、Singleton、Builder、Template Method
- ✅ **模块化设计**: 头文件与实现分离，清晰的项目结构
- ✅ **详细注释**: 所有代码都有中文注释，易于理解
- ✅ **完整文档**: 包含架构设计、指令集、C++20 特性详解

## 项目结构

```
vm_2206/
├── include/vm/          # 头文件
├── src/                 # 实现文件
├── docs/                # 文档
└── CMakeLists.txt       # 构建配置
```

## 文档导航

📚 **完整文档请查看 `docs/` 目录**：

- **[项目总览](docs/README.md)** - 架构设计、核心组件、设计模式详解
- **[指令集手册](docs/INSTRUCTION_SET.md)** - 所有指令的详细说明和示例
- **[C++20 特性](docs/CPP20_FEATURES.md)** - Concepts、std::optional、enum class 等特性详解

## 支持的指令

| 类型 | 指令 | 操作码 | 说明 |
|------|------|--------|------|
| **I/O** | READ | 10 | 从终端读取输入 |
| | WRITE | 11 | 向终端输出 |
| **存储** | LOAD | 20 | 内存 → 累加器 |
| | STORE | 21 | 累加器 → 内存 |
| **运算** | ADD | 30 | 加法 |
| | SUBTRACT | 31 | 减法 |
| | DIVIDE | 32 | 除法 |
| | MULTIPLY | 33 | 乘法 |
| **控制** | BRANCH | 40 | 无条件跳转 |
| | BRANCHNEG | 41 | 负数条件跳转 |
| | BRANCHZERO | 42 | 零条件跳转 |
| | HALT | 43 | 停机 |

## 示例程序

### 两数相加

```cpp
+1007  // READ 07: 读取第一个数
+1008  // READ 08: 读取第二个数
+2007  // LOAD 07: 加载到累加器
+3008  // ADD 08:  累加
+2109  // STORE 09: 存储结果
+1109  // WRITE 09: 输出结果
+4300  // HALT: 停机
```

更多示例请查看 [指令集手册](docs/INSTRUCTION_SET.md)。

## 技术亮点

### 设计模式应用

- **Command 模式**: 每个指令是独立的命令对象
- **Factory 模式**: 集中管理指令创建
- **Singleton 模式**: 指令工厂单例
- **Builder 模式**: 流式 API 构建程序
- **Template Method 模式**: 算术指令通用流程

### C++20 特性

```cpp
// Concepts - 类型约束
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// std::optional - 安全返回值
[[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const;

// enum class - 强类型枚举
enum class OpCode : int {
    READ = 10,
    WRITE = 11,
    // ...
};
```

详细说明请查看 [C++20 特性文档](docs/CPP20_FEATURES.md)。

## 开发环境

### 编译器要求

- GCC >= 10
- Clang >= 10
- MSVC >= 19.26 (Visual Studio 2019 16.6)

### 依赖

- CMake >= 3.20
- C++20 标准库

## 测试

```bash
# 测试加法
echo -e "1\n10\n20" | ./build/vm_2206

# 测试条件分支
echo -e "2\n15\n5" | ./build/vm_2206

# 测试乘法
echo -e "3\n6\n7" | ./build/vm_2206
```

## 扩展建议

- 添加更多指令（如取模、位运算）
- 实现调试器（断点、单步执行）
- 支持汇编语言
- 添加更多寄存器
- 实现栈支持

## 许可证

MIT License

## 参考资料

- [项目文档](docs/README.md)
- [指令集手册](docs/INSTRUCTION_SET.md)
- [C++20 特性详解](docs/CPP20_FEATURES.md)
