/*
 * 单位转换器
 * 知识点：函数封装、菜单、switch
 * 编译：gcc converter.c -o converter.exe
 *
 * 支持：温度（摄氏/华氏/开尔文）、长度（米/英尺）、重量（千克/磅）
 */

#include <stdio.h>

/* ---------- 温度 ---------- */
static double c2f(double c) { return c * 9.0 / 5.0 + 32.0; }
static double f2c(double f) { return (f - 32.0) * 5.0 / 9.0; }
static double c2k(double c) { return c + 273.15; }
static double k2c(double k) { return k - 273.15; }

/* ---------- 长度 ---------- */
static double m2ft(double m) { return m * 3.28084; }
static double ft2m(double ft) { return ft / 3.28084; }

/* ---------- 重量 ---------- */
static double kg2lb(double kg) { return kg * 2.20462; }
static double lb2kg(double lb) { return lb / 2.20462; }

static void print_menu(void)
{
    printf("\n========== 单位转换器 ==========\n");
    printf("  1) 摄氏 -> 华氏\n");
    printf("  2) 华氏 -> 摄氏\n");
    printf("  3) 摄氏 -> 开尔文\n");
    printf("  4) 开尔文 -> 摄氏\n");
    printf("  5) 米   -> 英尺\n");
    printf("  6) 英尺 -> 米\n");
    printf("  7) 千克 -> 磅\n");
    printf("  8) 磅   -> 千克\n");
    printf("  0) 退出\n");
    printf("================================\n");
}

int main(void)
{
    int choice;
    double value, result;

    while (1) {
        print_menu();
        printf("请选择功能 (0-8)：");
        if (scanf("%d", &choice) != 1) {
            printf("请输入数字。\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (choice == 0) {
            break;
        }

        if (choice < 1 || choice > 8) {
            printf("无效的选项。\n");
            continue;
        }

        printf("请输入数值：");
        if (scanf("%lf", &value) != 1) {
            printf("请输入数字。\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice) {
        case 1:
            result = c2f(value);
            printf("%.4g °C = %.4g °F\n", value, result);
            break;
        case 2:
            result = f2c(value);
            printf("%.4g °F = %.4g °C\n", value, result);
            break;
        case 3:
            result = c2k(value);
            printf("%.4g °C = %.4g K\n", value, result);
            break;
        case 4:
            result = k2c(value);
            printf("%.4g K = %.4g °C\n", value, result);
            break;
        case 5:
            result = m2ft(value);
            printf("%.4g m = %.4g ft\n", value, result);
            break;
        case 6:
            result = ft2m(value);
            printf("%.4g ft = %.4g m\n", value, result);
            break;
        case 7:
            result = kg2lb(value);
            printf("%.4g kg = %.4g lb\n", value, result);
            break;
        case 8:
            result = lb2kg(value);
            printf("%.4g lb = %.4g kg\n", value, result);
            break;
        }
    }

    printf("再见！\n");
    return 0;
}
