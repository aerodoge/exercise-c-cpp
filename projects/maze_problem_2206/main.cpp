#include <array>
#include <iostream>

#include "include/maze.hpp"
#include "include/maze_generator.hpp"

int main()
{
    std::array<std::array<char, 12>, 12> mazeGrid{};
    Position start{};
    Position end{};

    std::cout << "=========================================\n";
    std::cout << "      迷宫程序\n";
    std::cout << "=========================================\n";

    // 选择迷宫来源
    std::cout << "\n选择迷宫来源:\n";
    std::cout << "1. 使用预定义迷宫\n";
    std::cout << "2. 自动生成随机迷宫\n";
    std::cout << "请输入选项 (1 或 2): ";

    int mazeChoice;
    std::cin >> mazeChoice;

    if (mazeChoice == 2)
    {
        // 生成随机迷宫
        std::cout << "\n正在生成随机迷宫...\n";
        MazeGenerator<12> generator;
        mazeGrid = generator.generate(start, end);
        std::cout << "迷宫生成完成！\n";
    }
    else
    {
        // 使用预定义迷宫
        mazeGrid = {{{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
                     {'#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#'},
                     {'#', '.', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#'},
                     {'#', '#', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#'},
                     {'#', '.', '.', '.', '.', '#', '#', '#', '.', '#', '.', '.'},
                     {'#', '#', '#', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
                     {'#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
                     {'#', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
                     {'#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
                     {'#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '.', '#'},
                     {'#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#'},
                     {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}}};
        start = {1, 1}; // 起点
        end = {4, 11};  // 终点（右边界的出口）
    }

    Maze<12> maze(mazeGrid, start, end);

    // 选择算法
    std::cout << "\n选择遍历算法:\n";
    std::cout << "1. 右手法则（靠右墙行走）\n";
    std::cout << "2. 深度优先搜索（DFS）\n";
    std::cout << "请输入选项 (1 或 2): ";

    int choice;
    std::cin >> choice;

    bool useRightHand = (choice == 1);
    maze.solve(useRightHand);

    std::cout << "\n图例:\n";
    std::cout << "  # - 墙壁\n";
    std::cout << "  . - 可行路径\n";
    std::cout << "  S - 起点\n";
    std::cout << "  E - 终点\n";
    std::cout << "  X - 探索的路径\n";
    std::cout << "  o - 回溯的路径\n";

    return 0;
}