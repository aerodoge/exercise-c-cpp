/**
 * @file Tortoise.cpp
 * @brief 乌龟类实现
 */

#include "Tortoise.h"
#include "RandomGenerator.h"

Tortoise::Tortoise() : Racer("Tortoise", 'T') {}

void Tortoise::move() {
    const int moveType = RandomGenerator::getInstance().generate();

    if (moveType >= 1 && moveType <= 5) {
        // 快速移动 (50%): 向右3格
        position += 3;
    } else if (moveType >= 6 && moveType <= 7) {
        // 滑倒 (20%): 向左6格
        position -= 6;
    } else {
        // 缓慢移动 (30%): 向右1格
        position += 1;
    }

    ensureValidPosition();  // 确保不会滑出起点
}
