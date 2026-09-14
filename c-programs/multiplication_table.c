/*
 * 九九乘法表
 * 知识点：for 循环嵌套、printf 格式化输出
 * 编译：gcc multiplication_table.c -o multiplication_table.exe
 */

#include <stdio.h>

int main(void)
{
    int i, j;

    printf("================ 九九乘法表 ================\n");
    for (i = 1; i <= 9; i++) {
        for (j = 1; j <= i; j++) {
            printf("%d × %d = %-2d  ", j, i, i * j);
        }
        printf("\n");
    }
    printf("============================================\n");

    return 0;
}
