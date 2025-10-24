#include <stdio.h>

int add(int a, int b) { return a + b; }

static inline int add_inline(int a, int b) { return a + b; }

int main(void) {
    int x = add(1, 2);        // 产生 call 指令
    int y = add_inline(1, 2); // 可能直接展开为 y = 3 + 4
    printf("x = %d, y = %d\n", x, y);
    return 0;
}