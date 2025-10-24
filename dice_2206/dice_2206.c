#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int rollDice() {
    return rand() % 6 + 1;
}

int main() {
    srand(time(NULL));

    int dice1 = rollDice();
    int dice2 = rollDice();
    int sum = dice1 + dice2;
    printf("第一个骰子: %d,第二个骰子: %d,点数之和: %d\n", dice1,dice2,sum);

    switch(sum) {
    case 7:
    case 11:
        printf("玩家获胜！游戏结束。\n");
        return 0;
    case 2:
    case 3:
    case 12:
        printf("玩家失利！游戏结束。\n");
        return 0;
    case 4:
    case 5:
    case 6:
    case 8:
    case 9:
    case 10:
        printf("建立点数: %d\n", sum);
        int point = sum;

        while(true) {
            printf("按回车键继续投掷...\n");
            getchar();

            dice1 = rollDice();
            dice2 = rollDice();
            sum = dice1 + dice2;
            printf("第一个骰子: %d,第二个骰子: %d,点数之和: %d\n", dice1,dice2,sum);

            if(sum == point) {
                printf("投出点数 %d！玩家获胜！游戏结束。\n", point);
                return 0;
            }
            if(sum == 7) {
                printf("投出7！玩家失利！游戏结束。\n");
                return 0;
            }
        }
    default:
        printf("错误的点数\n");
        break;
    }

    return 0;
}
