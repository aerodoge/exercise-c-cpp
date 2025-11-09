/**
 * @file Race.h
 * @brief 比赛管理类模板声明和实现（模板类必须在头文件中实现）
 */

#ifndef RACE_H
#define RACE_H

#include "RacerConcept.h"
#include "Constants.h"
#include <iostream>
#include <memory>
#include <format>
#include <ranges>

/**
 * @class Race
 * @brief 比赛管理类，负责协调整个赛跑过程
 * @tparam R1 参赛者1的类型（必须满足RacerConcept）
 * @tparam R2 参赛者2的类型（必须满足RacerConcept）
 * @details 使用C++20模板和概念确保类型安全，支持任意满足RacerConcept的参赛者类型
 */
template<RacerConcept R1, RacerConcept R2>
class Race {
private:
    std::unique_ptr<R1> racer1;     ///< 参赛者1的智能指针
    std::unique_ptr<R2> racer2;     ///< 参赛者2的智能指针
    int tickCount{0};                ///< 时钟滴答计数（比赛回合数）

    /// 获胜者枚举类型，使用enum class提高类型安全性
    enum class Winner {
        None,       ///< 尚未决出胜负
        Racer1,     ///< 参赛者1获胜
        Racer2,     ///< 参赛者2获胜
        Tie         ///< 平局
    };

public:
    /**
     * @brief 构造函数
     * @param r1 参赛者1的unique_ptr
     * @param r2 参赛者2的unique_ptr
     */
    Race(std::unique_ptr<R1> r1, std::unique_ptr<R2> r2)
        : racer1(std::move(r1)), racer2(std::move(r2)) {}

    /**
     * @brief 打印比赛开始信息
     */
    void printStartMessage() const {
        std::cout << "BANG !!!!!\n"
                  << "AND THEY'RE OFF !!!!!\n\n";
    }

    /**
     * @brief 打印当前赛道状态
     * @details 使用C++20 ranges库的iota视图遍历赛道，显示参赛者位置
     *          如果两个参赛者在同一位置，则显示"OUCH!!!"（乌龟咬住兔子）
     */
    void printTrack() const {
        const int pos1 = racer1->getPosition();
        const int pos2 = racer2->getPosition();

        if (pos1 == pos2) {
            // 两者在同一位置，乌龟咬住兔子！
            std::cout << "OUCH!!! ";
            for (const int i : std::views::iota(1, FINISH_LINE + 1)) {
                std::cout << (i == pos1 ? "OUCH!!!" : " ");
            }
        } else {
            // 两者在不同位置，正常显示
            for (const int i : std::views::iota(1, FINISH_LINE + 1)) {
                if (i == pos1) {
                    std::cout << racer1->getSymbol();
                } else if (i == pos2) {
                    std::cout << racer2->getSymbol();
                } else {
                    std::cout << ' ';
                }
            }
        }
        std::cout << '\n';
    }

    /**
     * @brief 检查比赛是否结束并返回获胜者
     * @return Winner枚举值，表示获胜情况
     */
    [[nodiscard]] Winner checkWinner() const noexcept {
        const bool r1Finished = racer1->hasFinished();
        const bool r2Finished = racer2->hasFinished();

        if (r1Finished && r2Finished) {
            return Winner::Tie;         // 同时到达，平局
        } else if (r1Finished) {
            return Winner::Racer1;      // 参赛者1先到达
        } else if (r2Finished) {
            return Winner::Racer2;      // 参赛者2先到达
        }
        return Winner::None;            // 比赛继续
    }

    /**
     * @brief 打印获胜信息
     * @param winner 获胜者枚举值
     * @details 根据不同的参赛者显示特定的获胜消息
     */
    void printWinnerMessage(Winner winner) const {
        std::cout << '\n';

        switch (winner) {
            case Winner::Racer1:
                // 参赛者1获胜
                if (racer1->getName() == "Tortoise") {
                    std::cout << "TORTOISE WINS!!! YAY!!!\n";  // 乌龟获胜！弱者的胜利！
                } else {
                    std::cout << std::format("{} wins!\n", racer1->getName());
                }
                break;
            case Winner::Racer2:
                // 参赛者2获胜
                if (racer2->getName() == "Hare") {
                    std::cout << "Hare wins. Yuch.\n";  // 兔子获胜（不太光彩）
                } else {
                    std::cout << std::format("{} wins!\n", racer2->getName());
                }
                break;
            case Winner::Tie:
                // 平局
                std::cout << "It's a tie.\n";
                break;
            case Winner::None:
                break;
        }
    }

    /**
     * @brief 打印比赛统计信息
     * @details 使用C++20的std::format格式化输出
     */
    void printStatistics() const {
        std::cout << std::format("\nRace finished in {} ticks!\n", tickCount);
        std::cout << std::format("Final positions - {}: {}, {}: {}\n",
                                 racer1->getName(), racer1->getPosition(),
                                 racer2->getName(), racer2->getPosition());
    }

    /**
     * @brief 运行整场比赛
     * @details 主循环：每个时钟滴答移动参赛者，显示赛道，检查获胜条件
     */
    void run() {
        printStartMessage();

        while (true) {
            ++tickCount;  // 时钟滴答

            // 移动两个参赛者
            racer1->move();
            racer2->move();

            // 显示当前赛道状态
            std::cout << std::format("Tick {}: ", tickCount);
            printTrack();

            // 检查是否有获胜者
            const Winner winner = checkWinner();
            if (winner != Winner::None) {
                printWinnerMessage(winner);
                break;  // 比赛结束
            }
        }

        printStatistics();  // 显示比赛统计
    }
};

#endif // RACE_H
