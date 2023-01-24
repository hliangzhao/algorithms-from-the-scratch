//
// Created by hliangzhao on 23/1/23.
//
#include <iostream>
#include <ctime>

using namespace std;

// ------------------------------------------------------------------------------------------

/**
 * n 皇后问题：输入 n，代表 n \times n 的棋盘和 n 个皇后。
 * 要求任意两个皇后不同行、不同列，也不在同一条斜线上，计算总共有多少种摆法。
 * */

// ------------------------------------------------------------------------------------------

/**
 * 方法一：暴力递归（回溯）。
 * 每次找到一个合法解后，回到最近一步尚未探索过的步骤中，去测试其他放置方案是否合法。
 * 复杂度是 O(n!)
 * */

int n_queens1(int n);

int proc1(int i, int *records, int n);

int valid1(const int *records, int i, int j);

int n_queens1(int n) {
    if (n < 1) {
        return 0;
    }
    /// 每个皇后必然一行一个。用大小为 n 的一位数组来记录。records[i] 表示第 i 个皇后所在的列
    int *records = new int[n];
    return proc1(0, records, n);
}

int proc1(int i, int *records, int n) {
    /// 全部放完，这是一个合法解
    if (i == n) {
        return 1;
    }
    int res = 0;
    for (int j = 0; j < n; j++) {
        if (valid1(records, i, j)) {
            records[i] = j;
            res += proc1(i + 1, records, n);
        }
    }
    return res;
}

int valid1(const int *records, int i, int j) {
    for (int k = 0; k < i; k++) {
        /// 只需检查是否共列和共斜线
        /// 后者：判断行列 gap 是否相等，相等则是 45 度斜线
        if (j == records[k] || abs(records[k] - j) == abs(i - k)) {
            return false;
        }
    }
    return true;
}

// ------------------------------------------------------------------------------------------

/**
 * 方法二：仍然是暴力递归，但使用位运算加速。
 * */

int n_queens2(int n);

int proc2(int limit, int col_lim, int left_diag_lim, int right_diag_lim);

int n_queens2(int n) {
    /// 使用 int 数据类型进行位运算，因此 n 不要超过 32 位
    if (n < 1 || n > 32) {
        return 0;
    }
    /// 申请一个整型数，该数后 n 位均为 1，前 32-n 位均为 0
    /// 为 1 的位置是可以去探索的列
    int limit = n == 32 ? -1 : (1 << n) - 1;
    return proc2(limit, 0, 0, 0);
}

/**
 * 列、左右斜线上的限制，对应位为 1 则不能放皇后
 * */
int proc2(int limit, int col_lim, int left_diag_lim, int right_diag_lim) {
    if (col_lim == limit) {
        /// 找到一个合法解
        return 1;
    }
    int pos = 0, most_right_one = 0;
    /// 此时，pos 最右 n 列中，为 1 的位置，表示对于当前皇后而言，可以放的合法位置
    pos = limit & (~(col_lim | left_diag_lim | right_diag_lim));

    int res = 0;
    while (pos != 0) {
        /// pos 最右的 1 保留，其余全变成 0，这种位模式对应的数就是 most_right_one
        most_right_one = pos & (~pos + 1);
        /// pos 最右的 1 被选中，这就是本皇后被放置的列
        pos = pos - most_right_one;
        res += proc2(limit, col_lim | most_right_one, (left_diag_lim | most_right_one) << 1,
                     /// 应当采用逻辑右移
                     (unsigned) (right_diag_lim | most_right_one) >> 1);
    }
    return res;
}

// ------------------------------------------------------------------------------------------

int main() {
    ::clock_t start, end;
    start = ::clock();
    cout << n_queens1(14) << endl;
    end = ::clock();
    cout << "finished in " << (double) (end - start) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    start = ::clock();
    cout << n_queens2(14) << endl;
    end = ::clock();
    cout << "finished in " << (double) (end - start) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    return 0;
}
