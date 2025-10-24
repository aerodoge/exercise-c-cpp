#include <stdio.h>
static int internal_var = 42;  // 只在 file1.c 可见

static void helper(void) {      // 只在 file1.c 可见
    printf("helper\n");
}

void public_func(void) {
    helper();  // 可以调用
}
