/*
 * 斐波那契数列
 * 知识点：函数、递归、迭代、数组
 * 编译：gcc fibonacci.c -o fibonacci.exe
 *
 * 数列：0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
 */

#include <stdio.h>

/* 递归实现（简单但 n 大时很慢，且会重复计算） */
static long long fib_recursive(int n)
{
    if (n < 2) {
        return n;
    }
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

/* 迭代实现（推荐，时间 O(n)，空间 O(1)） */
static long long fib_iterative(int n)
{
    if (n < 2) {
        return n;
    }
    long long prev = 0, curr = 1, next;
    for (int i = 2; i <= n; i++) {
        next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main(void)
{
    int n;

    printf("请输入要计算到第几项斐波那契数（建议不超过 45，因为递归会很慢）：");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("输入无效，请输入一个非负整数。\n");
        return 1;
    }

    printf("\n前 %d 项斐波那契数列（迭代法）：\n", n + 1);
    for (int i = 0; i <= n; i++) {
        printf("F(%d) = %lld\n", i, fib_iterative(i));
    }

    if (n <= 45) {
        printf("\n第 %d 项（递归法）：F(%d) = %lld\n", n, n, fib_recursive(n));
    } else {
        printf("\n（n 较大，跳过递归法演示，避免等待过久。）\n");
    }

    return 0;
}
