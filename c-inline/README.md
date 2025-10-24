
## inline

inline 建议编译器将函数内联展开，减少函数调用开销。

### 内联后
```c
// 普通调用
call add            ; 跳转到函数
                    ; 保存寄存器、传参、返回...

// 内联后
mov eax, 3
add eax, 4          ; 直接计算，无函数调用开销
```

### 内联的重要性
```c
// inline 只是建议，编译器可以忽略
inline void complex_function(void) {
    // 复杂的代码...
    // 编译器可能拒绝内联
}

// 适合内联的函数：
inline int max(int a, int b) {
    return a > b ? a : b;
}

inline int square(int x) {
    return x * x;
}
```

### static inline（常见组合）
```c
// 推荐的内联函数写法（在头文件中）
static inline int fast_add(int a, int b) {
    return a + b;
}
```

### 什么时候用inline
适合内联：
- 函数体很小（1-3行）
- 频繁调用
- 性能关键路径

不适合内联：
- 函数体很大（会增加代码体积）
- 包含循环、递归
- 不常调用的函数

```c
// 好的inline使用
static inline int is_power_of_two(unsigned int x) {
    return x && !(x & (x - 1));
}

// 不好的inline使用
inline void process_large_data(int *data, int size) {
    for (int i = 0; i < size; i++) {
        // 大量代码...
    }
    // 内联会导致代码膨胀
}
```

## 链接错误
在C语言中，单独使用inline关键字声明函数时，编译器可能选择： 
- 内联展开：将函数调用替换为函数体代码
- 不生成独立符号：如果编译器认为函数会被完全内联，就不会为链接器生成该函数的符号定义

当链接器需要该符号但找不到时，就会报Undefined symbols错误。

### 两种解决方案对比

### 方案1：添加extern声明
```c
inline int add_inline(int a, int b) { return a + b; }
extern int add_inline(int a, int b);
```

原理：强制编译器生成一个外部可见的函数定义
优点：函数可以被其他编译单元调用
缺点：可能产生重复定义问题（如果多个文件都包含此代码）

### 方案2：使用static inline
```c
static inline int add_inline(int a, int b) { return a + b; }
```

原理：static限制函数作用域在当前编译单元，确保有本地定义
优点：
- 标准做法，避免链接冲突
- 编译器优化更好
- 不会有重复定义问题

缺点：函数仅在当前文件内可用

### 推荐

static inline是更好的选择，这是C语言中内联函数的标准写法。

## 编译器的实际行为

使用单独的inline时：
- 选择内联展开：函数调用被替换为函数体代码 → 确实没有符号生成
- 选择不内联：编译器仍然需要函数定义，但 inline 关键字暗示这个定义可能在其他地方 → 可能不生成符号

### 关键问题

inline的语义是"建议内联，但不保证生成独立符号"。编译器可能：
- 内联所有调用 → 不生成符号
- 部分内联 → 仍需要符号，但可能不生成
- 完全不内联 → 需要符号，但inline让编译器认为定义在别处

### 正确理解
```
inline int func();  // 声明：暗示定义在别处
inline int func() { return 1; }  // 定义：但编译器可能不为链接器生成符号

```
所以问题的根本是：inline函数定义不保证为链接器生成可用的符号，无论是否内联。

这就是为什么需要static inline（保证本地符号）或extern声明（强制生成外部符号）。









