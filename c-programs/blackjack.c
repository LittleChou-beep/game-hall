/*
 * 21 点（Blackjack）
 * 规则：尽量接近 21 点，超过就爆牌。J/Q/K=10，A=11（简化处理）。
 * 编译：gcc blackjack.c -o blackjack.exe -finput-charset=UTF-8 -fexec-charset=GBK
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 抽一张牌，返回点数 */
static int draw_card(void)
{
    int c = rand() % 13 + 1; /* 1~13 */
    if (c > 10) return 10;   /* J/Q/K 记 10 */
    if (c == 1) return 11;   /* A 记 11 */
    return c;
}

int main(void)
{
    srand((unsigned int)time(NULL));
    printf("===== 21 点（Blackjack）=====\n");
    printf("规则：尽量接近 21 点，超过就爆牌。J/Q/K=10，A=11。\n");

    char again;
    do {
        int player = draw_card() + draw_card();
        int dealer = draw_card(); /* 庄家先亮一张 */

        printf("\n你的点数：%d\n", player);
        printf("庄家明牌：%d\n", dealer);

        /* 玩家回合 */
        char choice;
        while (player < 21) {
            printf("要牌(h)还是停牌(s)？");
            scanf(" %c", &choice);
            if (choice == 'h' || choice == 'H') {
                int c = draw_card();
                player += c;
                printf("抽到 %d 点，你的点数：%d\n", c, player);
            } else {
                break;
            }
        }

        if (player > 21) {
            printf("爆牌了！你输了。\n");
        } else {
            /* 庄家补牌到 >= 17 */
            while (dealer < 17) {
                dealer += draw_card();
            }
            printf("庄家点数：%d\n", dealer);

            if (dealer > 21) {
                printf("庄家爆牌，你赢了！\n");
            } else if (player > dealer) {
                printf("你赢了！\n");
            } else if (player < dealer) {
                printf("你输了。\n");
            } else {
                printf("平局。\n");
            }
        }

        printf("再来一局吗？(y/n)：");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    printf("再见！\n");
    return 0;
}
