/*
 * 猜数字游戏（C 语言版）
 * 规则：程序随机生成一个 1~100 的数字，玩家来猜，程序提示"大了"或"小了"。
 *
 * 编译（MinGW / GCC）：gcc guess_number.c -o guess_number.exe
 * 运行：              guess_number.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * 读取一行输入并解析为整数。
 * 返回值：1 = 解析成功；0 = 输入不是合法整数（可重新输入）；-1 = 输入结束（EOF）
 */
static int read_int(int *out)
{
    char line[64];
    char extra;

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1; /* 输入结束（如 Ctrl+Z） */
    }

    /* %d 后面跟一个字符，用来判断数字后面是否还有多余内容 */
    if (sscanf(line, "%d %c", out, &extra) == 1) {
        return 1;
    }
    return 0;
}

/* 玩一局 */
static void play_round(void)
{
    int answer = rand() % 100 + 1; /* 1 ~ 100 */
    int attempts = 0;
    int guess;

    printf("\n我已经想好了一个 1 到 100 之间的数字，开始猜吧！\n");

    for (;;) {
        printf("请输入你猜的数字：");

        int rc = read_int(&guess);
        if (rc == -1) {
            printf("\n输入结束，本局退出。\n");
            return;
        }
        if (rc == 0) {
            printf("这不是一个有效的整数，请重新输入。\n");
            continue;
        }

        if (guess < 1 || guess > 100) {
            printf("数字要在 1 到 100 之间哦。\n");
            continue;
        }

        attempts++;

        if (guess < answer) {
            printf("小了，再往大猜猜~\n");
        } else if (guess > answer) {
            printf("大了，再往小猜猜~\n");
        } else {
            printf("恭喜你，猜对了！答案就是 %d，你一共猜了 %d 次。\n",
                   answer, attempts);
            return;
        }
    }
}

int main(void)
{
    char line[64];

    srand((unsigned int)time(NULL)); /* 用当前时间做随机数种子 */

    printf("===== 欢迎来到猜数字游戏 =====\n");

    for (;;) {
        play_round();

        printf("还想再玩一局吗？(y/n)：");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        if (line[0] != 'y' && line[0] != 'Y') {
            printf("游戏结束，再见！\n");
            break;
        }
    }

    return 0;
}
