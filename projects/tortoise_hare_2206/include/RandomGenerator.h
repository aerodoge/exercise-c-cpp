/**
 * @file RandomGenerator.h
 * @brief 随机数生成器类声明
 */

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>

/**
 * @class RandomGenerator
 * @brief 使用梅森旋转算法的现代随机数生成器
 * @details 采用单例模式，生成1-10之间的均匀分布随机整数
 */
class RandomGenerator {
private:
    std::mt19937 engine;                            ///< 梅森旋转算法引擎
    std::uniform_int_distribution<int> distribution; ///< 均匀分布生成器

    /**
     * @brief 构造函数，使用随机设备初始化引擎
     */
    RandomGenerator();

public:
    /**
     * @brief 生成1-10之间的随机整数
     * @return 随机整数
     */
    [[nodiscard]] int generate();

    /**
     * @brief 获取单例实例
     * @return RandomGenerator引用
     */
    static RandomGenerator& getInstance();

    // 禁用拷贝和赋值
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;
};

#endif // RANDOM_GENERATOR_H
