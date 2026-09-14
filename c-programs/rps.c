/*
 * 石头剪刀布游戏
 * 知识点：枚举、随机数、条件分支
 * 编译：gcc rps.c -o rps.exe
 *
 * 0=剪刀, 1=石头, 2=布
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const char *NAME[3] = {"剪刀", "石头", "布"};

/* 返回 1=玩家赢, 0=平局, -1=电脑赢 */
static int judge(int player, int computer)
{
    if (player == computer) {
        return 0;
    }
    /* 玩家赢的情况：剪刀赢布(0>2)、石头赢剪刀(1>0)、布赢石头(2>1) */
    if ((player == 0 && computer == 2) ||
        (player == 1 && computer == 0) ||
        (player == 2 && computer == 1)) {
        return 1;
    }
    return -1;
}

int main(void)
{
    int player, computer;
    int win = 0, lose = 0, draw = 0;
    char again;

    srand((unsigned int)time(NULL));

    printf("===== 石头剪刀布 =====\n");
    printf("0 = 剪刀, 1 = 石头, 2 = 布\n");

    do {
        printf("\n请出招 (0/1/2)：");
        if (scanf("%d", &player) != 1 || player < 0 || player > 2) {
            printf("输入无效，请输入 0、1 或 2。\n");
            /* 清空输入缓冲区 */
            while (getchar() != '\n')
                ;
            continue;
        }

        computer = rand() % 3;
        printf("你出：%s  |  电脑出：%s\n", NAME[player], NAME[computer]);

        int result = judge(player, computer);
        if (result == 1) {
            printf("你赢了！\n");
            win++;
        } else if (result == 0) {
            printf("平局。\n");
            draw++;
        } else {
            printf("你输了。\n");
            lose++;
        }

        printf("当前战绩：胜 %d  平 %d  负 %d\n", win, draw, lose);

        printf("再来一局吗？(y/n)：");
        scanf(" %c", &again); /* 前面的空格跳过换行 */
    } while (again == 'y' || again == 'Y');

    printf("\n最终战绩：胜 %d  平 %d  负 %d，再见！\n", win, draw, lose);
    return 0;
}
