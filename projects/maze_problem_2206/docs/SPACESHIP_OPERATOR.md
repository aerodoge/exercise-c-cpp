# 三路比较运算符（Spaceship Operator）详解

## 概述

三路比较运算符 `<=>` 是 **C++20** 引入的新特性，位于 `include/position.hpp:21`：

```cpp
struct Position {
    int row;
    int col;

    auto operator<=>(const Position&) const = default;  // ← 仅此一行
};
```

这一行代码自动生成了 **6 个比较运算符**，极大简化了代码编写。

## 为什么叫"Spaceship"（飞船）？

因为符号 `<=>` 看起来像一艘飞船：🚀

## 核心概念

### 什么是"三路"比较？

传统的比较运算符每次只能判断一种关系：
```cpp
a < b   // 只能判断"小于"
a == b  // 只能判断"等于"
```

三路比较一次可以返回**三种结果**：
```cpp
a <=> b 的结果：
  • a < b  → 返回 "小于"（less）
  • a == b → 返回 "等于"（equal）
  • a > b  → 返回 "大于"（greater）
```

就像三岔路口，一次比较得到三个方向的信息。

## 传统方式 vs 三路比较

### C++17 及之前：需要写 6 个函数

```cpp
struct Position {
    int row;
    int col;

    // 1. 等于
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    // 2. 不等于
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    // 3. 小于
    bool operator<(const Position& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }

    // 4. 小于等于
    bool operator<=(const Position& other) const {
        return *this < other || *this == other;
    }

    // 5. 大于
    bool operator>(const Position& other) const {
        return !(*this <= other);
    }

    // 6. 大于等于
    bool operator>=(const Position& other) const {
        return !(*this < other);
    }
};
```

**总计：20+ 行代码，容易出错，难以维护**

### C++20：一行搞定

```cpp
struct Position {
    int row;
    int col;

    auto operator<=>(const Position&) const = default;
};
```

**自动生成所有 6 个比较运算符：**
- `==` (等于)
- `!=` (不等于)
- `<`  (小于)
- `<=` (小于等于)
- `>`  (大于)
- `>=` (大于等于)

## 语法详解

```cpp
auto operator<=>(const Position& other) const = default;
│    │          │                       │      │
│    │          │                       │      └─ 让编译器自动生成实现
│    │          │                       └─ const成员函数（不修改this对象）
│    │          └─ 参数：另一个Position对象的const引用
│    └─ 运算符名称：三路比较运算符
└─ 返回类型：auto（让编译器自动推导）
```

### `= default` 的作用

```cpp
= default
```

告诉编译器：
> "请帮我自动生成这个函数，按照成员变量的声明顺序逐个比较"

编译器会生成类似这样的代码（伪代码）：
```cpp
auto Position::operator<=>(const Position& other) const {
    // 先比较 row
    if (auto cmp = row <=> other.row; cmp != 0)
        return cmp;  // row 不同，直接返回比较结果

    // row 相同，再比较 col
    return col <=> other.col;
}
```

## 比较逻辑：字典序

Position 的比较采用**字典序**（Lexicographical Order），类似字典中单词的排序：

```cpp
Position p1{1, 5};   // row=1, col=5
Position p2{1, 10};  // row=1, col=10
Position p3{2, 3};   // row=2, col=3

// 比较规则：
// 1. 先比较 row
// 2. 如果 row 相同，再比较 col
// 3. 如果 row 不同，直接确定大小

p1 < p2  // true  (row相同(1==1)，比较col: 5 < 10)
p1 < p3  // true  (row不同(1<2)，直接得出结果)
p2 < p3  // true  (row不同(1<2))
p1 == p1 // true  (row相同且col相同)
```

### 字典序示例

类比字典中的单词排序：
```
apple    (a, p, p, l, e)
apply    (a, p, p, l, y)  ← 前4个字母相同，第5个 'e' < 'y'
banana   (b, a, n, a, n, a) ← 第1个字母 'a' < 'b'
```

