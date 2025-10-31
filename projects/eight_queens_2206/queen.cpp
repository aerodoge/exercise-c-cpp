#include "queen.h"

Queen::Queen()
    : _positions(BOARD_SIZE, -1), _cols(BOARD_SIZE, false), _diag1(2 * BOARD_SIZE - 1, false),
      _diag2(2 * BOARD_SIZE - 1, false)
{
}

int Queen::solve()
{
    _solutions.clear();
    backtrack(0);
    return static_cast<int>(_solutions.size());
}

void Queen::backtrack(const int row)
{
    // 如果已经成功放置了所有行的皇后，保存这个解
    if (row == BOARD_SIZE)
    {
        _solutions.push_back(_positions);
        return;
    }

    // 尝试在当前行的每一列放置皇后
    for (int col = 0; col < BOARD_SIZE; ++col)
    {
        if (can_place(row, col))
        {
            // 放置皇后
            _positions[row] = col;
            _cols[col] = true;
            _diag1[row - col + BOARD_SIZE - 1] = true;
            _diag2[row + col] = true;

            // 递归处理下一行
            backtrack(row + 1);

            // 移除皇后
            _positions[row] = -1;
            _cols[col] = false;
            _diag1[row - col + BOARD_SIZE - 1] = false;
            _diag2[row + col] = false;
        }
    }
}

bool Queen::can_place(const int row, const int col) const
{
    // 检查列是否被占用
    if (_cols[col])
    {
        return false;
    }

    // 检查正斜线是否被占用 (row - col 为常数)
    if (_diag1[row - col + BOARD_SIZE - 1])
    {
        return false;
    }

    // 检查反斜线是否被占用 (row + col 为常数)
    if (_diag2[row + col])
    {
        return false;
    }

    return true;
}

void Queen::print_board(const std::vector<int>& positions) const
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            if (positions[row] == col)
            {
                std::cout << "Q ";
            }
            else
            {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Queen::print_solution(const int solution_index) const
{
    if (solution_index < 0 || solution_index >= static_cast<int>(_solutions.size()))
    {
        std::cout << "无效的索引!" << std::endl;
        return;
    }

    std::cout << "解决方案 " << (solution_index + 1) << ":" << std::endl;
    print_board(_solutions[solution_index]);
}

void Queen::print_all_solutions() const
{
    std::cout << "所有解决方案: " << _solutions.size() << std::endl << std::endl;
    for (size_t i = 0; i < _solutions.size(); ++i)
    {
        std::cout << "解决方案 " << (i + 1) << ":" << std::endl;
        print_board(_solutions[i]);
    }
}

int Queen::get_solution_count() const
{
    return static_cast<int>(_solutions.size());
}
