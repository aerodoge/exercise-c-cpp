#include <atomic>
#include <cassert>
#include <thread>

std::atomic<bool> flag{false};
int data = 0;

void producer()
{
    data = 42;                                   // 普通写操作
    flag.store(true, std::memory_order_release); // 释放操作
}

void consumer()
{
    while (!flag.load(std::memory_order_acquire))
    { // 获取操作
        std::this_thread::yield();
    }
    // 此时保证能看到data=42
    assert(data == 42); // 永远不会失败
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
