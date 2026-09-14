/*
 * 猜单词
 * 猜一个英文单词（水果/动物/常见词），你有 8 次机会。
 * 编译：gcc guess_word.c -o guess_word.exe -finput-charset=UTF-8 -fexec-charset=GBK
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static const char *WORDS[] = {
    "apple", "banana", "cherry", "dragon", "eagle",
    "flower", "guitar", "hammer", "island", "jungle",
    "kitten", "lemon", "monkey", "orange", "planet",
    "rocket", "silver", "tiger", "winter", "yellow"
};
static const int WORD_COUNT = 20;

int main(void)
{
    srand((unsigned int)time(NULL));
    printf("===== 猜单词 =====\n");
    printf("猜一个英文单词（水果/动物/常见词）。你有 8 次机会。\n");

    char again;
    do {
        const char *word = WORDS[rand() % WORD_COUNT];
        int len = (int)strlen(word);
        int guessed[26] = {0};
        int remaining = 8;
        int solved = 0;

        while (remaining > 0 && !solved) {
            /* 显示当前进度 */
            solved = 1;
            printf("\n单词：");
            for (int i = 0; i < len; i++) {
                if (guessed[word[i] - 'a']) {
                    printf("%c ", word[i]);
                } else {
                    printf("_ ");
                    solved = 0;
                }
            }
            printf("   剩余次数：%d\n", remaining);

            printf("猜一个字母：");
            char c;
            if (scanf(" %c", &c) != 1) {
                printf("\n输入结束，本局退出。\n");
                return 0;
            }
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
            if (c < 'a' || c > 'z') {
                printf("请输入英文字母。\n");
                continue;
            }

            if (guessed[c - 'a']) {
                printf("你已经猜过这个字母了。\n");
                continue;
            }
            guessed[c - 'a'] = 1;

            int hit = 0;
            for (int i = 0; i < len; i++) {
                if (word[i] == c) hit = 1;
            }
            if (hit) {
                printf("猜中了！\n");
            } else {
                remaining--;
                printf("没有这个字母，剩余 %d 次。\n", remaining);
            }
        }

        if (solved) {
            printf("\n恭喜！单词就是 \"%s\"。\n", word);
        } else {
            printf("\n机会用完了，单词是 \"%s\"。\n", word);
        }

        printf("再来一局吗？(y/n)：");
        if (scanf(" %c", &again) != 1) {
            break;
        }
    } while (again == 'y' || again == 'Y');

    printf("再见！\n");
    return 0;
}
