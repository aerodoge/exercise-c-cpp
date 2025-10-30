# Knight Tour

骑士之旅

## in-place initialization
```c++
class KnightTour
{
public:
    // ...
private:
    int _board[BOARD_SIZE][BOARD_SIZE]{};
    Knight _knight{0,0};
};
```
### 可以这样初始化的情况
- 基本数据类型（如int、double等）
- 数组（用{}会将所有元素初始化为默认值）
- 有构造函数的类对象
- 聚合类型（struct、简单类等）

### 不能这样初始化的情况
- 引用类型：必须在构造函数初始化列表中初始化
- const成员变量：必须在构造函数初始化列表中初始化
- 没有默认构造函数的类：如果类只有带参构造函数且参数不匹配

```c++
class Example {
private:
    int& _ref;           // 错误：引用不能就地初始化
    const int _const{5}; // 可以，但更常见的是在初始化列表中
    SomeClass _obj;      // 错误：如果SomeClass没有默认构造函数
};
```

## 就地初始化（in-place initialization）和统一初始化（uniform initialization）

### 就地初始化（in-place initialization）

指在声明成员变量的地方直接初始化，而不是在构造函数中初始化：
```c++
class Example {
private:
int _value = 42;           // 就地初始化
std::vector<int> _vec{1,2,3}; // 就地初始化
};
```

### 统一初始化（uniform initialization）

指C++11引入的使用大括号{}进行初始化的语法，可以用于各种场景：

```c++
// 1. 变量初始化
int x{42};
int arr[]{1, 2, 3};

// 2. 对象初始化
std::vector<int> vec{1, 2, 3};
std::string str{"hello"};

// 3. 函数参数
func({1, 2, 3});

// 4. 返回值
return {x, y};
```

### 关系和区别
- 包含关系：就地初始化可以使用统一初始化语法
    ```c++
    class Example {
    private:
        int _a = 10;      // 就地初始化，使用=语法
        int _b{20};       // 就地初始化，使用统一初始化语法
        int _c[3]{1,2,3}; // 就地初始化，使用统一初始化语法
    };
    ```
- 范围不同
   - 就地初始化：仅限于成员变量声明时的初始化
   - 统一初始化：可用于任何初始化场景
- 目的不同
   - 就地初始化：简化成员变量初始化，避免在每个构造函数中重复代码
   - 统一初始化：提供一致的初始化语法，防止类型收窄，解决"最令人头疼的解析"问题

## 统一初始化的三个主要优势

### 提供一致的初始化语法
C++11之前，不同类型有不同的初始化方式，容易混乱：
```c++
// C++11之前的各种初始化方式
int a = 5;                    // 赋值初始化
int b(10);                    // 直接初始化
int arr[] = {1, 2, 3};        // 聚合初始化
std::vector<int> v(5, 10);    // 构造函数初始化

// C++11统一初始化：都用{}
int a{5};
int b{10};
int arr[]{1, 2, 3};
std::vector<int> v{5, 10};    // 注意：这个含义变了！
```

### 防止类型收窄
传统初始化允许隐式类型转换，可能丢失精度：
```c++
// 传统初始化：允许收窄转换
int x = 3.14;     // 编译通过，x = 3（丢失小数部分）
char c = 1000;    // 编译通过，但可能溢出

// 统一初始化：禁止收窄转换
int x{3.14};      // 编译错误！不允许double到int的收窄
char c{1000};     // 编译错误！1000超出char范围
```

### 解决"最令人头疼的解析"（Most Vexing Parse）
编译器有时无法区分对象声明和函数声明
```c++
class Timer {
public:
    Timer();
    Timer(int seconds);
};

// 问题：这是声明对象还是声明函数？
Timer t();         // 实际上声明了一个返回Timer的无参函数！
Timer t2(Timer()); // 也是函数声明，参数是返回Timer的函数指针

// 解决方案1：加括号
Timer t3((Timer())); // 对象声明

// 解决方案2：统一初始化
Timer t4{};        // 明确的对象声明
Timer t5{Timer{}}; // 明确的对象声明
```
### Example
```c++
  #include <fstream>
  #include <iterator>
  #include <algorithm>
  #include <vector>

  std::ifstream file("data.txt");

  // 想要：用文件内容初始化vector
  // 写成：
  std::vector<std::string> words(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>());

  // 问题：编译器认为这是函数声明！
  // words是一个函数，返回vector，参数是两个函数指针

  // 解决方案：统一初始化
  std::vector<std::string> words{std::istream_iterator<std::string>(file), std::istream_iterator<std::string>()};
```



## clang format
`clang-format -i knight_tour.h`

- 参数说明：
  - -i : 就地修改文件（in-place）
  - 如果没有.clang-format配置文件，会使用默认的LLVM风格

- 如果要指定特定风格：
  - `clang-format -i --style=Google knight_tour.h`

批量格式化多个文件：
  - `clang-format -i *.cpp *.h`






