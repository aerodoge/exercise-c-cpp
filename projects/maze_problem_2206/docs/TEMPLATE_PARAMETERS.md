# C++ 模板参数详解：类型参数 vs 非类型参数

## 核心概念

C++ 模板参数有**两大类**：

### 1. 类型参数 (Type Parameters)
使用 `typename` 或 `class` 关键字声明

### 2. 非类型参数 (Non-Type Parameters)
使用**具体类型**声明（如 `int`, `std::size_t`, `char*` 等）

---

## 一、类型参数 (Type Parameters)

### 语法

```cpp
template<typename T>        // ✅ 推荐写法
template<class T>           // ✅ 等价写法（不推荐，容易混淆）
```

**重点**：`typename` 和 `class` 在声明模板参数时**完全等价**，但习惯上用 `typename`。

### 示例

```cpp
// T 是类型参数，可以被任何类型替换
template<typename T>
class Container {
    T value_;
public:
    void set(T val) { value_ = val; }
    T get() const { return value_; }
};

// 使用：T 被替换为具体类型
Container<int> c1;          // T = int
Container<double> c2;       // T = double
Container<std::string> c3;  // T = std::string
Container<Position> c4;     // T = 自定义类型
```

### 特点

- ✅ 可以是任何**类型**：内置类型、自定义类、指针、引用等
- ✅ 在编译时被替换为具体类型
- ✅ 不需要是编译期常量

---

## 二、非类型参数 (Non-Type Parameters)

### 语法

```cpp
template<std::size_t N>     // N 是非类型参数（整数）
template<int Width>         // Width 是非类型参数（整数）
template<char* Name>        // Name 是非类型参数（指针）
template<bool Flag>         // Flag 是非类型参数（布尔值）
```

**重点**：这里 `std::size_t`, `int`, `char*`, `bool` 是**具体类型**，不是关键字！

### 示例

```cpp
// N 是非类型参数，必须是编译期常量整数
template<std::size_t N>
class Array {
    int data_[N];  // 数组大小在编译期确定
public:
    constexpr std::size_t size() const { return N; }
};

// 使用：N 被替换为具体的数值
Array<10> arr1;   // N = 10，创建 int[10]
Array<20> arr2;   // N = 20，创建 int[20]
Array<100> arr3;  // N = 100，创建 int[100]

// ❌ 错误：必须是编译期常量
int n = 10;
Array<n> arr4;    // 编译错误！n 不是编译期常量
```

### 允许的非类型参数

| 类型 | 示例 | 用途 |
|------|------|------|
| **整数类型** | `int`, `std::size_t`, `unsigned` | 数组大小、维度、配置值 |
| **枚举类型** | `enum Color` | 编译期选项 |
| **指针类型** | `int*`, `char*` | 指向全局对象 |
| **引用类型** | `int&` | 引用全局对象 |
| **nullptr_t** | `std::nullptr_t` | 空指针 |
| **浮点类型** | `double` (C++20) | 物理常量 |

### 特点

- ✅ 必须是**编译期常量**
- ✅ 可以用于编译期计算
- ✅ 零运行时开销
- ❌ 不能是运行时变量

---

## 三、对比示例

### 示例1：迷宫生成器

```cpp
// ❌ 错误理解
template<typename N>  // N 是类型参数，不是数值！
class MazeGenerator {
    // 无法使用：N 不是数值，是类型
    // std::array<char, N> _grid;  // 编译错误！
};

// ✅ 正确写法：非类型参数
template<std::size_t N>  // N 是非类型参数（数值）
class MazeGenerator {
    std::array<std::array<char, N>, N> _grid;  // ✅ N 是编译期常量
};

// 使用
MazeGenerator<12> gen;  // N = 12（数值）
```

### 示例2：混合使用

```cpp
// 同时使用类型参数和非类型参数
template<typename T, std::size_t N>
class FixedArray {
    T data_[N];  // T 是类型，N 是大小
public:
    T& operator[](std::size_t i) { return data_[i]; }
    constexpr std::size_t size() const { return N; }
};

// 使用
FixedArray<int, 10> arr1;      // T=int, N=10
FixedArray<double, 20> arr2;   // T=double, N=20
FixedArray<Position, 5> arr3;  // T=Position, N=5
```

### 示例3：std::array 的实现

```cpp
// 标准库中 std::array 的简化实现
namespace std {
    template<typename T, std::size_t N>
    //       ^^^^^^^^      ^^^^^^^^^^^
    //       类型参数      非类型参数
    struct array {
        T elements_[N];

        constexpr std::size_t size() const noexcept {
            return N;  // N 在编译期已知
        }
    };
}

// 使用
std::array<int, 100> arr;  // T=int, N=100
//         ^^^  ^^^
//         类型  数值
```

---

## 四、深入理解：为什么可以用具体类型？

### typename 的真正含义

```cpp
template<typename T>
//       ^^^^^^^^
// 意思是："T 是一个类型参数，将被某个类型替换"
```

- `typename` **不是类型**，而是一个**关键字**，用于声明"T 是类型参数"
- 类似于函数参数的 `void func(int x)` 中的 `int` 是 x 的类型声明

### 非类型参数的原理

```cpp
template<std::size_t N>
//       ^^^^^^^^^^^
// 意思是："N 是一个 std::size_t 类型的编译期常量参数"
```

- `std::size_t` **是一个类型**（无符号整数类型）
- 表示 N 必须是一个 `std::size_t` 类型的编译期常量

### 类比：函数参数

```cpp
// 函数参数
void func(int x, double y, std::string s) {
//        ^^^    ^^^^^^    ^^^^^^^^^^^^
//        具体类型，不是关键字
}

// 模板参数（类似）
template<typename T, std::size_t N, int Value>
//       ^^^^^^^^    ^^^^^^^^^^^    ^^^
//       关键字       具体类型       具体类型
class MyClass { };
```

