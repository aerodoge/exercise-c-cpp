#include <atomic>
#include <cassert>
#include <thread>
#include <vector>

std::atomic<int> counter{0};
std::vector<int> data;
std::atomic<bool> ready{false};

void writer()
{
    // 准备数据
    for (int i = 0; i < 10; ++i)
    {
        data.push_back(i);
        counter.fetch_add(1, std::memory_order_relaxed);
    }

    // 发布数据（release语义）
    ready.store(true, std::memory_order_release);
}

void reader()
{
    // 等待数据准备完成
    while (!ready.load(std::memory_order_acquire))
    {
        std::this_thread::yield();
    }

    // 保证能看到所有的数据写入
    assert(data.size() == 10);
    assert(counter.load(std::memory_order_relaxed) == 10);
}
int main()
{
    std::thread writerThread(writer);
    std::thread readerThread(reader);
    writerThread.join();
    readerThread.join();
    return 0;
}