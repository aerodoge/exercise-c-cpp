#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x{0}, y{0};
std::atomic<int> r1{0}, r2{0};

void thread1()
{
    x.store(1, std::memory_order_relaxed);  // 写x
    r1 = y.load(std::memory_order_relaxed); // 读y
}

void thread2()
{
    y.store(1, std::memory_order_relaxed);  // 写y
    r2 = x.load(std::memory_order_relaxed); // 读x
}

int main()
{
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.join();
    t2.join();

    // 可能的结果：r1=0, r2=0（看起来两个写操作都没发生）
    std::cout << "r1=" << r1 << ", r2=" << r2 << std::endl;
}
