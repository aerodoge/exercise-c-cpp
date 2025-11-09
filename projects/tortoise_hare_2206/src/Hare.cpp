/**
 * @file Hare.cpp
 * @brief 兔子类实现
 */

#include "Hare.h"
#include "RandomGenerator.h"

Hare::Hare() : Racer("Hare", 'H') {}

void Hare::move() {
    const int moveType = RandomGenerator::getInstance().generate();

    switch (moveType) {
        case 1:
        case 2:
            // 睡觉 (20%): 原地不动
            break;
        case 3:
        case 4:
            // 大跳 (20%): 向右9格
            position += 9;
            break;
        case 5:
            // 严重滑倒 (10%): 向左12格
            position -= 12;
            break;
        case 6:
        case 7:
        case 8:
            // 小跳 (30%): 向右1格
            position += 1;
            break;
        case 9:
        case 10:
            // 轻微滑倒 (20%): 向左2格
            position -= 2;
            break;
    }

    ensureValidPosition();  // 确保不会滑出起点
}
