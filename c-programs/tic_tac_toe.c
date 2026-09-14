/*
 * 井字棋（人机对战）
 * 你是 X，电脑是 O，位置编号 1-9（从上到下、从左到右）。
 * 编译：gcc tic_tac_toe.c -o tic_tac_toe.exe -finput-charset=UTF-8 -fexec-charset=GBK
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char board[9];

static void reset_board(void)
{
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }
}

static void print_board(void)
{
    printf("\n");
    for (int r = 0; r < 3; r++) {
        printf("  %c | %c | %c \n", board[r * 3], board[r * 3 + 1], board[r * 3 + 2]);
        if (r < 2) {
            printf(" ---+---+---\n");
        }
    }
    printf("\n");
}

/* 判断某个棋子是否连成一线 */
static int check_win(char p)
{
    static const int lines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},   /* 三行 */
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},   /* 三列 */
        {0, 4, 8}, {2, 4, 6}               /* 两条对角线 */
    };
    for (int i = 0; i < 8; i++) {
        if (board[lines[i][0]] == p &&
            board[lines[i][1]] == p &&
            board[lines[i][2]] == p) {
            return 1;
        }
    }
    return 0;
}

static int is_full(void)
{
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            return 0;
        }
    }
    return 1;
}

/* 电脑走棋：优先获胜，其次堵玩家，再次占中心/角，最后随机空位 */
static void ai_move(void)
{
    /* 1. 自己能赢就走 */
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'O';
            if (check_win('O')) return;
            board[i] = ' ';
        }
    }
    /* 2. 堵玩家 */
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'X';
            if (check_win('X')) {
                board[i] = 'O';
                return;
            }
            board[i] = ' ';
        }
    }
    /* 3. 占中心 */
    if (board[4] == ' ') {
        board[4] = 'O';
        return;
    }
    /* 4. 占角 */
    static const int corners[4] = {0, 2, 6, 8};
    for (int i = 0; i < 4; i++) {
        if (board[corners[i]] == ' ') {
            board[corners[i]] = 'O';
            return;
        }
    }
    /* 5. 随机空位 */
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'O';
            return;
        }
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));
    printf("===== 井字棋 =====\n");
    printf("你是 X，电脑是 O。输入位置编号 1-9 落子。\n");

    while (1) {
        reset_board();

        /* 随机决定谁先手 */
        if (rand() % 2 == 0) {
            printf("\n电脑先手！\n");
            ai_move();
        }

        while (1) {
            print_board();

            int pos;
            printf("你的回合，输入位置 (1-9)：");
            if (scanf("%d", &pos) != 1 || pos < 1 || pos > 9 || board[pos - 1] != ' ') {
                printf("无效位置，请重新输入。\n");
                while (getchar() != '\n')
                    ;
                continue;
            }
            board[pos - 1] = 'X';

            if (check_win('X')) {
                print_board();
                printf("你赢了！\n");
                break;
            }
            if (is_full()) {
                print_board();
                printf("平局！\n");
                break;
            }

            ai_move();
            if (check_win('O')) {
                print_board();
                printf("电脑赢了。\n");
                break;
            }
            if (is_full()) {
                print_board();
                printf("平局！\n");
                break;
            }
        }

        char again;
        printf("再来一局吗？(y/n)：");
        scanf(" %c", &again);
        if (again != 'y' && again != 'Y') {
            break;
        }
    }

    printf("再见！\n");
    return 0;
}
