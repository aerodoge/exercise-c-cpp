#include <stdbool.h>
#include <stdio.h>

#define BOARD_SIZE 50
#define MAX_MOVES 1000

// ANSI颜色代码
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_PURPLE  "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// 海龟
typedef struct {
    int x;
    int y;
    bool pen_down;
} Turtle;

// 位置
typedef struct {
    int x;
    int y;
} Position;

// 棋盘
char board[BOARD_SIZE][BOARD_SIZE];
// 移动轨迹
Position moves[MAX_MOVES];

int move_count = 0;

// 初始化棋盘：全部设为 '.'
void init_board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '.';
        }
    }
}

// 初始化乌龟位置和状态
void init_turtle(Turtle *turtle) {
    turtle->x = BOARD_SIZE / 2;
    turtle->y = BOARD_SIZE / 2;
    turtle->pen_down = true;
    board[turtle->y][turtle->x] = '*';
}

// 检查新位置是否在边界内
bool is_valid_position(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// 移动海龟
bool move_turtle(Turtle *turtle, const char direction) {
    int new_x = turtle->x;
    int new_y = turtle->y;

    switch (direction) {
    case 'w':
    case 'W':
        new_y--;
        break;
    case 's':
    case 'S':
        new_y++;
        break;
    case 'a':
    case 'A':
        new_x--;
        break;
    case 'd':
    case 'D':
        new_x++;
        break;
    default:
        printf("方向输入错误! W-上移1步/A-左移1步/S-下移1步/D-右移1步\n");
        return false;
    }

    if (!is_valid_position(new_x, new_y)) {
        printf("不能走出边界!\n");
        return false;
    }
    // 更新位置
    turtle->x = new_x;
    turtle->y = new_y;
    // 画出路径
    if (turtle->pen_down) {
        board[turtle->y][turtle->x] = '*';
    }

    if (move_count < MAX_MOVES) {
        moves[move_count].x = turtle->x;
        moves[move_count].y = turtle->y;
        move_count++;
    }

    return true;
}

// 打印棋盘
void print_board() {
    printf("\n  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%3d", i);
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == '*') {
                printf("  " ANSI_COLOR_RED "%c" ANSI_COLOR_RESET, board[i][j]);
            } else {
                printf("  %c", board[i][j]);
            }
        }
        printf("\n");
    }
}

// 打印移动历史
void print_moves() {
    printf("\n移动轨迹:\n");
    for (int i = 0; i < move_count; i++) {
        printf("移动 %d: (%d, %d)\n", i + 1, moves[i].x, moves[i].y);
    }
}

// 切换画笔
void toggle_pen(Turtle *turtle) {
    turtle->pen_down = !turtle->pen_down;
    printf("Pen is now %s\n",
           turtle->pen_down ? "放下 (画)" : "抬起 (不画)");
}

// 打印帮助信息
void print_help() {
    printf("\n命令:\n");
    printf("W/w - 上移1步\n");
    printf("S/s - 下移1步\n");
    printf("A/a - 左移1步\n");
    printf("D/d - 右移1步\n");
    printf("P/p - 切换画笔(up/down)\n");
    printf("H/h - 帮助\n");
    printf("M/m - 移动轨迹\n");
    printf("Q/q - 退出\n");
}

int main() {
    // 创建海龟
    Turtle turtle ={.x = 0, .y =0, .pen_down = false};
    // 初始化棋盘：将棋盘的二维数组赋初值
    init_board();
    // 初始化海龟：设置起始位置，画笔状态
    init_turtle(&turtle);

    printf("欢迎来到海龟图形\n");
    printf("棋盘大小: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("起始位置: (%d, %d)\n", turtle.x, turtle.y);
    print_help();

    // 输入的命令
    char command;
    while (1) {
        print_board();
        printf("\n当前位置: (%d, %d) - Pen: %s\n", turtle.x, turtle.y,
               turtle.pen_down ? "放下" : "抬起");
        printf("输入命令: ");
        scanf(" %c", &command);
        switch (command) {
        case 'w':
        case 'W':
        case 's':
        case 'S':
        case 'a':
        case 'A':
        case 'd':
        case 'D':
            // 移动海龟
            move_turtle(&turtle, command);
            break;
        case 'p':
        case 'P':
            // 抬起/放下画笔
            toggle_pen(&turtle);
            break;
        case 'h':
        case 'H':
            // 帮助信息
            print_help();
            break;
        case 'm':
        case 'M':
            // 移动轨迹
            print_moves();
            break;
        case 'q':
        case 'Q':
            printf("程序退出，欢迎再次使用海龟图形!\n");
            return 0;
        default:
            printf("未知命令! 按 H 获取帮助.\n");
            break;
        }
    }
}