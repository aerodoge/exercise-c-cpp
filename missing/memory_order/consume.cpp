#include <atomic>
#include <thread>

struct Node
{
    int data;
    Node *next;
};

std::atomic<Node *> head{nullptr};

void producer()
{
    Node *new_node = new Node{42, nullptr};
    new_node->next = head.load(std::memory_order_relaxed);

    // 发布新节点
    head.store(new_node, std::memory_order_release);
}

void consumer()
{
    Node *node = head.load(std::memory_order_consume); // C++17后不推荐
    if (node)
    {
        // 只对依赖于node的操作提供排序保证
        int value = node->data;  // 有依赖，保证同步
        Node *next = node->next; // 有依赖，保证同步
    }
}

int main()
{
    producer();
    consumer();
    return 0;
}
