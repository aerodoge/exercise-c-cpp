#include <stdio.h>

// 功能：安全读取整数输入
// 参数：
//    prompt：提示信息
//    value：存储读取结果的指针
// 返回值：1-成功，0-失败
int safe_input_int(const char *prompt, int *value) {
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        // 清空输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("无效输入，请输入数字\n");
        return 0;
    }
    return 1;
}

typedef enum ClassType { CLASS_FIRST = 1, CLASS_ECONOMY = 2 } ClassType;

// 功能：打印座位信息
// 参数：
//    seats：座位数组
//    size：座位个数
void show_seats(const int seats[], int size) {
    printf("座位信息:\n");
    for (int i = 0; i < size; i++) {
        printf("座位 %02d: %s\n", i + 1, seats[i] == 0 ? "[✓]" : "[x]");
    }
}

// 功能：判断头等舱满座
// 参数：座位数组
// 返回值：0-没满，1-满
int first_class_full(const int seats[]) {
    for (int i = 0; i < 5; i++) {
        if (seats[i] == 0) {
            return 0;
        }
    }
    return 1;
}

// 功能：判断经济舱满座
// 参数：座位数组
// 返回值：0-没满，1-满
int economy_full(const int seats[]) {
    for (int i = 5; i < 10; i++) {
        if (seats[i] == 0) {
            return 0;
        }
    }
    return 1;
}

// 功能：判断是否全部满座
// 参数：座位数组
// 返回值：0-没满，1-满
int seats_full(const int seats[]) { return first_class_full(seats) && economy_full(seats); }

// 功能：检查用户自选座位号
// 参数：
//    seat_num：座位号
//    type：等级
// 返回值：0-无效，1-有效
int check_seat_num(const int seat_num, const ClassType type) {
    switch (type) {
    case CLASS_FIRST:
        return (seat_num >= 1) && (seat_num <= 5);
    case CLASS_ECONOMY:
        return (seat_num >= 6) && (seat_num <= 10);
    }
    return 0;
}

// 功能：出票
// 参数：
//    seats: 座位数组
//    type: 1-头等舱，2-经济舱
// 返回值：座位号
int sell_ticket(int seats[], const ClassType type) {
    int choice = -1;
    if (!safe_input_int("您是自选还是系统分配 (0-系统分配, 1-自选):", &choice)) {
        return 0;
    }
    if (choice != 1 && choice != 0) {
        printf("选择错误，请输入0或1\n");
        return 0;
    }
    int seat_num = 0; // 座位号
    if (choice == 1) {
        // 自选
        int input = 0; // 用户输入的座位号
        if (!safe_input_int("请输入座位号:", &input)) {
            return 0;
        }
        if (!check_seat_num(input, type)) {
            // 用户选择的座位号错误
            return 0;
        }
        seat_num = input;     // 用户选择有效，赋给seat_num
        seats[input - 1] = 1; // 座位已被选择，数组对应位置置1
    } else {
        // 系统分配
        switch (type) {
        case CLASS_FIRST: // 头等舱
            for (int i = 0; i < 5; i++) {
                if (seats[i] == 0) {         // 座位可选
                    seat_num = i + 1;        // 座位号：数组下标加1
                    seats[seat_num - 1] = 1; // 数组中对应位置置1
                    break;
                }
            }
            break;
        case CLASS_ECONOMY: // 经济舱
            for (int i = 5; i < 10; i++) {
                if (seats[i] == 0) {
                    seat_num = i + 1;
                    seats[seat_num - 1] = 1;
                    break;
                }
            }
            break;
        }
    }

    return seat_num;
}

// 功能：处理头等舱
// 参数：座位数组
// 返回值：座位号
int handle_first_class(int seats[]) {
    int seat_num = 0;
    if (first_class_full(seats)) {
        // 头等舱满座
        int choice = -1;
        if (!safe_input_int("头等舱已满，您是否选择经济舱（0-不选择, 1-选择）:", &choice)) {
            return seat_num;
        }
        if (choice != 1 && choice != 0) {
            printf("选择错误, 请输入0或1\n");
            return seat_num;
        }
        if (choice == 1) { // 选经济舱
            // 这里不用判断经济舱是否满，因为每轮开头会判断所有座位是否满
            // 如果头等舱满，经济舱一定有座位
            seat_num = sell_ticket(seats, CLASS_ECONOMY);
        }
    } else {
        // 头等舱有位置
        seat_num = sell_ticket(seats, CLASS_FIRST);
    }

    return seat_num;
}

// 功能：处理经济舱
// 参数：座位数组
// 返回值：座位号
int handle_economy(int seats[]) {
    int seat_num = 0;
    if (economy_full(seats)) {
        // 经济舱满座
        int choice = -1;
        if (!safe_input_int("经济舱已满,您是否选择头等舱 (0-不选择,1-选择):", &choice)) {
            return seat_num;
        }
        if (choice != 1 && choice != 0) {
            printf("选择错误, 请输入0或1\n");
            return seat_num;
        }
        if (choice == 1) { // 选头等舱
            // 这里不用判断头等舱是否满，因为每轮开头会判断所有座位是否满
            // 如果经济舱满，头等舱一定有座位
            seat_num = sell_ticket(seats, CLASS_FIRST);
        }
    } else {
        // 经济舱有位置
        seat_num = sell_ticket(seats, CLASS_ECONOMY);
    }

    return seat_num;
}

int main() {
    // 10个座位，0表示空闲，1表示已售出
    int seats[10] = {0};

    while (1) {
        if (seats_full(seats)) {
            printf("非常抱歉, 本次航班已售完, 下次航班3小时之后起飞!\n");
            return 0;
        }
        // 打印座位信息
        show_seats(seats, 10);
        // 用户的选择
        int choice = 0;
        // 机票座位号
        int seat_num = 0;
        if (!safe_input_int("请选择 (1-头等舱, 2-经济舱, 3-退出):", &choice)) {
            continue;
        }
        switch (choice) {
        case 1:
            // 用户选头等舱
            seat_num = handle_first_class(seats);
            break;
        case 2:
            // 用户选经济舱
            seat_num = handle_economy(seats);
            break;
        case 3:
            printf("退出系统成功, 欢迎乘坐!\n");
            return 0;
        default:
            printf("无效的选择, 请重新选择\n");
            continue;
        }
        if (seat_num >= 1 && seat_num <= 10) {
            // 座位号大于0(1-10号)出票成功，打印机票信息
            printf(" -------------------------------------\n");
            printf("| 您的登记证, 等级: %s, 座位号: %d |\n", choice == 1 ? "头等舱" : "经济舱",
                   seat_num);
            printf(" -------------------------------------\n");
        } else {
            // 出票失败，让用户再次选择
            printf("出票失败, 请重新选择!\n");
        }
    }
}
