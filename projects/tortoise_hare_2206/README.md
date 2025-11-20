# tortoise hare

使用现代C++20特性实现的经典龟兔赛跑问题模拟。

## 项目简介

本项目是一个教学性质的C++程序，模拟伊索寓言中著名的龟兔赛跑故事。乌龟和兔子在70格的赛道上进行竞赛，各自有不同的移动策略，使用随机数来模拟比赛的不确定性。

## 核心特性

### 现代C++20特性

- **Concepts（概念）**: 使用RacerConcept定义参赛者的类型约束
- **Ranges库**: 使用std::views::iota优雅地遍历赛道
- **std::format**: 现代化的格式化输出
- **constexpr**: 编译期常量表达式
- **[[nodiscard]]**: 防止忽略重要的返回值
- **noexcept**: 明确标注不抛异常的函数
- **enum class**: 类型安全的枚举
- **智能指针**: 使用std::unique_ptr自动管理内存
- **static_assert**: 编译期类型验证

### 面向对象设计

- **抽象基类**: Racer定义所有参赛者的公共接口
- **多态**: 通过虚函数实现不同的移动策略
- **模板元编程**: Race类使用模板支持任意类型的参赛者
- **单例模式**: RandomGenerator使用单例模式管理随机数生成

## 移动规则

### 乌龟（Tortoise）- 稳定但缓慢

基于1-10的随机数：

- **1-5 (50%)**: 快速移动，向右3格
- **6-7 (20%)**: 滑倒，向左6格
- **8-10 (30%)**: 缓慢移动，向右1格

### 兔子（Hare）- 快速但不稳定

基于1-10的随机数：

- **1-2 (20%)**: 睡觉，原地不动
- **3-4 (20%)**: 大跳，向右9格
- **5 (10%)**: 严重滑倒，向左12格
- **6-8 (30%)**: 小跳，向右1格
- **9-10 (20%)**: 轻微滑倒，向左2格

## 项目结构

```
tortoise_hare_2206/
├── include/                 # 头文件目录
│   ├── Constants.h         # 全局常量定义
│   ├── RacerConcept.h      # C++20概念定义
│   ├── RandomGenerator.h   # 随机数生成器声明
│   ├── Racer.h             # 参赛者抽象基类
│   ├── Tortoise.h          # 乌龟类声明
│   ├── Hare.h              # 兔子类声明
│   └── Race.h              # 比赛管理类模板
├── src/                    # 源文件目录
│   ├── RandomGenerator.cpp # 随机数生成器实现
│   ├── Racer.cpp           # 参赛者基类实现
│   ├── Tortoise.cpp        # 乌龟类实现
│   └── Hare.cpp            # 兔子类实现
├── main.cpp                # 主程序入口
├── CMakeLists.txt          # CMake构建配置
├── README.md               # 项目说明文档
└── tortoise_hare.pptx      # 项目需求文档
```

## 编译要求

- **CMake**: 3.20或更高版本
- **C++编译器**: 支持C++20标准
    - GCC 10+
    - Clang 13+
    - MSVC 2019 16.11+
    - Apple Clang 13+

## 编译和运行

### 使用CMake编译

```bash
# 配置项目
cmake -B build -S .

# 编译
cmake --build build

# 运行
./build/tortoise_hare_2206
```

### 清理重新编译

```bash
rm -rf build
cmake -B build -S .
cmake --build build
```

## 运行示例

```
BANG !!!!!
AND THEY'RE OFF !!!!!

Tick 1: HT
Tick 2: H   T
Tick 3:        T H
Tick 4:         T         H
...
Tick 47:                                    T

Hare wins. Yuch.

Race finished in 47 ticks!
Final positions - Tortoise: 36, Hare: 73
```

### 特殊情况

- **相遇**: 当乌龟和兔子在同一格时，显示OUCH!!!（乌龟咬住兔子）
- **乌龟获胜**: 显示TORTOISE WINS!!! YAY!!!（弱者的胜利）
- **兔子获胜**: 显示Hare wins. Yuch.
- **平局**: 显示It's a tie.

## 设计模式

### 1. 策略模式

- Racer抽象基类定义移动策略接口
- Tortoise和Hare实现不同的移动策略

### 2. 模板方法模式

- Race::run()定义比赛流程框架
- 各个步骤方法可被重写或扩展

### 3. 单例模式

- RandomGenerator确保全局只有一个随机数生成器实例

## 核心类说明

### Racer（参赛者抽象基类）

- **职责**: 定义所有参赛者的公共接口
- **关键方法**:
    - move(): 纯虚函数，由子类实现具体移动规则
    - getPosition(): 获取当前位置
    - hasFinished(): 判断是否到达终点

### Tortoise & Hare（具体参赛者）

- **职责**: 实现各自的移动策略
- **特点**: 使用final关键字防止进一步继承

### Race（比赛管理器）

- **职责**: 协调整个比赛过程
- **特点**: 使用模板和概念实现类型安全

### RandomGenerator（随机数生成器）

- **职责**: 提供高质量的随机数
- **特点**: 单例模式，使用梅森旋转算法

## 学习要点

本项目展示了以下C++编程技术：

1. **现代C++特性**: 展示C++20的新特性应用
2. **面向对象设计**: 继承、多态、封装
3. **模板元编程**: 类模板、概念约束
4. **RAII原则**: 使用智能指针管理资源
5. **异常安全**: try-catch异常处理
6. **代码组织**: 头文件和源文件的分离
7. **构建系统**: CMake的使用

## 开发说明

### 添加新的参赛者

1. 继承Racer基类
2. 实现move()虚函数
3. 确保满足RacerConcept概念

示例：

```cpp
class Cheetah final : public Racer {
public:
    Cheetah() : Racer("Cheetah", 'C') {}

    void move() override {
        // 实现猎豹的移动策略
        position += 5;  // 超快速度！
        ensureValidPosition();
    }
};
```

### 扩展比赛功能

Race类是一个模板类，可以接受任意满足RacerConcept的参赛者类型，非常容易扩展。

## 已知限制

- 赛道固定为70格
- 仅支持两个参赛者
- 移动规则固定，无法动态配置

## 未来改进方向

- 支持可配置的赛道长度
- 支持多个参赛者（3个及以上）
- 添加图形界面显示
- 实现回放功能
- 添加统计分析功能（胜率、平均用时等）
- 支持自定义移动规则配置

## 许可证

本项目仅供学习和教学使用。

## 作者

Claude Code - 2025

## 致谢

本项目基于经典的编程练习题改编，原题来源于C语言教材中的项目实战。

---

**Happy Coding!**

## 技术文档

- [头文件包含路径说明](docs/include-path-guide.md) - 解释为什么 `#include "Tortoise.h"` 不需要写路径
