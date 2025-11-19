# 设计模式深入分析

## 目录

- [单例模式 (Singleton Pattern)](#单例模式-singleton-pattern)
  - [InstructionFactory 中的应用](#instructionfactory-中的应用)
  - [使用单例模式的原因](#使用单例模式的原因)
  - [单例模式适用场景](#单例模式适用场景)
  - [单例模式的优缺点](#单例模式的优缺点)
  - [最佳实践](#最佳实践)

---

## 单例模式 (Singleton Pattern)

### InstructionFactory 中的应用

#### 代码实现

**头文件** (`include/InstructionFactory.h`):

```cpp
class InstructionFactory
{
private:
    // 指令映射表：操作码 -> 指令对象
    std::unordered_map<OpCode, std::unique_ptr<IInstruction>> instructions_;

    /**
     * @brief 私有构造函数（Singleton 模式）
     * 在构造时创建所有指令对象
     */
    InstructionFactory();

public:
    /**
     * @brief 获取工厂单例实例
     */
    static InstructionFactory& getInstance();

    // 删除拷贝和移动构造/赋值函数，确保单例唯一性
    InstructionFactory(const InstructionFactory&) = delete;
    InstructionFactory& operator=(const InstructionFactory&) = delete;
    InstructionFactory(InstructionFactory&&) = delete;
    InstructionFactory& operator=(InstructionFactory&&) = delete;

    /**
     * @brief 根据操作码获取对应的指令对象
     */
    [[nodiscard]] std::optional<IInstruction*> getInstruction(OpCode opcode) const;
};
```

**实现文件** (`src/InstructionFactory.cpp`):

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

// 获取单例实例
InstructionFactory& InstructionFactory::getInstance()
{
    static InstructionFactory instance; // 静态局部变量，保证线程安全的单例
    return instance;
}

// 根据操作码获取指令对象
std::optional<IInstruction*> InstructionFactory::getInstruction(const OpCode opcode) const
{
    if (auto it = instructions_.find(opcode); it != instructions_.end())
    {
        return it->second.get(); // 返回 unique_ptr 管理的裸指针
    }
    return std::nullopt; // 操作码无效时返回空
}
```

### 使用单例模式的原因

#### 1. **资源复用 - 避免重复创建**

`InstructionFactory` 在构造函数中创建了 **12 个指令对象**（READ、WRITE、LOAD、STORE、ADD 等），这些对象在整个程序生命周期中被**复用**。

```cpp
// 构造函数只执行一次，创建所有指令对象
InstructionFactory::InstructionFactory() {
    instructions_.emplace(OpCode::READ, std::make_unique<ReadInstruction>());
    instructions_.emplace(OpCode::WRITE, std::make_unique<WriteInstruction>());
    // ... 共12个指令
}
```

**如果不用单例会怎样？**
- 每次需要工厂时，都会创建新的工厂实例
- 每个工厂实例都会创建 12 个指令对象
- 假设虚拟机执行 1000 条指令，可能会创建多个工厂实例，造成内存浪费

**使用单例的效果：**
- 12 个指令对象只创建一次
- 所有代码共享同一个工厂实例
- 内存高效，性能更好

#### 2. **全局唯一访问点**

虚拟机的任何地方都能通过 `getInstance()` 获取工厂实例：

```cpp
// 在 VirtualMachine 中使用
auto& factory = InstructionFactory::getInstance();
auto instruction = factory.getInstruction(opcode);
```

**优点：**
- 不需要在代码中传递工厂实例
- 不需要担心工厂的生命周期管理
- 简化了代码结构

#### 3. **状态一致性**

所有的指令对象存储在 `instructions_` 映射表中，使用单例确保所有代码访问的是**同一组指令对象**。

**保证：**
- 整个程序只有一套指令定义
- 不会出现不同版本的指令对象
- 行为一致性

#### 4. **线程安全的初始化**

使用 C++11 的 **Meyers Singleton** 模式（静态局部变量）：

```cpp
static InstructionFactory& getInstance() {
    static InstructionFactory instance;  // C++11 保证线程安全
    return instance;
}
```

**C++11 保证：**
- 第一次调用 `getInstance()` 时，`instance` 被初始化
- 多线程环境下，编译器保证只初始化一次
- 没有额外的锁开销（编译器优化）

#### 5. **内存管理简化**

使用单例模式 + `std::unique_ptr`：

```cpp
std::unordered_map<OpCode, std::unique_ptr<IInstruction>> instructions_;
```

**优点：**
- 指令对象的生命周期与工厂绑定
- 程序退出时，工厂析构，所有指令自动释放
- 不需要手动管理内存

---

### 单例模式适用场景

#### ✅ **应该使用单例的情况**

##### 1. **全局唯一的资源管理器**

当系统中某个资源在逻辑上应该全局唯一时：

- **配置管理器** (Configuration Manager)
  ```cpp
  class ConfigManager {
  public:
      static ConfigManager& getInstance();
      std::string get(const std::string& key);
  };
  ```

- **日志系统** (Logger)
  ```cpp
  class Logger {
  public:
      static Logger& getInstance();
      void log(const std::string& message);
  };
  ```

- **线程池 / 连接池**
  ```cpp
  class ThreadPool {
  public:
      static ThreadPool& getInstance();
      void submit(std::function<void()> task);
  };
  ```

- **硬件设备驱动**（如打印机、显卡）
  ```cpp
  class PrinterDriver {
  public:
      static PrinterDriver& getInstance();
      void print(const Document& doc);
  };
  ```

##### 2. **创建成本高且可复用的对象**

当对象的创建开销很大，且可以被多处代码复用时：

- **数据库连接管理器**
  ```cpp
  class DatabaseManager {
  private:
      Connection connection_;  // 维护唯一连接
  public:
      static DatabaseManager& getInstance();
      QueryResult execute(const std::string& sql);
  };
  ```

- **文件系统管理器**
  ```cpp
  class FileSystem {
  public:
      static FileSystem& getInstance();
      File open(const std::string& path);
  };
  ```

- **缓存管理器**
  ```cpp
  class CacheManager {
  private:
      std::unordered_map<std::string, std::string> cache_;
  public:
      static CacheManager& getInstance();
      std::optional<std::string> get(const std::string& key);
  };
  ```

##### 3. **需要全局访问点的工具类**

- **工厂类**（如本项目的 `InstructionFactory`）
- **ID 生成器**
  ```cpp
  class IDGenerator {
  private:
      std::atomic<int> nextID_{0};
  public:
      static IDGenerator& getInstance();
      int getNextID() { return nextID_++; }
  };
  ```

- **计数器 / 统计器**
  ```cpp
  class PerformanceCounter {
  private:
      std::atomic<int> requestCount_{0};
  public:
      static PerformanceCounter& getInstance();
      void increment() { requestCount_++; }
  };
  ```

#### ❌ **不应该使用单例的情况**

##### 1. **对象需要多个实例**

如果将来可能需要多个实例，单例会限制扩展性。

**反例：**
```cpp
// 错误：假设将来需要支持多个虚拟机实例，每个使用不同的指令集
class InstructionFactory {
    static InstructionFactory& getInstance();  // 单例限制了扩展性
};

// 正确做法：依赖注入
class VirtualMachine {
    InstructionFactory& factory_;  // 注入工厂实例
public:
    VirtualMachine(InstructionFactory& factory) : factory_(factory) {}
};
```

##### 2. **对象有状态且需要独立管理**

单例的全局状态可能导致耦合和测试困难。

**反例：**
```cpp
// 错误：购物车应该每个用户独立拥有
class ShoppingCart {
    static ShoppingCart& getInstance();
    std::vector<Item> items_;  // 全局状态！
};

// 正确做法：每个用户有自己的购物车实例
class User {
    ShoppingCart cart_;  // 独立实例
};
```

##### 3. **需要继承或多态**

单例模式使继承变得复杂。

**反例：**
```cpp
// 错误：单例难以继承
class DatabaseManager {
    static DatabaseManager& getInstance();
};

class MySQLManager : public DatabaseManager { /* 复杂！ */ };
```

##### 4. **仅仅为了方便**

如果只是为了避免传递参数，应考虑依赖注入。

**反例：**
```cpp
// 错误：仅为了方便
class Formatter {
    static Formatter& getInstance();
};

// 正确做法：依赖注入
class Printer {
    Formatter& formatter_;
public:
    Printer(Formatter& formatter) : formatter_(formatter) {}
};
```

---

### 单例模式的优缺点

#### ✅ **优点**

| 优点 | 说明 | 示例 |
|-----|------|------|
| **全局访问点** | 任何地方都能访问 | `Logger::getInstance().log("msg")` |
| **节省资源** | 只创建一次实例 | 避免重复创建数据库连接 |
| **状态一致** | 全局共享同一状态 | 所有代码使用同一配置 |
| **线程安全** | C++11 保证初始化安全 | Meyers Singleton |
| **延迟初始化** | 第一次使用时才创建 | 避免启动开销 |

#### ❌ **缺点**

| 缺点 | 说明 | 解决方案 |
|-----|------|---------|
| **全局状态** | 隐式依赖，难以追踪 | 依赖注入 |
| **测试困难** | 单例难以 Mock | 使用接口 + 依赖注入 |
| **单一职责** | 同时负责创建和管理 | 分离关注点 |
| **扩展性差** | 难以继承和多态 | 使用工厂方法 |
| **生命周期** | 析构顺序不确定 | 显式管理生命周期 |

---

### 最佳实践

#### 1. **使用 Meyers Singleton（推荐）**

```cpp
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;  // C++11 线程安全
        return instance;
    }

    // 禁用拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    Singleton() = default;  // 私有构造
};
```

**优点：**
- 线程安全（C++11 保证）
- 延迟初始化
- 简洁高效

#### 2. **避免在析构时访问其他单例**

```cpp
class Logger {
public:
    ~Logger() {
        // 危险！Config 可能已经析构
        // ConfigManager::getInstance().save();
    }
};
```

**解决方案：**
- 使用显式的 `shutdown()` 方法
- 避免单例之间的依赖

#### 3. **使用接口提高可测试性**

```cpp
// 定义接口
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& msg) = 0;
};

// 单例实现
class Logger : public ILogger {
public:
    static ILogger& getInstance() {
        static Logger instance;
        return instance;
    }
    void log(const std::string& msg) override { /* ... */ }
};

// 测试时可以 Mock
class MockLogger : public ILogger {
    void log(const std::string& msg) override { /* 测试实现 */ }
};
```

#### 4. **考虑依赖注入替代单例**

```cpp
// 单例模式（耦合）
class Service {
    void doWork() {
        Logger::getInstance().log("Working...");
    }
};

// 依赖注入（松耦合，易测试）
class Service {
    ILogger& logger_;
public:
    Service(ILogger& logger) : logger_(logger) {}
    void doWork() {
        logger_.log("Working...");
    }
};
```

---

### InstructionFactory 的设计评价

#### ✅ **使用单例是合理的，因为：**

1. **指令对象无状态** (Stateless)
   - 所有指令类没有成员变量
   - 可以安全地被多处代码复用
   - 不存在线程安全问题

2. **指令集固定不变**
   - 虚拟机的指令集在程序运行期间不会改变
   - 所有虚拟机实例使用相同的指令集
   - 符合单例"全局唯一"的语义

3. **避免重复创建**
   - 指令对象创建有开销（虽然不大）
   - 复用指令对象更高效
   - 简化内存管理

4. **全局访问便利**
   - 虚拟机任何地方都能获取指令
   - 代码简洁，易于维护

#### ⚠️ **潜在的限制：**

如果将来需要支持以下场景，单例会成为限制：

1. **多种虚拟机实例使用不同的指令集**
   ```cpp
   VirtualMachine vm1(factory1);  // 使用指令集 A
   VirtualMachine vm2(factory2);  // 使用指令集 B
   ```

2. **动态加载/卸载指令插件**
   ```cpp
   factory.registerInstruction(OpCode::CUSTOM, new CustomInstruction());
   factory.unregisterInstruction(OpCode::CUSTOM);
   ```

3. **测试时需要 Mock 指令工厂**
   ```cpp
   MockInstructionFactory mockFactory;
   VirtualMachine vm(mockFactory);  // 测试时使用 Mock
   ```

**改进建议：**

如果需要更高的灵活性，可以将单例改为依赖注入：

```cpp
// VirtualMachine.h
class VirtualMachine {
    InstructionFactory& factory_;  // 注入工厂引用

public:
    // 构造时注入工厂
    VirtualMachine(InstructionFactory& factory)
        : factory_(factory) {}

    void run() {
        auto instruction = factory_.getInstruction(opcode);
        // ...
    }
};

// 使用
auto& factory = InstructionFactory::getInstance();  // 可以使用单例
VirtualMachine vm(factory);  // 也可以注入其他工厂实例
```

这种设计兼顾了灵活性和便利性。

---

## 总结

### InstructionFactory 使用单例模式的核心原因

1. ✅ **资源复用** - 12 个指令对象只创建一次
2. ✅ **全局访问** - 任何地方都能获取工厂实例
3. ✅ **内存高效** - 避免重复创建指令对象
4. ✅ **线程安全** - C++11 保证初始化安全
5. ✅ **简化管理** - 不需要手动管理生命周期

### 单例模式的使用原则

| 适用场景 | 不适用场景 |
|---------|-----------|
| 全局唯一的资源管理器 | 对象需要多个实例 |
| 创建成本高且可复用的对象 | 对象有状态且需要独立管理 |
| 需要全局访问点的工具类 | 需要继承或多态 |
| 线程池、连接池、缓存 | 仅仅为了方便 |

### 最佳实践建议

1. 使用 **Meyers Singleton**（C++11 静态局部变量）
2. 禁用拷贝和移动构造/赋值
3. 考虑使用接口提高可测试性
4. 必要时使用**依赖注入**替代单例
5. 避免单例之间的相互依赖

---

**参考代码位置：**
- `include/InstructionFactory.h` - 单例模式声明
- `src/InstructionFactory.cpp:28-32` - Meyers Singleton 实现
- `src/InstructionFactory.cpp:11-25` - 指令对象初始化
