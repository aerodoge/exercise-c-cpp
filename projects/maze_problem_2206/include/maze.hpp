#ifndef MAZE_HPP
#define MAZE_HPP

#include "position.hpp"
#include <array>
#include <cstddef>

// 迷宫类模板
template<std::size_t N>
class Maze {
private:
    std::array<std::array<char, N>, N> grid_;
    Position start_;
    Position end_;
    int steps_ = 0;

    // 四个方向的偏移量（上、右、下、左）
    static constexpr std::array<Position, 4> directions_ = {{
        {-1, 0},  // UP
        {0, 1},   // RIGHT
        {1, 0},   // DOWN
        {0, -1}   // LEFT
    }};

    // 私有辅助方法
    bool traverseRightHand(Position current, Direction facing);
    bool traverseDFS(Position current);

public:
    // 构造函数
    Maze(const std::array<std::array<char, N>, N>& grid, Position start, Position end);

    // 检查位置是否有效
    [[nodiscard]] bool isValid(const Position& pos) const noexcept;

    // 检查位置是否可以访问
    [[nodiscard]] bool canVisit(const Position& pos) const noexcept;

    // 打印迷宫
    void display() const;

    // 标记位置
    void mark(const Position& pos, char marker);

    // 获取字符
    [[nodiscard]] char getChar(const Position& pos) const;

    // 开始遍历
    bool solve(bool useRightHand = true);
};

// 由于是模板类，需要在头文件中包含实现
#include "../src/maze.tpp"

#endif // MAZE_HPP