---

## 五、完整对比表

| 特性 | 类型参数 | 非类型参数 |
|------|----------|------------|
| **声明方式** | `template<typename T>` | `template<std::size_t N>` |
| **替换内容** | 类型（int, double等） | 编译期常量值（10, 20等） |
| **使用示例** | `Container<int>` | `Array<10>` |
| **必须是编译期常量** | ❌ 否 | ✅ 是 |
| **可以是运行时值** | N/A | ❌ 否 |
| **用于类型声明** | ✅ 是 | ❌ 否 |
| **用于常量计算** | ❌ 否 | ✅ 是 |
| **示例** | `T value_;` | `int arr[N];` |

---

## 六、实战示例：本项目中的使用

### 迷宫生成器

```cpp
// include/maze_generator.hpp
template<std::size_t N>
//       ^^^^^^^^^^^ 具体类型（非类型参数）
class MazeGenerator {
private:
    std::array<std::array<char, N>, N> _grid;
    //                          ^   ^
    //                  N 是编译期常量，可用于数组大小

public:
    [[nodiscard]] constexpr bool is_in_bounds(int row, int col) const noexcept {
        return row >= 0 && row < static_cast<int>(N);
        //                                          ^ 编译期常量
    }
};

// main.cpp
MazeGenerator<12> generator;
//            ^^ 传入数值 12
```

### 迷宫求解器

```cpp
// include/maze.hpp
template<std::size_t N>
class Maze {
    std::array<std::array<char, N>, N> _grid;
    // ...
};

// main.cpp
Maze<12> maze(mazeGrid, start, end);
//   ^^ 传入数值 12
```

---

## 七、常见误区

### ❌ 误区1：把 typename 当作类型

```cpp
// 错误理解
template<typename N>  // N 是类型参数，不是数值！
class Array {
    int data_[N];  // ❌ 错误！N 是类型，不是数值
};

Array<10> arr;  // ❌ 错误！10 不是类型
```

**正确理解**：
- `typename N` 表示 N 是一个**类型参数**，应该被 `int`, `double` 等类型替换
- 不能用于数组大小

### ❌ 误区2：非类型参数可以是运行时变量

```cpp
template<std::size_t N>
class Array { /* ... */ };

// 错误
int n;
std::cin >> n;  // 运行时输入
Array<n> arr;   // ❌ 编译错误！n 不是编译期常量
```

**正确用法**：
```cpp
constexpr std::size_t n = 10;  // 编译期常量
Array<n> arr;  // ✅ 正确

// 或直接用字面量
Array<10> arr;  // ✅ 正确
```

### ❌ 误区3：typename 和 class 有区别

```cpp
// 这两种写法完全等价
template<typename T>  // ✅ 推荐
class Container { };

template<class T>     // ✅ 等价（但不推荐）
class Container { };
```

**注意**：
- 在模板参数声明中，`typename` 和 `class` 等价
- 但 `typename` 还有其他用途（如依赖类型名称消歧义）
- 现代C++推荐使用 `typename` 避免混淆

---

## 八、高级用法

### 1. 多个非类型参数

```cpp
template<std::size_t Rows, std::size_t Cols>
class Matrix {
    double data_[Rows][Cols];
public:
    constexpr std::size_t rows() const { return Rows; }
    constexpr std::size_t cols() const { return Cols; }
};

Matrix<3, 4> mat;  // 3×4 矩阵
```

### 2. 混合参数

```cpp
template<typename T, std::size_t N, int DefaultValue = 0>
//       ^^^^^^^^^   ^^^^^^^^^^^    ^^^
//       类型参数    非类型参数     带默认值的非类型参数
class Array {
    T data_[N];
public:
    Array() {
        std::fill_n(data_, N, static_cast<T>(DefaultValue));
    }
};

Array<int, 10> arr1;      // T=int, N=10, DefaultValue=0
Array<int, 10, 42> arr2;  // T=int, N=10, DefaultValue=42
```

### 3. 自动推导（C++17）

```cpp
template<typename T, std::size_t N>
class Array {
    T data_[N];
public:
    Array(const T (&arr)[N]) {  // 从C数组构造
        std::copy(arr, arr + N, data_);
    }
};

// C++17 类模板参数推导
int arr[] = {1, 2, 3, 4, 5};
Array a(arr);  // 自动推导为 Array<int, 5>
//    ^ 不需要写 Array<int, 5>
```

---

## 总结

### 关键要点

1. **typename/class 是关键字**，用于声明**类型参数**
   ```cpp
   template<typename T>  // T 是类型参数
   ```

2. **具体类型（如 int, std::size_t）** 用于声明**非类型参数**
   ```cpp
   template<std::size_t N>  // N 是非类型参数（数值）
   ```

3. **本项目使用的是非类型参数**
   ```cpp
   template<std::size_t N>
   class MazeGenerator { };

   MazeGenerator<12> gen;  // N = 12（数值）
   ```

### 记忆口诀

```
typename/class → 类型参数 → 被类型替换 → Container<int>
具体类型       → 非类型参数 → 被数值替换 → Array<10>
```

### 选择指南

| 需求 | 使用 | 示例 |
|------|------|------|
| 需要任意类型 | 类型参数 | `template<typename T>` |
| 需要编译期常量 | 非类型参数 | `template<std::size_t N>` |
| 数组/容器大小 | 非类型参数 | `std::array<T, N>` |
| 泛型容器 | 类型参数 | `std::vector<T>` |
| 编译期配置 | 非类型参数 | `template<bool Debug>` |

现在你完全理解了！`template<std::size_t N>` 中的 `std::size_t` 确实是一个**具体类型**，用于声明非类型参数。
