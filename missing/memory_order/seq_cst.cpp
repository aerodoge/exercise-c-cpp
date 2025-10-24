#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> x{false}, y{false};
std::atomic<int> z{0};

void write_x_then_y()
{
    x.store(true, std::memory_order_seq_cst); // 1
    y.store(true, std::memory_order_seq_cst); // 2
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
    { // 3
        std::this_thread::yield();
    }
    if (x.load(std::memory_order_seq_cst))
    { // 4
        ++z;
    }
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst))
    { // 5
        std::this_thread::yield();
    }
    if (y.load(std::memory_order_seq_cst))
    { // 6
        ++z;
    }
}

int main()
{
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    std::thread t3(read_x_then_y);

    t1.join();
    t2.join();
    t3.join();

    // 使用seq_cst，z永远不会是0
    std::cout << "z = " << z << std::endl; // 通常输出1或2
}
