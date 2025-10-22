/*
问题描述：

有三根柱子：A（起始柱）、B（辅助柱）、C（目标柱）
A柱上有n个不同大小的圆盘，从上到下按从小到大排列
目标是将所有圆盘从A柱移动到C柱

规则：

每次只能移动一个圆盘
大圆盘不能放在小圆盘上面
可以使用B柱作为辅助

解题思路：
移动n个圆盘的步骤可以分解为：

将上面的(n-1)个圆盘从A移到B（借助C）
将最大的圆盘从A移到C
将(n-1)个圆盘从B移到C（借助A）

*/
#include <stdio.h>

// 递归函数：移动n个圆盘从source到target，使用auxiliary作为辅助柱
void hanoi(int n, char source, char auxiliary, char target) {
    if (n == 1) {
        // 基本情况：只有一个圆盘，直接移动
        printf("将圆盘 1 从 %c 移动到 %c\n", source, target);
        return;
    }

    // 步骤1：将上面的(n-1)个圆盘从source移到auxiliary（借助target）
    hanoi(n - 1, source, target, auxiliary);

    // 步骤2：将最大的圆盘从source移到target
    printf("将圆盘 %d 从 %c 移动到 %c\n", n, source, target);

    // 步骤3：将(n-1)个圆盘从auxiliary移到target（借助source）
    hanoi(n - 1, auxiliary, source, target);
}

int main() {
    int n;

    printf("请输入圆盘数量: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("圆盘数量必须大于0\n");
        return 1;
    }

    printf("\n移动 %d 个圆盘的步骤:\n\n", n);
    hanoi(n, 'A', 'B', 'C');

    // 计算总步数
    int total_moves = (1 << n) - 1;
    printf("\n总共需要 %d 步\n", total_moves);

    return 0;
}