Position 的比较同理：
```
{1, 5}   (row=1, col=5)
{1, 10}  (row=1, col=10)  ← row相同，col: 5 < 10
{2, 3}   (row=2, col=3)   ← row: 1 < 2
```

## 实际使用示例

### 基本比较

```cpp
Position p1{1, 5};
Position p2{1, 10};
Position p3{2, 3};

// 所有比较运算符都可用
if (p1 == p2) { /* ... */ }  // false
if (p1 != p2) { /* ... */ }  // true
if (p1 < p2)  { /* ... */ }  // true (字典序)
if (p1 <= p2) { /* ... */ }  // true
if (p1 > p3)  { /* ... */ }  // false
if (p1 >= p1) { /* ... */ }  // true
```

### 在迷宫代码中的应用

#### 1. 判断是否到达终点

```cpp
// src/maze.tpp:81 和 src/maze.tpp:131
if (current == end_) {  // ← 使用 operator==
    mark(current, 'X');
    display();
    return true;
}
```

#### 2. 用于 STL 容器

```cpp
#include <set>
#include <map>
#include <vector>

// set 需要 operator<
std::set<Position> visited;
Position p{1, 1};
visited.insert(p);  // ✓ 内部使用 < 排序

// map 需要 operator<
std::map<Position, int> distances;
distances[{2, 3}] = 10;  // ✓ 可以作为键

// 排序需要 operator<
std::vector<Position> path = {{3,4}, {1,2}, {2,1}};
std::sort(path.begin(), path.end());
// 结果: {1,2}, {2,1}, {3,4}（字典序）
```

#### 3. 用于优先队列（扩展）

```cpp
#include <queue>

// A* 算法中的应用（假设扩展）
struct Node {
    Position pos;
    int cost;

    auto operator<=>(const Node&) const = default;
};

std::priority_queue<Node> pq;  // ✓ 需要比较运算符
```

## 返回类型详解

### 为什么用 `auto`？

```cpp
auto operator<=>(const Position&) const = default;
^^^^
```

因为 `<=>` 的返回类型取决于成员类型，有三种可能：

#### 1. `std::strong_ordering`（强序）

```cpp
#include <compare>

int a = 5, b = 10;
auto result = a <=> b;  // 类型: std::strong_ordering

// 可能的返回值：
// - std::strong_ordering::less      (a < b)
// - std::strong_ordering::equal     (a == b)
// - std::strong_ordering::greater   (a > b)
```

**特性**：
- 任意两个值都可以比较
- 要么 a<b，要么 a==b，要么 a>b
- 如果 a==b，则它们可以互相替换

**示例类型**：`int`, `char`, `std::string`

#### 2. `std::weak_ordering`（弱序）

```cpp
// 大小写不敏感的字符串比较
struct CaseInsensitiveString {
    std::string data;

    std::weak_ordering operator<=>(const CaseInsensitiveString& other) const {
        // "Hello" 和 "hello" 等价但不相等
    }
};
```

**特性**：
- 有些不同的值可能"等价"但不"相等"
- 例如："Hello" 和 "hello" 在大小写不敏感比较中等价

#### 3. `std::partial_ordering`（偏序）

```cpp
double x = 1.0;
double y = std::numeric_limits<double>::quiet_NaN();
auto result = x <=> y;  // std::partial_ordering::unordered

// 可能的返回值：
// - partial_ordering::less
// - partial_ordering::equal
// - partial_ordering::greater
// - partial_ordering::unordered  ← 无法比较（如NaN）
```

**特性**：
- 某些值对无法比较
- 例如：浮点数的 NaN 与任何值比较都是 unordered

### Position 的返回类型

```cpp
struct Position {
    int row;  // int 支持 strong_ordering
    int col;  // int 支持 strong_ordering

    auto operator<=>(const Position&) const = default;
    // 编译器推导出: std::strong_ordering
};
```

因为 `int` 支持强序，所以 `Position` 也是强序。

## 自定义比较逻辑

### 示例1：只比较 row，忽略 col

