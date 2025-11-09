#ifndef POSITION_HPP
#define POSITION_HPP

#include <concepts>
#include <compare>

// 现代C++20概念：定义坐标类型约束
template<typename T>
concept Coordinate = requires(T t) {
    { t.row } -> std::convertible_to<int>;
    { t.col } -> std::convertible_to<int>;
};

// 位置结构体
struct Position {
    int row;
    int col;

    // C++20 三路比较运算符
    auto operator<=>(const Position&) const = default;
};

// 方向枚举
enum class Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

#endif // POSITION_HPP
