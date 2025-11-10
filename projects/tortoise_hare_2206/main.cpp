#include "Tortoise.h"
#include "Hare.h"
#include "Race.h"
#include "RacerConcept.h"
#include <iostream>
#include <memory>
#include <format>

// 编译期验证：确保Tortoise和Hare类型满足RacerConcept概念
static_assert(RacerConcept<Tortoise>, "Tortoise类必须满足RacerConcept概念");
static_assert(RacerConcept<Hare>, "Hare类必须满足RacerConcept概念");

/**
 * @brief 程序入口点
 * @return 0表示程序成功执行，1表示发生错误
 */
int main() {
    try {
        // 创建参赛者对象（使用智能指针自动管理内存）
        auto tortoise = std::make_unique<Tortoise>();
        auto hare = std::make_unique<Hare>();

        // 创建比赛对象并运行
        // 使用CTAD (类模板参数推导)
        Race race(std::move(tortoise), std::move(hare));
        race.run();

    } catch (const std::exception& e) {
        // 捕获并处理可能的异常
        std::cerr << std::format("错误: {}\n", e.what());
        return 1;
    }

    return 0;
}
