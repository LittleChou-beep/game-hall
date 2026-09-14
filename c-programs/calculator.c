/*
 * 简单计算器
 * 知识点：switch、浮点运算、输入校验
 * 编译：gcc calculator.c -o calculator.exe
 */

#include <stdio.h>

int main(void)
{
    double a, b, result;
    char op;

    printf("===== 简单计算器 =====\n");
    printf("支持 + - * / 四种运算\n");
    printf("输入格式：数字 运算符 数字（例如 3 + 4）\n");

    while (1) {
        printf("\n请输入算式（输入 0 0 0 退出）：");

        if (scanf("%lf %c %lf", &a, &op, &b) != 3) {
            printf("输入格式有误，请按 \"数字 运算符 数字\" 的格式输入。\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (a == 0 && b == 0 && op == '0') {
            break;
        }

        int ok = 1;
        switch (op) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b == 0) {
                printf("错误：除数不能为 0。\n");
                ok = 0;
            } else {
                result = a / b;
            }
            break;
        default:
            printf("不支持的运算符：%c（仅支持 + - * /）\n", op);
            ok = 0;
        }

        if (ok) {
            printf("= %.6g\n", result);
        }
    }

    printf("计算器已退出。\n");
    return 0;
}