```cpp
struct Position {
    int row;
    int col;

    // 自定义：只关心 row
    auto operator<=>(const Position& other) const {
        return row <=> other.row;  // 忽略 col
    }

    // 注意：还需要自定义 == 运算符
    bool operator==(const Position& other) const {
        return row == other.row;
    }
};

Position p1{1, 5};
Position p2{1, 10};
p1 == p2  // true（col 被忽略）
```

### 示例2：反向排序

```cpp
struct Position {
    int row;
    int col;

    // 反向字典序：row 从大到小
    auto operator<=>(const Position& other) const {
        if (auto cmp = other.row <=> row; cmp != 0)  // 注意顺序颠倒
            return cmp;
        return other.col <=> col;
    }
};

Position p1{1, 5};
Position p2{2, 3};
p1 < p2  // false（因为 2 > 1，反向排序）
```

### 示例3：曼哈顿距离排序

```cpp
struct Position {
    int row;
    int col;

    // 按离原点的曼哈顿距离排序
    auto operator<=>(const Position& other) const {
        int dist1 = std::abs(row) + std::abs(col);
        int dist2 = std::abs(other.row) + std::abs(other.col);
        return dist1 <=> dist2;
    }
};

Position p1{1, 1};  // 距离 = 2
Position p2{2, 2};  // 距离 = 4
p1 < p2  // true (2 < 4)
```

## 添加新成员的便利性

### 传统方式的问题

假设你要给 Position 添加楼层信息：

```cpp
struct Position {
    int row;
    int col;
    int floor;  // 新增

    // ❌ 需要修改所有 6 个运算符函数！
    bool operator==(const Position& other) const {
        return row == other.row &&
               col == other.col &&
               floor == other.floor;  // 新增这一行
    }

    bool operator<(const Position& other) const {
        if (row != other.row) return row < other.row;
        if (col != other.col) return col < other.col;
        return floor < other.floor;  // 新增这一行
    }

    // ... 还要修改其他 4 个函数
};
```

**痛点**：容易遗漏，容易出错

### 三路比较的优势

```cpp
struct Position {
    int row;
    int col;
    int floor;  // 新增

    auto operator<=>(const Position&) const = default;
    // ✓ 完全不需要修改！自动包含 floor 的比较
};
```

编译器自动处理：
```cpp
// 编译器生成的逻辑（伪代码）
if (row != other.row) return row <=> other.row;
if (col != other.col) return col <=> other.col;
return floor <=> other.floor;  // 自动加上
```

## 性能考量

### 零成本抽象

三路比较符合 C++ 的"零成本抽象"原则：

```cpp
// 手写版本
bool operator<(const Position& other) const {
    if (row != other.row) return row < other.row;
    return col < other.col;
}

// 三路比较版本
auto operator<=>(const Position&) const = default;
```

**编译后的机器码几乎相同**，甚至三路比较可能更优：

```asm
; 编译器可以优化为高效的分支预测代码
cmp    eax, edx      ; 比较 row
jne    .L1           ; 不等就跳转
cmp    ebx, ecx      ; 比较 col
.L1:
```

### 编译期优化

```cpp
constexpr Position p1{1, 2};
constexpr Position p2{3, 4};
constexpr bool result = p1 < p2;  // 编译期计算，零运行时开销
```

## 常见陷阱

### 陷阱1：忘记定义 `operator==`

某些情况下需要显式定义：

```cpp
struct Position {
    int row;
    int col;

    auto operator<=>(const Position&) const = default;
    // ✓ 这会自动生成 operator==
};

struct CustomPosition {
    int row;
    int col;

    // 自定义 <=>
    auto operator<=>(const CustomPosition& other) const {
        return row <=> other.row;  // 只比较 row
    }

    // ❌ 忘记定义 ==，会导致编译错误
    // ✓ 需要显式定义
    bool operator==(const CustomPosition& other) const = default;
};
```

**规则**：
- 如果 `operator<=>` 是 `= default`，自动生成 `operator==`
- 如果 `operator<=>` 是自定义的，需要手动定义 `operator==`

### 陷阱2：比较顺序问题

