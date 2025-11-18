#ifndef POSITION_HPP
#define POSITION_HPP

#include <compare>
#include <concepts>

// 定义坐标类型约束
template <typename T>
concept Coordinate = requires(T t) {
    { t.row } -> std::convertible_to<int>;
    { t.col } -> std::convertible_to<int>;
};

// 位置结构体
struct Position
{
    int row;
    int col;

    // 三路比较运算符
    // 被称为spaceship operator（飞船运算符，因为看起来像🚀），是C++20的新特性。
    auto operator<=>(const Position&) const = default;
    /**
    * // 编译器实际生成的（伪代码）
      auto Position::operator<=>(const Position& other) const {
          // 按声明顺序比较成员
          if (auto cmp = row <=> other.row; cmp != 0)
              return cmp;  // row 不同，直接返回比较结果
          return col <=> other.col;  // row 相同，比较 col
      }

      // 并且自动生成这些：
      bool operator==(const Position& other) const { ... }
      bool operator!=(const Position& other) const { ... }
      bool operator<(const Position& other) const { ... }
      bool operator<=(const Position& other) const { ... }
      bool operator>(const Position& other) const { ... }
      bool operator>=(const Position& other) const { ... }
     */
};

// 方向枚举
enum class Direction
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

#endif // POSITION_HPP
