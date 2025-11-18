#ifndef MAZE_HPP
#define MAZE_HPP

#include <array>
#include <cstddef>

#include "position.hpp"

// 迷宫类模板
template <std::size_t N>
class Maze
{
private:
    std::array<std::array<char, N>, N> _grid;
    Position _start;
    Position _end;
    int _steps = 0;

    // 四个方向的偏移量（上、右、下、左）
    static constexpr std::array<Position, 4> _directions = {{
        {-1, 0}, // UP
        {0, 1},  // RIGHT
        {1, 0},  // DOWN
        {0, -1}  // LEFT
    }};

    // 右手法则
    bool traverse_right_hand(Position current, Direction facing);
    // 深度优先
    bool traverse_dfs(Position current);

public:
    // 构造函数
    Maze(const std::array<std::array<char, N>, N>& grid, Position start, Position end);

    // 检查位置是否有效
    [[nodiscard]] bool is_valid(const Position& pos) const noexcept;

    // 检查位置是否可以访问
    [[nodiscard]] bool can_visit(const Position& pos) const noexcept;

    // 打印迷宫
    void display() const;

    // 标记位置
    void mark(const Position& pos, char marker);

    // 获取字符
    [[nodiscard]] char get_char(const Position& pos) const;

    // 开始遍历
    bool solve(bool useRightHand = true);
};

// 由于是模板类，需要在头文件中包含实现
#include "../src/maze.tpp"

#endif // MAZE_HPP
