#Memory Order

##memory_order_relaxed - relaxed只保证单个原子操作的原子性，不提供任何同步或排序约束 -
    编译器和CPU可以自由重排序操作 -
    可能出现r1 = 0,
    r2 = 0的结果，因为两个线程的操作在不同CPU核心上可能以任意顺序执行 -
         这是性能最好的内存序，适用于简单的计数器场景

         ##memory_order_acquire -
         acquire用于读操作，创建获取屏障 - 防止acquire操作之后的所有读写操作重排到acquire之前 -
         与对应的release操作建立同步关系 - 保证能看到release操作之前的所有写操作 -
         适用于消费者模式，确保数据的可见性

         ##memory_order_release -
         release用于写操作，创建释放屏障 - 防止release操作之前的所有读写操作重排到release之后 -
         当其他线程用acquire读取时，建立happens - before关系 -
         保证release之前的所有操作对acquire线程可见 -
         适用于生产者模式，确保数据完整性后再发布

         ##memory_order_acq_rel -
         acq_rel同时具有acquire和release语义 - 对于RMW（read - modify - write）操作，既获取又释放 -
         防止操作前后的内存操作重排 - 在多线程同步点特别有用 -
         比单独使用acquire或release更严格，但比seq_cst性能更好

         ##memory_order_seq_cst -
         seq_cst（顺序一致性）是最严格的内存序，也是默认值 - 保证所有线程看到相同的全局操作顺序 -
         所有seq_cst操作形成单一的全序关系 - 防止任何形式的重排序 -
         性能开销最大，但提供最强的一致性保证 -
         适用于需要严格一致性的场景

         ##memory_order_consume（已废弃） -
         consume原本设计用于依赖排序，只保证有数据依赖的操作 -
         在C++ 17中被标记为不推荐使用，因为实现复杂且容易出错 - 大多数编译器将其实现为acquire语义 -
         理论上性能比acquire好，但实践中收益有限

         ##性能对比和选择建议
```c++
         // 性能从高到低：
         // relaxed > consume > acquire/release > acq_rel > seq_cst

         // 选择指南：
         std::atomic<int>
             counter{0};

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
