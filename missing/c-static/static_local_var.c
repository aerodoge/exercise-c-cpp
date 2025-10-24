#include <stdio.h>

extern int internal_var;
extern void helper(void);

void counter(void)
{
    static int counter = 0;
    counter++;
    printf("counter = %d\n", counter);
}

int main(void)
{
    counter();
    counter();
    counter();

    // 无法链接外部静态静态函数
    // helper();
    // 无法链接外部静态变量
    // printf("internal_var = %d\n", internal_var);

    return 0;
}