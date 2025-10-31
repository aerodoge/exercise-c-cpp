#ifndef EIGHT_QUEENS_QUEEN_H
#define EIGHT_QUEENS_QUEEN_H

#include <iostream>
#include <vector>

class Queen
{
public:
    // 构造函数
    Queen();

    // 求解8皇后问题，返回找到的解的数量
    int solve();

    // 打印所有解
    void print_all_solutions() const;

    // 打印指定的解
    void print_solution(const int solution_index) const;

    // 获取解的数量
    [[nodiscard]] int get_solution_count() const;

    static constexpr int BOARD_SIZE = 8;

private:
    // 使用回溯算法求解
    void backtrack(const int row);

    // 检查位置能否放置
    [[nodiscard]] bool can_place(const int row, const int col) const;

    // 打印单个棋盘状态
    void print_board(const std::vector<int>& positions) const;

    // 存储当前正在构建的解：positions[row] = col
    std::vector<int> _positions;
    // 存储所有找到的解
    std::vector<std::vector<int>> _solutions;
    // 列标记
    std::vector<bool> _cols;
    // 反斜线（\）标记 (row - col + BOARD_SIZE - 1)
    /*
     * row-col的范围是[-7, 7]，共15个不同的值
     * 数组索引必须从0开始，不能是负数。所以需要将[-7, 7]映射到[0, 14]
     *
     *    0  1  2  3  4  5  6  7     0  1  2  3  4  5  6  7
     * 0  0 -1 -2 -3 -4 -5 -6 -7     7  6  5  4  3  2  1  0
     * 1  1  0 -1 -2 -3 -4 -5 -6     8  7  6  5  4  3  2  1
     * 2  2  1  0 -1 -2 -3 -4 -5     9  8  7  6  5  4  3  2
     * 3  3  2  1  0 -1 -2 -3 -4     10 9  8  7  6  5  4  3
     * 4  4  3  2  1  0 -1 -2 -3     11 10 9  8  7  6  5  4
     * 5  5  4  3  2  1  0 -1 -2     12 11 10 9  8  7  6  5
     * 6  6  5  4  3  2  1  0 -1     13 12 11 10 9  8  7  6
     * 7  7  6  5  4  3  2  1  0     14 13 12 11 10 9  8  7
     */
    std::vector<bool> _diag1;
    // 正斜线（/）标记 (row + col)
    /*
     *    0  1  2  3  4  5  6  7
     * 0  0  1  2  3  4  5  6  7
     * 1  1  2  3  4  5  6  7  8
     * 2  2  3  4  5  6  7  8  9
     * 3  3  4  5  6  7  8  9  10
     * 4  4  5  6  7  8  9  10 11
     * 5  5  6  7  8  9  10 11 12
     * 6  6  7  8  9  10 11 12 13
     * 7  7  8  9  10 11 12 13 14
     */
    std::vector<bool> _diag2;
};

#endif // EIGHT_QUEENS_QUEEN_H