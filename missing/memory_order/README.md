# memory order

## 什么是流水线（Pipeline）

## 类比：洗衣服的流水线

想象你要洗很多批衣服，每批衣服需要：

- 洗（30分钟）
- 烘干（30分钟）
- 叠衣服（30分钟）

### 非流水线方式（一批一批做）：

```
批次1: [洗] → [烘] → [叠]
批次2:                  [洗] → [烘] → [叠]
批次3:                                   [洗] → [烘] → [叠]
总时间: 270分钟
```

### 流水线方式（重叠做）

```
时间:  0-30   30-60   60-90   90-120  120-150
批次1: [洗]   [烘]     [叠]
批次2:        [洗]     [烘]    [叠]
批次3:                 [洗]    [烘]     [叠]
总时间: 150分钟

当批次1在烘干时，批次2可以开始洗；当批次1在叠衣服时，批次2在烘干，批次3可以开始洗。
```

## CPU流水线

CPU 执行一条指令也需要多个步骤：

- IF (Instruction Fetch)     - 取指令：从内存读取指令
- ID (Instruction Decode)    - 译码：解析指令是什么操作
- EX (Execute)               - 执行：进行实际计算
- MEM (Memory Access)        - 访存：读写内存（如果需要）
- WB (Write Back)            - 写回：将结果写回寄存器

### 非流水线（每条指令90ns，假设每阶段10ns）

```
指令1: [IF][ID][EX][MEM][WB]
指令2:                      [IF][ID][EX][MEM][WB]
指令3:                                           [IF][ID][EX][MEM][WB]
总时间: 150ns
```

### 流水线

```
时钟:   1    2      3     4     5      6      7
指令1:[IF]  [ID]  [EX]  [MEM]  [WB]
指令2:      [IF]  [ID]  [EX]   [MEM]  [WB]
指令3:            [IF]  [ID]   [EX]   [MEM]  [WB]
总时间: 70ns（理想情况下，吞吐量提升5倍）
```

## 流水线冒险

### 数据依赖（Data Hazard）

```
int a = x + y;    // 指令1
int b = a * 2;    // 指令2：依赖指令1的结果

时钟:   1     2     3      4     5      6     7     8
指令1: [IF]  [ID]  [EX]  [MEM]  [WB]
指令2:       [IF]  [ID]  [等]   [等]   [EX]  [MEM]  [WB]  ← 必须等待a的值
                    ↑
            需要a的值，但a还在执行中！
```

指令2必须停顿（stall）等待，浪费了CPU周期。

### 通过重排避免停顿

```
int a = x + y;    // 指令1
int b = a * 2;    // 指令2：依赖指令1
int c = p + q;    // 指令3：不依赖指令1

时钟:   1      2     3     4     5      6      7     8
指令1: [IF]  [ID]  [EX]  [MEM]  [WB]
指令2:       [IF]  [ID]  [等]   [等]   [EX]   [MEM]  [WB]        
指令3:             [IF]  [ID]   [EX]  [MEM]   [WB]  

重排：
int a = x + y;    // 指令1
int c = p + q;    // 指令2：不依赖指令1
int b = a * 2;    // 指令3：依赖指令1

时钟:   1      2     3     4      5     6      7     8
指令1: [IF]  [ID]  [EX]  [MEM]  [WB]
指令2:       [IF]  [ID]  [EX]   [MEM]  [WB]              ← 不依赖指令1，继续执行
指令3:             [IF]  [ID]   [等]   [EX]   [MEM] [WB]  ← 等待时间减少

```

# Memory Order

## memory_order_relaxed

- relaxed只保证单个原子操作的原子性，不提供任何同步或排序约束
- 编译器和CPU可以自由重排序操作
- 可能出现r1 = 0, r2 = 0的结果，因为两个线程的操作在不同CPU核心上可能以任意顺序执行
- 这是性能最好的内存序，适用于简单的计数器场景

## memory_order_acquire

- acquire用于读操作，创建获取屏障
- 防止acquire操作之后的所有读写操作重排到acquire之前
- 与对应的release操作建立同步关系
- 保证能看到release操作之前的所有写操作
- 适用于消费者模式，确保数据的可见性

## memory_order_release

- release用于写操作，创建释放屏障
- 防止release操作之前的所有读写操作重排到release之后
- 当其他线程用acquire读取时，建立happens
- before关系
- 保证release之前的所有操作对acquire线程可见
- 适用于生产者模式，确保数据完整性后再发布

## memory_order_acq_rel

- acq_rel同时具有acquire和release语义
- 对于RMW（read - modify - write）操作，既获取又释放
- 防止操作前后的内存操作重排 - 在多线程同步点特别有用
- 比单独使用acquire或release更严格，但比seq_cst性能更好

## memory_order_seq_cst

- seq_cst（顺序一致性）是最严格的内存序，也是默认值
- 保证所有线程看到相同的全局操作顺序
- 所有seq_cst操作形成单一的全序关系
- 防止任何形式的重排序
- 性能开销最大，但提供最强的一致性保证
- 适用于需要严格一致性的场景

## memory_order_consume（已废弃）

- consume原本设计用于依赖排序，只保证有数据依赖的操作
- 在C++ 17中被标记为不推荐使用，因为实现复杂且容易出错
- 大多数编译器将其实现为acquire语义
- 理论上性能比acquire好，但实践中收益有限

## 性能对比和选择建议

```c++
// 性能从高到低：
// relaxed > consume > acquire/release > acq_rel > seq_cst

// 选择指南：
std::atomic<int> counter{0};

// 简单计数器：使用relaxed
counter.fetch_add(1, std::memory_order_relaxed);

// 生产者-消费者：使用acquire-release
flag.store(true, std::memory_order_release); // 生产者
while (!flag.load(std::memory_order_acquire))
    ; // 消费者

// 原子操作需要同步：使用acq_rel
counter.compare_exchange_weak(expected, desired, std::memory_order_acq_rel);

// 需要全局一致性：使用seq_cst（默认）
flag.store(true); // 等价于memory_order_seq_cst
```
