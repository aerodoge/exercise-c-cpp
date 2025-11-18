#ifndef MAZE_TPP
#define MAZE_TPP

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

// 构造函数实现
template <std::size_t N>
Maze<N>::Maze(const std::array<std::array<char, N>, N>& grid, Position start, Position end)
    : _grid(grid), _start(start), _end(end)
{
}

// 检查位置是否有效
template <std::size_t N>
bool Maze<N>::is_valid(const Position& pos) const noexcept
{
    return pos.row >= 0 && pos.row < static_cast<int>(N) && pos.col >= 0 &&
           pos.col < static_cast<int>(N);
}

// 检查位置是否可以访问
template <std::size_t N>
bool Maze<N>::can_visit(const Position& pos) const noexcept
{
    return is_valid(pos) && (_grid[pos.row][pos.col] == '.' || _grid[pos.row][pos.col] == 'E');
}

// 打印迷宫
template <std::size_t N>
void Maze<N>::display() const
{
    std::cout << "\n步骤 " << _steps << ":\n";
    std::cout << "   "; // 3个空格对齐
    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << std::setw(2) << std::setfill('0') << i << " ";
    }
    std::cout << "\n";

    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << std::setw(2) << std::setfill('0') << i << " ";
        for (std::size_t j = 0; j < N; ++j)
        {
            std::cout << std::setw(2) << std::setfill(' ') << _grid[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

// 标记位置
template <std::size_t N>
void Maze<N>::mark(const Position& pos, char marker)
{
    if (is_valid(pos) && _grid[pos.row][pos.col] != 'S' && _grid[pos.row][pos.col] != 'E')
    {
        _grid[pos.row][pos.col] = marker;
    }
}

// 获取字符
template <std::size_t N>
char Maze<N>::get_char(const Position& pos) const
{
    return is_valid(pos) ? _grid[pos.row][pos.col] : '#';
}

// 右手法则遍历（递归实现）
template <std::size_t N>
bool Maze<N>::traverse_right_hand(Position current, Direction facing)
{
    _steps++;

    // 到达终点
    if (current == _end)
    {
        mark(current, 'X');
        display();
        std::this_thread::sleep_for(300ms);
        return true;
    }

    // 标记当前位置
    mark(current, 'X');
    display();
    std::this_thread::sleep_for(300ms);

    // 右手法则：优先尝试右转，然后直行，然后左转，最后后退
    // 按照面向方向，计算右、前、左、后的方向
    std::array<Direction, 4> tryOrder = {{
        static_cast<Direction>((static_cast<int>(facing) + 1) % 4), // 右
        facing,                                                     // 前
        static_cast<Direction>((static_cast<int>(facing) + 3) % 4), // 左
        static_cast<Direction>((static_cast<int>(facing) + 2) % 4)  // 后
    }};

    for (Direction dir : tryOrder)
    {
        Position next = {current.row + _directions[static_cast<int>(dir)].row,
                         current.col + _directions[static_cast<int>(dir)].col};

        if (can_visit(next))
        {
            if (traverse_right_hand(next, dir))
            {
                return true;
            }
        }
    }

    // 回溯
    mark(current, 'o');
    display();
    std::this_thread::sleep_for(200ms);
    return false;
}

// DFS遍历（深度优先搜索）
template <std::size_t N>
bool Maze<N>::traverse_dfs(Position current)
{
    _steps++;

    // 到达终点
    if (current == _end)
    {
        mark(current, 'X');
        display();
        std::this_thread::sleep_for(300ms);
        return true;
    }

    // 标记当前位置
    mark(current, 'X');
    display();
    std::this_thread::sleep_for(300ms);

    // 尝试四个方向（上、右、下、左）
    for (const auto& dir : _directions)
    {
        Position next = {current.row + dir.row, current.col + dir.col};

        if (can_visit(next))
        {
            if (traverse_dfs(next))
            {
                return true;
            }
        }
    }

    // 回溯
    mark(current, 'o');
    display();
    std::this_thread::sleep_for(200ms);
    return false;
}

// 开始遍历
template <std::size_t N>
bool Maze<N>::solve(bool useRightHand)
{
    _steps = 0;
    mark(_start, 'S');
    mark(_end, 'E');

    std::cout << "\n开始迷宫遍历..." << "\n";
    std::cout << "起点: (" << _start.row << ", " << _start.col << ")\n";
    std::cout << "终点: (" << _end.row << ", " << _end.col << ")\n";
    std::cout << "算法: " << (useRightHand ? "右手法则" : "深度优先搜索") << "\n";

    display();
    std::this_thread::sleep_for(1s);

    bool result;
    if (useRightHand)
    {
        result = traverse_right_hand(_start, Direction::RIGHT);
    }
    else
    {
        result = traverse_dfs(_start);
    }

    if (result)
    {
        std::cout << "\n成功找到出口！总步数: " << _steps << "\n";
    }
    else
    {
        std::cout << "\n未找到出口。\n";
    }

    return result;
}

#endif // MAZE_TPP
