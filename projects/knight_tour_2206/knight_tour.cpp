// 骑士之旅
#include "knight_tour.h"

// 构造函数
Position::Position(const int x, const int y) : _x(x), _y(y) {}

// 打印位置
void Position::print() const {
    std::cout << "(" << _x << "," << _y << ")" << std::endl;
}

// 构造函数
Knight::Knight(const int x, const int y) : _position(x, y) {}

// 打印骑士位置
void Knight::print_position() const { _position.print(); }

// 上走2格，左走1格
void Knight::up_left() {
    _position._y -= 2;
    _position._x--;
}

// 上走2格，右走1格
void Knight::up_right() {
    _position._y -= 2;
    _position._x++;
}

// 下走2格，左走1格
void Knight::down_left() {
    _position._y += 2;
    _position._x--;
}

// 下走2格，右走1格
void Knight::down_right() {
    _position._y += 2;
    _position._x++;
}

// 左走2格，上走1格
void Knight::left_up() {
    _position._x -= 2;
    _position._y--;
}

// 左走2格，下走1格
void Knight::left_down() {
    _position._x -= 2;
    _position._y++;
}

// 右走2格，上走1格
void Knight::right_up() {
    _position._x += 2;
    _position._y--;
}

// 右走2格，下走1格
void Knight::right_down() {
    _position._x += 2;
    _position._y++;
}

// 骑士之旅构造函数
KnightTour::KnightTour(const int x, const int y) : _knight(x, y) {
    // 初始化棋盘为0
    // for (auto & arr : _board) {
    //     for (int & j : arr) {
    //         j = 0;
    //     }
    // }
}

// 无参构造函数
KnightTour::KnightTour() {}

// 打印棋盘
void KnightTour::print_board() {
    for (int (&arr)[8] : _board) {
        for (const int j : arr) {
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
bool KnightTour::valid_position(const int x, const int y) const {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// 能走该位置
bool KnightTour::can_visit(const int x, const int y) const {
    return valid_position(x, y) && _board[x][y] == 0;
}

// 求解骑士之旅
bool KnightTour::solve_tour(const int x, const int y, const int steps) {
    // 标记当前位置
    _board[x][y] = steps;

    // 如果所有格子都被访问，成功
    if (steps == BOARD_SIZE * BOARD_SIZE) {
        return true;
    }

    // 8个可能的移动方向
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

    // 尝试所有8个方向
    for (int i = 0; i < 8; i++) {
        int next_x = x + dx[i];
        int next_y = y + dy[i];

        if (can_visit(next_x, next_y)) {
            if (solve_tour(next_x, next_y, steps + 1)) {
                return true;
            }
        }
    }

    // 撤销当前移动
    _board[x][y] = 0;

    return false;
}

// 开始
void KnightTour::start() {
    std::cout << "开始骑士之旅..." << std::endl;
    std::cout << "初始棋盘:" << std::endl;

    // 创建一个临时对象来演示
    KnightTour tour(0, 0);
    tour.print_board();

    std::cout << "\n正在求解..." << std::endl;

    if (tour.solve_tour(0, 0, 1)) {
        std::cout << "找到解决方案!" << std::endl;
        tour.print_board();
    } else {
        std::cout << "无解" << std::endl;
    }
}