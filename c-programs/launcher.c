/*
 * 启动程序（Launcher）
 * 把项目里的所有小工具整合到一个菜单中，选择编号即可运行。
 *
 * 知识点：system() 调用外部程序、菜单循环、清屏
 *
 * 编译：gcc launcher.c -o launcher.exe
 * 运行：launcher.exe
 *
 * 注意：请确保同目录下已编译好各个小工具的 exe。
 */

#include <stdio.h>
#include <stdlib.h>

/* 清屏：Windows 用 cls，类 Unix 用 clear */
#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

typedef struct {
    const char *name;   /* 菜单显示名称 */
    const char *cmd;    /* 要执行的命令 */
} Tool;

/* 工具列表：名称 与 对应的 exe 命令 */
static const Tool TOOLS[] = {
    {"猜数字游戏",     "guess_number.exe"},
    {"石头剪刀布",     "rps.exe"},
    {"井字棋",         "tic_tac_toe.exe"},
    {"21 点",          "blackjack.exe"},
    {"猜单词",         "guess_word.exe"},
    {"九九乘法表",     "multiplication_table.exe"},
    {"斐波那契数列",   "fibonacci.exe"},
    {"简单计算器",     "calculator.exe"},
    {"单位转换器",     "converter.exe"},
};

static const int TOOL_COUNT = (int)(sizeof(TOOLS) / sizeof(TOOLS[0]));

static void print_menu(void)
{
    printf("============================================\n");
    printf("            C 语言练手小工具启动器          \n");
    printf("============================================\n");
    for (int i = 0; i < TOOL_COUNT; i++) {
        printf("  %2d) %s\n", i + 1, TOOLS[i].name);
    }
    printf("   0) 退出\n");
    printf("============================================\n");
}

int main(void)
{
    int choice;

    while (1) {
        CLEAR_SCREEN();
        print_menu();
        printf("请输入要运行的工具编号：");

        if (scanf("%d", &choice) != 1) {
            printf("请输入数字。按回车继续...");
            while (getchar() != '\n')
                ;
            getchar();
            continue;
        }

        if (choice == 0) {
            printf("再见！\n");
            break;
        }

        if (choice < 1 || choice > TOOL_COUNT) {
            printf("无效的编号。按回车继续...");
            while (getchar() != '\n')
                ;
            getchar();
            continue;
        }

        /* 运行选中的工具 */
        CLEAR_SCREEN();
        printf(">>> 正在运行：%s <<<\n\n", TOOLS[choice - 1].name);
        int ret = system(TOOLS[choice - 1].cmd);

        printf("\n>>> %s 已结束（退出码 %d）<<<\n",
               TOOLS[choice - 1].name, ret);
        printf("按回车返回菜单...");
        while (getchar() != '\n')
            ;
        getchar();
    }

    return 0;
}
