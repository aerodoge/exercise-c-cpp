#include <atomic>
#include <thread>

std::atomic<int> shared_counter{0};
std::atomic<bool> flags[2] = {false, false};

void worker(int id)
{
    // 原子地增加计数器（获取-释放语义）
    int old_value = shared_counter.fetch_add(1, std::memory_order_acq_rel);

    // 设置自己的标志
    flags[id].store(true, std::memory_order_release);

    // 等待另一个线程
    int other_id = 1 - id;
    while (!flags[other_id].load(std::memory_order_acquire))
    {
        std::this_thread::yield();
    }

    // 读取最终计数器值
    int final_value = shared_counter.load(std::memory_order_acquire);
    printf("Thread %d: old=%d, final=%d\n", id, old_value, final_value);
}

int main()
{
    std::thread t1(worker, 0);
    std::thread t2(worker, 1);
    t1.join();
    t2.join();
}
