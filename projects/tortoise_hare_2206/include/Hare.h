/**
 * @file Hare.h
 * @brief 兔子类声明
 */

#ifndef HARE_H
#define HARE_H

#include "Racer.h"

/**
 * @class Hare
 * @brief 兔子类，实现快速但不稳定的移动策略
 * @details
 * 移动规则（基于1-10随机数）：
 * - 1-2 (20%): 睡觉，原地不动
 * - 3-4 (20%): 大跳，向右9格
 * - 5 (10%): 严重滑倒，向左12格
 * - 6-8 (30%): 小跳，向右1格
 * - 9-10 (20%): 轻微滑倒，向左2格
 */
class Hare final : public Racer {
public:
    /// 构造函数，初始化名称为"Hare"，符号为'H'
    Hare();

    /**
     * @brief 实现兔子的移动规则
     * @details 使用switch语句清晰表达五种不同的移动类型
     */
    void move() override;
};

#endif // HARE_H
