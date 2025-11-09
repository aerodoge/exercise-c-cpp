/**
 * @file RandomGenerator.cpp
 * @brief 随机数生成器类实现
 */

#include "RandomGenerator.h"

RandomGenerator::RandomGenerator()
    : engine(std::random_device{}()),
      distribution(1, 10) {}

int RandomGenerator::generate() {
    return distribution(engine);
}

RandomGenerator& RandomGenerator::getInstance() {
    static RandomGenerator instance;
    return instance;
}
