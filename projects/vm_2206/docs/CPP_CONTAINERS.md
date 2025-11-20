# C++ 标准库容器对比

## 目录

- [map vs unordered_map](#map-vs-unordered_map)
    - [核心区别对比](#核心区别对比)
    - [底层实现](#底层实现)
    - [性能对比](#性能对比)
    - [元素顺序](#元素顺序)
    - [键的要求](#键的要求)
    - [项目中的实际使用](#项目中的实际使用)
    - [如何选择](#如何选择)
    - [性能测试](#性能测试)

---

## map vs unordered_map

### 核心区别对比

| 特性         | `std::map`           | `std::unordered_map`    |
|------------|----------------------|-------------------------|
| **底层实现**   | 红黑树 (Red-Black Tree) | 哈希表 (Hash Table)        |
| **元素顺序**   | 按键排序（默认升序）           | 无序                      |
| **查找复杂度**  | O(log n)             | O(1) 平均，O(n) 最坏         |
| **插入复杂度**  | O(log n)             | O(1) 平均，O(n) 最坏         |
| **删除复杂度**  | O(log n)             | O(1) 平均，O(n) 最坏         |
| **内存占用**   | 较小（只存储节点）            | 较大（需要哈希表 + 桶）           |
| **迭代器稳定性** | 插入/删除不影响其他迭代器        | rehash 时所有迭代器失效         |
| **键的要求**   | 必须支持 `<` 比较          | 必须支持 `==` 和 `std::hash` |
| **缓存友好性**  | 较差（指针跳转）             | 较好（连续内存桶）               |
| **适用场景**   | 需要有序、范围查询            | 只需快速查找                  |

---

### 底层实现

#### 1. `std::map` - 红黑树

红黑树是一种自平衡二叉搜索树，保证树的高度始终为 O(log n)。

```cpp
#include <map>
#include <iostream>

int main() {
    // map 内部是平衡二叉搜索树
    std::map<int, std::string> ordered_map;

    // 插入元素（无序插入）
    ordered_map[3] = "three";
    ordered_map[1] = "one";
    ordered_map[2] = "two";
    ordered_map[5] = "five";
    ordered_map[4] = "four";

    // 遍历时自动按键排序：1, 2, 3, 4, 5
    std::cout << "std::map (有序):\n";
    for (const auto& [key, value] : ordered_map) {
        std::cout << key << ": " << value << "\n";
    }

    return 0;
}
```

**输出：**

```
std::map (有序):
1: one
2: two
3: three
4: four
5: five
```

**红黑树特点：**

- ✅ 自动维护元素的有序性
- ✅ 查找、插入、删除都是 O(log n)
- ✅ 最坏情况性能稳定
- ❌ 每次插入需要旋转和重新着色
- ❌ 需要多次指针跳转，缓存不友好

**内部结构示意：**

```
        3 (黑)
       / \
      /   \
    1(红) 5(黑)
      \   /
     2(黑)4(红)
```

#### 2. `std::unordered_map` - 哈希表

哈希表使用哈希函数将键映射到桶（bucket），通过桶索引直接访问元素。

```cpp
#include <unordered_map>
#include <iostream>

int main() {
    // unordered_map 内部是哈希表
    std::unordered_map<int, std::string> hash_map;

    // 插入元素（无序插入）
    hash_map[3] = "three";
    hash_map[1] = "one";
    hash_map[2] = "two";
    hash_map[5] = "five";
    hash_map[4] = "four";

    // 遍历时顺序不确定
    std::cout << "std::unordered_map (无序):\n";
    for (const auto& [key, value] : hash_map) {
        std::cout << key << ": " << value << "\n";
    }

    return 0;
}
```

**输出（顺序不确定）：**

```
std::unordered_map (无序):
4: four
5: five
2: two
1: one
3: three
```

**哈希表特点：**

- ✅ 查找、插入、删除平均 O(1)
- ✅ 大数据量下性能优异
- ✅ 实现简单直接
- ❌ 最坏情况退化到 O(n)（哈希冲突）
- ❌ 需要额外的桶数组空间
- ❌ 不维护元素顺序

**内部结构示意：**

```
桶数组:
[0] -> nullptr
[1] -> {1, "one"}
[2] -> {2, "two"}
[3] -> {3, "three"}
[4] -> {4, "four"}
[5] -> {5, "five"}
[6] -> nullptr
[7] -> nullptr
```

---

### 性能对比

#### 1. 时间复杂度

| 操作    | `std::map`   | `std::unordered_map` |
|-------|--------------|----------------------|
| 查找    | O(log n)     | O(1) 平均，O(n) 最坏      |
| 插入    | O(log n)     | O(1) 平均，O(n) 最坏      |
| 删除    | O(log n)     | O(1) 平均，O(n) 最坏      |
| 遍历    | O(n)         | O(n + 桶数量)           |
| 最小/最大 | O(1)         | O(n)                 |
| 范围查询  | O(log n + k) | 不支持                  |

**示例：100 万元素的查找**

```cpp
#include <chrono>

// std::map: O(log n)
std::map<int, int> map;
for (int i = 0; i < 1000000; ++i) {
    map[i] = i;
}
// 查找需要约 log2(1000000) ≈ 20 次比较

// std::unordered_map: O(1)
std::unordered_map<int, int> umap;
for (int i = 0; i < 1000000; ++i) {
    umap[i] = i;
}
// 查找通常只需 1-2 次操作（哈希 + 桶内查找）
```

#### 2. 空间复杂度

**`std::map`:**

```cpp
// 每个节点包含：
// - 键 (key)
// - 值 (value)
// - 左子节点指针
// - 右子节点指针
// - 父节点指针
// - 颜色标记 (红/黑)

struct MapNode {
    Key key;           // 例如: 4 字节
    Value value;       // 例如: 4 字节
    Node* left;        // 8 字节
    Node* right;       // 8 字节
    Node* parent;      // 8 字节
    bool color;        // 1 字节
    // 总计: 约 33-40 字节（考虑对齐）
};

// 1000 个元素 ≈ 40 KB
```

**`std::unordered_map`:**

```cpp
// 需要两部分内存：
// 1. 桶数组（动态调整大小）
// 2. 链表节点（处理哈希冲突）

// 桶数组（假设负载因子 1.0）
std::vector<Node*> buckets(1000);  // 8000 字节

// 链表节点
struct HashNode {
    Key key;           // 4 字节
    Value value;       // 4 字节
    HashNode* next;    // 8 字节
    size_t hash;       // 8 字节（缓存哈希值）
    // 总计: 约 24 字节
};

// 1000 个元素 ≈ 32 KB（节点 + 桶数组）
// 但需要预留空间，实际可能 40-50 KB
```

**结论：**

- **小数据量**：内存差异不明显
- **大数据量**：`unordered_map` 通常占用更多内存
- **内存稳定性**：`map` 更可预测

#### 3. 实际性能测试

```cpp
#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>

template<typename Container>
void benchmark(const std::string& name, int n) {
    using namespace std::chrono;

    // 插入测试
    auto start = high_resolution_clock::now();
    Container container;
    for (int i = 0; i < n; ++i) {
        container[i] = i;
    }
    auto end = high_resolution_clock::now();
    auto insert_time = duration_cast<milliseconds>(end - start).count();

    // 查找测试
    start = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        auto it = container.find(i);
    }
    end = high_resolution_clock::now();
    auto find_time = duration_cast<milliseconds>(end - start).count();

    // 删除测试
    start = high_resolution_clock::now();
    for (int i = 0; i < n; i += 2) {
        container.erase(i);
    }
    end = high_resolution_clock::now();
    auto erase_time = duration_cast<milliseconds>(end - start).count();

    std::cout << name << " (n=" << n << "):\n";
    std::cout << "  插入: " << insert_time << "ms\n";
    std::cout << "  查找: " << find_time << "ms\n";
    std::cout << "  删除: " << erase_time << "ms\n\n";
}

int main() {
    benchmark<std::map<int, int>>("std::map", 100000);
    benchmark<std::unordered_map<int, int>>("std::unordered_map", 100000);
    return 0;
}
```

**典型结果（n=100,000）：**

```
std::map (n=100000):
  插入: 45ms
  查找: 35ms
  删除: 22ms

std::unordered_map (n=100000):
  插入: 25ms
  查找: 12ms
  删除: 8ms
```

**结论：**

- `unordered_map` 在所有操作上都更快（约 2-3 倍）
- 数据量越大，差异越明显

---

### 元素顺序

#### `std::map` - 自动排序

```cpp
#include <map>
#include <string>

int main() {
    std::map<std::string, int> word_count;

    // 无序插入
    word_count["dog"] = 3;
    word_count["apple"] = 1;
    word_count["zoo"] = 26;
    word_count["cat"] = 2;
    word_count["banana"] = 5;

    // 按字母顺序遍历（自动排序）
    std::cout << "按字母顺序:\n";
    for (const auto& [word, count] : word_count) {
        std::cout << word << ": " << count << "\n";
    }

    // 范围查询：找出 'b' 到 'd' 之间的单词
    auto start = word_count.lower_bound("b");
    auto end = word_count.upper_bound("d");
    std::cout << "\n'b' 到 'd' 之间:\n";
    for (auto it = start; it != end; ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    // 找最小和最大键
    std::cout << "\n最小: " << word_count.begin()->first << "\n";
    std::cout << "最大: " << word_count.rbegin()->first << "\n";

    return 0;
}
```

**输出：**

```
按字母顺序:
apple: 1
banana: 5
cat: 2
dog: 3
zoo: 26

'b' 到 'd' 之间:
banana: 5
cat: 2

最小: apple
最大: zoo
```

**适用场景：**

- ✅ 需要按键排序遍历
- ✅ 需要范围查询（`lower_bound`, `upper_bound`）
- ✅ 需要快速找到最小/最大键
- ✅ 需要有序输出

#### `std::unordered_map` - 无序

```cpp
#include <unordered_map>
#include <string>

int main() {
    std::unordered_map<std::string, int> word_count;

    // 无序插入
    word_count["dog"] = 3;
    word_count["apple"] = 1;
    word_count["zoo"] = 26;
    word_count["cat"] = 2;
    word_count["banana"] = 5;

    // 顺序不确定（取决于哈希值和桶位置）
    std::cout << "遍历顺序（不确定）:\n";
    for (const auto& [word, count] : word_count) {
        std::cout << word << ": " << count << "\n";
    }

    // 不支持范围查询
    // auto start = word_count.lower_bound("b");  // ❌ 编译错误

    return 0;
}
```

**输出（顺序不确定）：**

```
遍历顺序（不确定）:
banana: 5
zoo: 26
dog: 3
cat: 2
apple: 1
```

**适用场景：**

- ✅ 只需要快速查找，不关心顺序
- ✅ 频繁插入/删除
- ❌ 不支持范围查询
- ❌ 不能快速找最小/最大键

---

### 键的要求

#### `std::map` - 需要支持 `<` 比较运算符

```cpp
#include <map>
#include <string>

// 自定义类型作为键
struct Person {
    std::string name;
    int age;

    // 必须定义 < 运算符（用于排序）
    bool operator<(const Person& other) const {
        if (name != other.name) {
            return name < other.name;  // 先按姓名排序
        }
        return age < other.age;  // 姓名相同则按年龄排序
    }
};

int main() {
    std::map<Person, std::string> person_map;

    person_map[{"Alice", 30}] = "Engineer";
    person_map[{"Bob", 25}] = "Designer";
    person_map[{"Alice", 25}] = "Manager";

    // 按姓名和年龄排序
    for (const auto& [person, job] : person_map) {
        std::cout << person.name << " (" << person.age << "): " << job << "\n";
    }

    return 0;
}
```

**输出：**

```
Alice (25): Manager
Alice (30): Engineer
Bob (25): Designer
```

**或者使用自定义比较器：**

```cpp
// 自定义比较器（按年龄排序）
struct CompareByAge {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;
    }
};

std::map<Person, std::string, CompareByAge> person_map;
```

#### `std::unordered_map` - 需要哈希函数和 `==` 运算符

```cpp
#include <unordered_map>
#include <string>

struct Person {
    std::string name;
    int age;

    // 必须定义 == 运算符（用于比较键是否相等）
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// 必须为自定义类型提供哈希函数
namespace std {
    template<>
    struct hash<Person> {
        size_t operator()(const Person& p) const {
            // 组合 name 和 age 的哈希值
            size_t h1 = hash<string>()(p.name);
            size_t h2 = hash<int>()(p.age);
            return h1 ^ (h2 << 1);  // 简单的哈希组合
        }
    };
}

int main() {
    std::unordered_map<Person, std::string> person_map;

    person_map[{"Alice", 30}] = "Engineer";
    person_map[{"Bob", 25}] = "Designer";
    person_map[{"Alice", 25}] = "Manager";

    // 顺序不确定
    for (const auto& [person, job] : person_map) {
        std::cout << person.name << " (" << person.age << "): " << job << "\n";
    }

    return 0;
}
```

**或者使用 lambda 定义哈希函数：**

```cpp
auto hash_func = [](const Person& p) {
    return std::hash<std::string>()(p.name) ^ std::hash<int>()(p.age);
};

auto equal_func = [](const Person& a, const Person& b) {
    return a.name == b.name && a.age == b.age;
};

std::unordered_map<Person, std::string, decltype(hash_func), decltype(equal_func)>
    person_map(10, hash_func, equal_func);
```

---

### 项目中的实际使用

#### InstructionFactory 使用 `unordered_map`

**代码位置：** `include/InstructionFactory.h`

```cpp
class InstructionFactory
{
private:
    // 使用 unordered_map 存储指令对象
    std::unordered_map<OpCode, std::unique_ptr<IInstruction>> instructions_;
    //     ^^^^^^^^^^^^^ 为什么选择 unordered_map？

public:
    [[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const;
};
```

**实现：** `src/InstructionFactory.cpp`

```cpp
// 构造函数：初始化所有指令对象
InstructionFactory::InstructionFactory()
{
    instructions_.emplace(OpCode::READ, std::make_unique<ReadInstruction>());
    instructions_.emplace(OpCode::WRITE, std::make_unique<WriteInstruction>());
    instructions_.emplace(OpCode::LOAD, std::make_unique<LoadInstruction>());
    instructions_.emplace(OpCode::STORE, std::make_unique<StoreInstruction>());
    instructions_.emplace(OpCode::ADD, std::make_unique<AddInstruction>());
    instructions_.emplace(OpCode::SUB, std::make_unique<SubtractInstruction>());
    instructions_.emplace(OpCode::MUL, std::make_unique<MultiplyInstruction>());
    instructions_.emplace(OpCode::DIV, std::make_unique<DivideInstruction>());
    instructions_.emplace(OpCode::JMP, std::make_unique<BranchInstruction>());
    instructions_.emplace(OpCode::JMPNEG, std::make_unique<BranchNegInstruction>());
    instructions_.emplace(OpCode::JMPZERO, std::make_unique<BranchZeroInstruction>());
    instructions_.emplace(OpCode::HALT, std::make_unique<HaltInstruction>());
}

// 根据操作码获取指令对象
std::optional<IInstruction*> InstructionFactory::getInstruction(const OpCode opcode) const
{
    if (auto it = instructions_.find(opcode); it != instructions_.end())
    {
        return it->second.get();
    }
    return std::nullopt;
}
```

#### 为什么选择 `unordered_map`？

##### ✅ **选择理由：**

1. **只需要查找，不需要排序**
   ```cpp
   // 虚拟机执行指令时只需要快速查找
   auto instruction = factory.getInstruction(OpCode::ADD);

   // 不需要：
   // - 遍历所有指令
   // - 按操作码顺序排列
   // - 范围查询
   // - 找最小/最大操作码
   ```

2. **查找性能更优**
    - 虚拟机执行每条指令都需要查找指令对象
    - `unordered_map`: O(1) 平均
    - `map`: O(log n) = O(log 12) ≈ 3-4 次比较
    - 虽然只有 12 个指令，但优先选择更快的方案

3. **操作码天然适合哈希**
   ```cpp
   enum class OpCode : int {
       READ = 10,
       WRITE = 11,
       LOAD = 20,
       // ...
   };
   // int 类型的枚举，哈希函数简单高效
   // std::hash<OpCode> 自动工作（底层是 int）
   ```

4. **不需要有序遍历**
    - 工厂只在构造时插入指令
    - 运行时只进行查找操作
    - 从不遍历所有指令

5. **内存占用不是问题**
    - 只有 12 个指令对象
    - `unordered_map` 的额外内存开销可以忽略

##### ⚠️ **如果使用 `map` 会怎样？**

```cpp
// 假设使用 map
std::map<OpCode, std::unique_ptr<IInstruction>> instructions_;

// ✅ 功能正常
// ❌ 查找稍慢：O(log 12) ≈ 3-4 次比较
// ❌ 插入时需要维护红黑树（虽然只插入一次）
// ❌ 内存占用稍大（红黑树节点需要3个指针）
// ❌ 没有实际好处（不需要排序功能）
```

##### 📊 **性能对比（12 个元素）：**

| 操作 | `std::map`       | `std::unordered_map`   | 差异      |
|----|------------------|------------------------|---------|
| 查找 | ~4 次比较           | ~1 次哈希 + 1 次比较         | 2-3x 更快 |
| 插入 | ~4 次比较 + 树旋转     | ~1 次哈希                 | 3-4x 更快 |
| 内存 | 12 × 40 = 480 字节 | 12 × 24 + 桶数组 ≈ 500 字节 | 相差不大    |

**结论：** `InstructionFactory` 使用 `unordered_map` 是**正确的选择**，充分利用了哈希表的高效查找特性。

---

### 如何选择

#### 决策流程图

```
需要使用关联容器？
    │
    ├─ 是 → 需要有序遍历/范围查询？
    │       │
    │       ├─ 是 → 使用 std::map
    │       │       - 按键排序输出
    │       │       - lower_bound/upper_bound
    │       │       - 找最小/最大键
    │       │
    │       └─ 否 → 只需要快速查找？
    │               │
    │               ├─ 是 → 使用 std::unordered_map
    │               │       - O(1) 查找
    │               │       - 频繁插入/删除
    │               │       - 大数据量
    │               │
    │               └─ 键类型难以哈希？
    │                       │
    │                       ├─ 是 → 使用 std::map
    │                       └─ 否 → 使用 std::unordered_map
    │
    └─ 否 → 考虑其他容器（vector, list, set, etc.）
```

#### 选择 `std::map` 的场景

| 场景        | 示例代码                                                                                                               | 说明          |
|-----------|--------------------------------------------------------------------------------------------------------------------|-------------|
| **有序遍历**  | ```cpp<br>std::map<std::string, int> word_count;<br>for (auto& [word, count] : word_count) {<br>  // 按字母顺序<br>}``` | 需要按键排序输出    |
| **范围查询**  | ```cpp<br>auto start = map.lower_bound(10);<br>auto end = map.upper_bound(20);<br>// 查找 [10, 20] 范围内的元素```         | 需要查找某个范围的键  |
| **最小/最大** | ```cpp<br>auto min = map.begin()->first;<br>auto max = map.rbegin()->first;```                                     | 需要快速找最小/最大键 |
| **稳定性能**  | 数据量不确定，需要保证最坏情况性能                                                                                                  | O(log n) 稳定 |
| **小数据量**  | 元素少于 100 个                                                                                                         | 性能差异不明显     |

**具体示例：**

```cpp
// 1. 事件时间线（按时间排序）
std::map<int, std::string> events;
events[2020] = "Event A";
events[2022] = "Event B";
events[2024] = "Event C";

// 按时间顺序输出
for (const auto& [year, event] : events) {
    std::cout << year << ": " << event << "\n";
}

// 2. 排行榜（按分数排序）
std::map<int, std::string, std::greater<>> leaderboard;
leaderboard[95] = "Alice";
leaderboard[87] = "Bob";
leaderboard[92] = "Charlie";

// 按分数从高到低输出
for (const auto& [score, name] : leaderboard) {
    std::cout << name << ": " << score << "\n";
}

// 3. 范围查询（查找价格区间的商品）
std::map<double, std::string> products;
products[19.99] = "Book";
products[49.99] = "Shirt";
products[99.99] = "Shoes";

// 查找价格在 [20, 80] 之间的商品
auto start = products.lower_bound(20.0);
auto end = products.upper_bound(80.0);
for (auto it = start; it != end; ++it) {
    std::cout << it->second << ": $" << it->first << "\n";
}
```

#### 选择 `std::unordered_map` 的场景

| 场景          | 示例代码                                                                                                | 说明                    |
|-------------|-----------------------------------------------------------------------------------------------------|-----------------------|
| **快速查找**    | ```cpp<br>std::unordered_map<std::string, User> users;<br>auto user = users["user123"];  // O(1)``` | 只需要快速查找，不关心顺序         |
| **频繁插入/删除** | ```cpp<br>cache[key] = value;  // O(1)<br>cache.erase(key);     // O(1)```                          | 缓存系统、会话管理             |
| **大数据量**    | 百万级以上的数据                                                                                            | O(1) vs O(log n) 差异明显 |
| **哈希表语义**   | ```cpp<br>std::unordered_map<int, int> hash_table;```                                               | 逻辑上就是哈希表              |

**具体示例：**

```cpp
// 1. 用户缓存（快速查找）
std::unordered_map<std::string, User> user_cache;
user_cache["user123"] = User{"Alice", 30};
auto user = user_cache.find("user123");  // O(1)

// 2. 单词频率统计
std::unordered_map<std::string, int> word_count;
for (const auto& word : words) {
    word_count[word]++;  // O(1)
}

// 3. 会话管理（频繁插入/删除）
std::unordered_map<int, Session> sessions;
sessions[session_id] = Session{...};  // O(1) 插入
sessions.erase(session_id);           // O(1) 删除

// 4. 配置管理
std::unordered_map<std::string, std::string> config;
config["host"] = "localhost";
config["port"] = "8080";
auto host = config["host"];  // O(1) 查找

// 5. 图的邻接表
std::unordered_map<int, std::vector<int>> graph;
graph[1] = {2, 3, 4};
graph[2] = {1, 5};
```

#### 特殊情况

**1. 键类型难以哈希**

```cpp
// 复杂的自定义类型
struct ComplexKey {
    std::vector<int> data;
    std::map<std::string, int> meta;
    // 难以定义高效的哈希函数
};

// 建议使用 map（只需要 < 运算符）
std::map<ComplexKey, Value> container;
```

**2. 需要自定义排序**

```cpp
// 按绝对值排序
auto cmp = [](int a, int b) { return std::abs(a) < std::abs(b); };
std::map<int, std::string, decltype(cmp)> map(cmp);

map[-5] = "minus five";
map[3] = "three";
map[-2] = "minus two";

// 输出：-2, 3, -5
```

**3. 内存受限环境**

```cpp
// map 内存占用更小且稳定
std::map<Key, Value> map;  // 每个节点约 40 字节

// unordered_map 需要额外的桶数组
std::unordered_map<Key, Value> umap;  // 可能需要更多内存
```

---

### 性能测试

#### 完整的基准测试代码

```cpp
#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>
#include <iomanip>

template<typename Container>
struct BenchmarkResult {
    std::string name;
    long long insert_time;
    long long find_time;
    long long erase_time;
    size_t memory_estimate;
};

template<typename Container>
BenchmarkResult<Container> benchmark(const std::string& name, int n) {
    using namespace std::chrono;
    BenchmarkResult<Container> result;
    result.name = name;

    // 生成随机数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, n * 10);

    std::vector<int> keys(n);
    for (int i = 0; i < n; ++i) {
        keys[i] = dis(gen);
    }

    // 插入测试
    auto start = high_resolution_clock::now();
    Container container;
    for (int key : keys) {
        container[key] = key;
    }
    auto end = high_resolution_clock::now();
    result.insert_time = duration_cast<microseconds>(end - start).count();

    // 查找测试
    start = high_resolution_clock::now();
    for (int key : keys) {
        auto it = container.find(key);
    }
    end = high_resolution_clock::now();
    result.find_time = duration_cast<microseconds>(end - start).count();

    // 删除测试（删除一半）
    start = high_resolution_clock::now();
    for (int i = 0; i < n; i += 2) {
        container.erase(keys[i]);
    }
    end = high_resolution_clock::now();
    result.erase_time = duration_cast<microseconds>(end - start).count();

    // 内存估算（简化）
    result.memory_estimate = container.size() * 40;  // 粗略估计

    return result;
}

template<typename Container>
void print_result(const BenchmarkResult<Container>& result) {
    std::cout << std::left << std::setw(20) << result.name
              << " Insert: " << std::setw(10) << result.insert_time << "μs"
              << " Find: " << std::setw(10) << result.find_time << "μs"
              << " Erase: " << std::setw(10) << result.erase_time << "μs"
              << " Memory: ~" << result.memory_estimate / 1024 << "KB\n";
}

int main() {
    std::cout << "=== Map vs Unordered_Map 性能对比 ===\n\n";

    for (int n : {100, 1000, 10000, 100000}) {
        std::cout << "数据量: " << n << "\n";
        std::cout << std::string(80, '-') << "\n";

        auto map_result = benchmark<std::map<int, int>>("std::map", n);
        auto umap_result = benchmark<std::unordered_map<int, int>>("std::unordered_map", n);

        print_result(map_result);
        print_result(umap_result);

        std::cout << "\n加速比:\n";
        std::cout << "  Insert: " << std::fixed << std::setprecision(2)
                  << (double)map_result.insert_time / umap_result.insert_time << "x\n";
        std::cout << "  Find:   " << std::fixed << std::setprecision(2)
                  << (double)map_result.find_time / umap_result.find_time << "x\n";
        std::cout << "  Erase:  " << std::fixed << std::setprecision(2)
                  << (double)map_result.erase_time / umap_result.erase_time << "x\n\n";
    }

    return 0;
}
```

#### 典型测试结果

```
=== Map vs Unordered_Map 性能对比 ===

数据量: 100
--------------------------------------------------------------------------------
std::map          Insert: 45         μs Find: 28         μs Erase: 15         μs Memory: ~2KB
std::unordered_map Insert: 32         μs Find: 18         μs Erase: 12         μs Memory: ~2KB

加速比:
  Insert: 1.41x
  Find:   1.56x
  Erase:  1.25x

数据量: 1000
--------------------------------------------------------------------------------
std::map          Insert: 520        μs Find: 380        μs Erase: 210        μs Memory: ~20KB
std::unordered_map Insert: 280        μs Find: 150        μs Erase: 95         μs Memory: ~20KB

加速比:
  Insert: 1.86x
  Find:   2.53x
  Erase:  2.21x

数据量: 10000
--------------------------------------------------------------------------------
std::map          Insert: 7800       μs Find: 5200       μs Erase: 3100       μs Memory: ~200KB
std::unordered_map Insert: 3200       μs Find: 1500       μs Erase: 980        μs Memory: ~200KB

加速比:
  Insert: 2.44x
  Find:   3.47x
  Erase:  3.16x

数据量: 100000
--------------------------------------------------------------------------------
std::map          Insert: 125000     μs Find: 85000      μs Erase: 52000      μs Memory: ~2000KB
std::unordered_map Insert: 42000      μs Find: 18000      μs Erase: 12000      μs Memory: ~2000KB

加速比:
  Insert: 2.98x
  Find:   4.72x
  Erase:  4.33x
```

**结论：**

1. 数据量越大，`unordered_map` 的优势越明显
2. 查找操作的加速比最高（4-5倍）
3. 小数据量（< 100）时差异不大
4. 内存占用相差不大

---

## 总结

### 快速决策表

| 需求            | 推荐              | 理由                      |
|---------------|-----------------|-------------------------|
| 只需要快速查找       | `unordered_map` | O(1) 平均                 |
| 需要有序遍历        | `map`           | 自动排序                    |
| 需要范围查询        | `map`           | lower_bound/upper_bound |
| 大数据量（> 10000） | `unordered_map` | 性能优势明显                  |
| 频繁插入/删除       | `unordered_map` | O(1) 平均                 |
| 需要稳定性能        | `map`           | O(log n) 最坏             |
| 键类型难以哈希       | `map`           | 只需要 <                   |
| 小数据量（< 100）   | 都可以             | 性能差异小                   |
| 内存受限          | `map`           | 更稳定                     |

### 项目中的使用建议

基于 `InstructionFactory` 的实践：

1. **当键是整数/枚举/字符串时**
    - 优先考虑 `unordered_map`（哈希效率高）

2. **当只需要查找操作时**
    - 使用 `unordered_map`（O(1) vs O(log n)）

3. **当需要有序或范围查询时**
    - 必须使用 `map`

4. **性能优化原则**
    - 先选择合适的容器
    - 不要过早优化
    - 用基准测试验证

---

**参考代码位置：**

- `include/InstructionFactory.h:34` - unordered_map 声明
- `src/InstructionFactory.cpp:11-25` - 初始化代码
- `src/InstructionFactory.cpp:35-42` - 查找实现
