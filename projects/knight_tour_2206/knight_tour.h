// 骑士之旅
#ifndef KNIGHT_TOUR_KNIGHT_H
#define KNIGHT_TOUR_KNIGHT_H


#include <iostream>
#include <cstdio>

// 位置
struct Position {
    // 构造函数
    Position(const int x, const int y);
    // 打印位置
    void print() const;

    int _x;
    int _y;
};

// 骑士
class Knight
{
public:
    Knight(const int x, const int y);
    void print_position() const;
    // 上走2格，左走1格
    void up_left();
    // 上走2格，右走1格
    void up_right();
    // 下走2格，左走1格
    void down_left();
    // 下走2格，右走1格
    void down_right();
    // 左走2格，上走1格
    void left_up();
    // 左走2格，下走1格
    void left_down();
    // 右走2格，上走1格
    void right_up();
    // 右走2格，下走1格
    void right_down();
private:
    Position _position;
};

// 骑士之旅
class KnightTour {
public:
    static constexpr int BOARD_SIZE = 8;

    KnightTour(const int x, const int y);

    void print_board() {
        for (auto & i : _board) {
            for (const int j : i) {
                if (j == 0) {
                    std::cout << " . ";
                } else {
                    printf("%02d ", j);
                }
            }
            std::cout << std::endl;
        }
    }

    // 边界检查
    bool is_valid_position(const int x, const int y) const;

    // 位置是否未被访问
    bool is_safe(const int x, const int y) const;

    // 求解骑士之旅
    bool solve_tour(const int x, const int y, const int move_count);

    void start();

private:
    int _board[BOARD_SIZE][BOARD_SIZE]{};
    Knight _knight;
};


#endif //KNIGHT_TOUR_KNIGHT_H