```cpp
struct Position {
    int col;   // 注意：col 在前
    int row;   // row 在后

    auto operator<=>(const Position&) const = default;
};

Position p1{5, 1};  // col=5, row=1
Position p2{10, 1}; // col=10, row=1

p1 < p2  // true，因为先比较 col (5 < 10)
// 这可能不是你想要的！通常我们期望先比较 row
```

**建议**：成员变量的声明顺序要与期望的比较优先级一致。

### 陷阱3：浮点数比较

```cpp
struct Point {
    double x;
    double y;

    auto operator<=>(const Point&) const = default;
    // 返回类型是 std::partial_ordering（因为 double）
};

Point p1{1.0, 2.0};
Point p2{NaN, 3.0};

auto result = p1 <=> p2;  // partial_ordering::unordered
// 无法判断大小关系
```

**建议**：浮点数比较要特别注意 NaN 和精度问题。

## 与其他 C++ 特性的配合

### 1. Concepts（概念）

```cpp
template <typename T>
concept Comparable = requires(T a, T b) {
    { a <=> b } -> std::convertible_to<std::strong_ordering>;
};

template <Comparable T>
void sort(std::vector<T>& vec) {
    // 只接受支持三路比较的类型
}
```

### 2. constexpr

```cpp
struct Position {
    int row;
    int col;

    constexpr auto operator<=>(const Position&) const = default;
};

constexpr Position p1{1, 2};
constexpr Position p2{3, 4};
static_assert(p1 < p2);  // 编译期断言
```

### 3. [[nodiscard]]

```cpp
struct Position {
    int row;
    int col;

    [[nodiscard]] auto operator<=>(const Position&) const = default;
};

Position p1{1, 2}, p2{3, 4};
p1 <=> p2;  // ⚠️ 警告：比较结果未使用
```

## 优势总结

| 特性 | 传统方式 | 三路比较 |
|------|----------|----------|
| **代码量** | 20+ 行 | 1 行 |
| **易出错** | 容易不一致 | 编译器保证一致 |
| **维护性** | 添加成员需改6处 | 自动处理 |
| **性能** | 手动优化 | 编译器优化 |
| **可读性** | 重复代码多 | 简洁明了 |
| **类型安全** | 需手动保证 | 编译器检查 |

## 最佳实践

### ✅ 推荐做法

```cpp
// 1. 对于简单的值类型，使用 = default
struct Position {
    int row;
    int col;
    auto operator<=>(const Position&) const = default;
};

// 2. 成员顺序与比较优先级一致
struct DateTime {
    int year;   // 最重要
    int month;
    int day;
    auto operator<=>(const DateTime&) const = default;
};

// 3. 使用 constexpr 支持编译期计算
struct Point {
    int x, y;
    constexpr auto operator<=>(const Point&) const = default;
};
```

### ❌ 避免的做法

```cpp
// 1. 不要混用新旧风格
struct Bad {
    auto operator<=>(const Bad&) const = default;
    bool operator<(const Bad&) const { /* ... */ }  // ❌ 冲突
};

// 2. 不要在有指针成员时使用 = default
struct Bad {
    int* data;
    auto operator<=>(const Bad&) const = default;  // ❌ 只比较指针地址
};

// 3. 浮点数要小心
struct Bad {
    double value;
    auto operator<=>(const Bad&) const = default;  // ⚠️ 注意 NaN
};
```

## 参考资料

- **C++20 标准文档**: [N4849 - expr.spaceship](https://timsong-cpp.github.io/cppwp/n4849/expr.spaceship)
- **cppreference**: [operator<=>](https://en.cppreference.com/w/cpp/language/default_comparisons)
- **项目代码位置**: `include/position.hpp:21`

## 总结

三路比较运算符 `<=>` 是 C++20 的革命性特性：

```cpp
auto operator<=>(const Position&) const = default;
```

**一行代码实现**：
- ✅ 自动生成 6 个比较运算符
- ✅ 字典序比较逻辑
- ✅ 零运行时开销
- ✅ 类型安全
- ✅ 易于维护

这体现了现代 C++ 的设计理念：**用更少的代码，做更多的事，且不牺牲性能**。
