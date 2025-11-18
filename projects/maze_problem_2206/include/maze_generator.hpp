#ifndef MAZE_GENERATOR_HPP
#define MAZE_GENERATOR_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <vector>

#include "position.hpp"

// 迷宫生成器类模板
template <std::size_t N>
class MazeGenerator
{
private:
    std::array<std::array<char, N>, N> _grid;
    std::mt19937 rng_; // 梅森（Mersenne Twister）旋转引擎随机数生成器

    // 检查位置是否在范围内
    [[nodiscard]] constexpr bool is_in_bounds(int row, int col) const noexcept
    {
        return row >= 0 && row < static_cast<int>(N) && col >= 0 && col < static_cast<int>(N);
    }

    // 挖洞
    void carve_passages_from(int row, int col)
    {
        // 四个方向：上、右、下、左
        std::array<Position, 4> directions = {{
            {-2, 0}, // UP
            {0, 2},  // RIGHT
            {2, 0},  // DOWN
            {0, -2}  // LEFT
        }};

        // 随机打乱方向顺序
        std::shuffle(directions.begin(), directions.end(), rng_);

        // 尝试每个方向
        for (const auto& dir : directions)
        {
            int newRow = row + dir.row;
            int newCol = col + dir.col;

            // 检查新位置是否有效、未访问，且不在边界上
            if (newRow > 0 && newRow < static_cast<int>(N) - 1 && newCol > 0 &&
                newCol < static_cast<int>(N) - 1 && _grid[newRow][newCol] == '#')
            {
                // 打通当前位置和新位置之间的墙
                _grid[row + dir.row / 2][col + dir.col / 2] = '.';
                _grid[newRow][newCol] = '.';

                // 递归访问新位置
                carve_passages_from(newRow, newCol);
            }
        }
    }

    // 确保边界是墙
    void ensure_borders()
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            _grid[0][i] = '#';
            _grid[N - 1][i] = '#';
            _grid[i][0] = '#';
            _grid[i][N - 1] = '#';
        }
    }

    // 使用BFS从起点找到所有可达位置
    std::vector<Position> find_reachable_positions(const Position& start)
    {
        std::vector<Position> reachable;
        std::array<std::array<bool, N>, N> visited{};
        std::vector<Position> queue;

        queue.push_back(start);
        visited[start.row][start.col] = true;

        // 四个方向
        std::array<Position, 4> directions = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

        while (!queue.empty())
        {
            Position current = queue.front();
            queue.erase(queue.begin());
            reachable.push_back(current);

            for (const auto& dir : directions)
            {
                int newRow = current.row + dir.row;
                int newCol = current.col + dir.col;

                if (is_in_bounds(newRow, newCol) && !visited[newRow][newCol] &&
                    _grid[newRow][newCol] == '.')
                {
                    visited[newRow][newCol] = true;
                    queue.push_back({newRow, newCol});
                }
            }
        }

        return reachable;
    }

    // 创建出口
    void create_exit(const Position& start, Position& exit)
    {
        // 从起点找到所有可达位置
        auto reachable = find_reachable_positions(start);

        // 找到最右侧的列号
        int maxCol = 1;
        for (const auto& pos : reachable)
        {
            if (pos.col > maxCol)
            {
                maxCol = pos.col;
            }
        }

        // 收集最右侧列的所有可达位置
        std::vector<Position> candidates;
        for (const auto& pos : reachable)
        {
            if (pos.col == maxCol)
            {
                candidates.push_back(pos);
            }
        }

        // 随机选择一个位置作为出口的内部位置
        if (!candidates.empty())
        {
            std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
            Position exitInside = candidates[dist(rng_)];
            // 在它右侧设置出口
            exit = {exitInside.row, exitInside.col + 1};
        }
        else
        {
            // 备用方案：使用起点右侧
            exit = {start.row, start.col + 1};
        }
    }

public:
    // 构造函数
    explicit MazeGenerator(unsigned int seed = std::random_device{}()) : rng_(seed)
    {
        // 初始化为全墙
        for (auto& row : _grid)
        {
            row.fill('#');
        }
    }

    // 生成迷宫
    [[nodiscard]] std::array<std::array<char, N>, N> generate(Position& start, Position& end)
    {
        // 初始化为全墙
        for (auto& row : _grid)
        {
            row.fill('#');
        }

        // 起点设置为 (1, 1)
        start = {1, 1};
        _grid[1][1] = '.';

        // 从起点开始递归生成迷宫
        carve_passages_from(1, 1);

        // 先创建出口（在确保边界之前）
        create_exit(start, end);

        // 确保边界是墙，但保留出口
        ensure_borders();

        // 打通出口：确保出口位置是可通过的
        _grid[end.row][end.col] = '.';

        return _grid;
    }

    // 设置随机种子
    void set_seed(unsigned int seed) { rng_.seed(seed); }
};

#endif // MAZE_GENERATOR_HPP
