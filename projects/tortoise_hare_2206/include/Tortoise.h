/**
 * @file Tortoise.h
 * @brief 乌龟类声明
 */

#ifndef TORTOISE_H
#define TORTOISE_H

#include "Racer.h"

/**
 * @class Tortoise
 * @brief 乌龟类，实现稳定但缓慢的移动策略
 * @details
 * 移动规则（基于1-10随机数）：
 * - 1-5 (50%): 快速移动，向右3格
 * - 6-7 (20%): 滑倒，向左6格
 * - 8-10 (30%): 缓慢移动，向右1格
 */
class Tortoise final : public Racer {
public:
    /// 构造函数，初始化名称为"Tortoise"，符号为'T'
    Tortoise();

    /**
     * @brief 实现乌龟的移动规则
     * @details 根据随机数决定移动类型：快速、滑倒或缓慢
     */
    void move() override;
};

#endif // TORTOISE_H
