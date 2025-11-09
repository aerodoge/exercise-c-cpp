/**
 * @file RacerConcept.h
 * @brief C++20概念定义
 */

#ifndef RACER_CONCEPT_H
#define RACER_CONCEPT_H

#include <concepts>
#include <string_view>

/**
 * @brief 参赛者概念，定义所有参赛者必须满足的接口要求
 * @tparam T 参赛者类型
 *
 * 要求：
 * - move(): 移动方法
 * - getPosition(): 获取当前位置
 * - getName(): 获取名称
 * - getSymbol(): 获取显示符号
 * - hasFinished(): 判断是否到达终点
 */
template<typename T>
concept RacerConcept = requires(T racer) {
    { racer.move() } -> std::same_as<void>;
    { racer.getPosition() } -> std::convertible_to<int>;
    { racer.getName() } -> std::convertible_to<std::string_view>;
    { racer.getSymbol() } -> std::convertible_to<char>;
    { racer.hasFinished() } -> std::convertible_to<bool>;
};

#endif // RACER_CONCEPT_H
