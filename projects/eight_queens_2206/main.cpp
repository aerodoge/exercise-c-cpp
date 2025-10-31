#include "queen.h"

int main()
{
    Queen queen;
    std::cout << "正在求解8皇后问题..." << std::endl << std::endl;
    int count = queen.solve();
    std::cout << "找到 " << count << " 个解" << std::endl << std::endl;
    // 打印前3个解作为示例
    if (count > 0)
    {
        std::cout << "以下是前3个解的示例：" << std::endl << std::endl;
        for (int i = 0; i < std::min(3, count); ++i)
        {
            queen.print_solution(i);
        }
    }

    // 如果需要查看所有解，可以取消注释下面这行
    // queen.print_all_solutions();

    return 0;
}