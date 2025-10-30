// 骑士之旅
#ifndef KNIGHT_TOUR_KNIGHT_H
#define KNIGHT_TOUR_KNIGHT_H

#include <cstdio>
#include <iostream>

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
class Knight {
  public:
    // 构造函数
    Knight(const int x, const int y);
    // 打印位置
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
    // 构造函数
    KnightTour(const int x, const int y);
    // 无参构造函数
    KnightTour();
    // 打印棋盘
    void print_board() const;
    // 边界检查
    bool valid_position(const int x, const int y) const;
    // 能走该位置
    bool can_visit(const int x, const int y) const;
    // 求解骑士之旅
    bool solve_tour(const int x, const int y, const int steps);
    // 开始
    void start();

    static constexpr int BOARD_SIZE = 8;

  private:
    int _board[BOARD_SIZE][BOARD_SIZE]{};
    Knight _knight{0, 0};
};

#endif // KNIGHT_TOUR_KNIGHT_H