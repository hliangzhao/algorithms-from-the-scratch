//
// Created by hliangzhao on 31/1/23.
//
#include <iostream>
#include <random>

using namespace std;

/**
 * 技巧一：滑动窗口
 * */

/**
 * 题目一：给定一个有序数组 arr，代表数周上从左到右的 n 个点。给定一个正数 L，代表一根长度为 L 的绳子，计算绳子最多能覆盖其中几个点。
 * */
int dot_cover(const int *arr, int size, int L) {
    int res = INT_MIN;
    for (int i = 0; i < size; i++) {
        /// 将绳子的起点放在 arr[i] 上。
        /// 另一种解法是，使用二分搜索在 arr 中找到 <= end 的最右点，此时该子步骤的复杂度为 O(log n)
        int end = arr[i] + L;
        int cur = 1;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] <= end) {
                cur++;
            }
        }
        res = max(res, cur);
    }
    return res;
}

/**
 * 复杂度为 O(n)。因为 i 和 j 只从左走到右一遍。
 * */
int dot_cover2(const int *arr, int size, int L) {
    int res = INT_MIN, cur;
    int i = 0, j = 0;
    while (i < size) {
        /// arr[i] 是一个被压住的点
        cur = 1;
        while (j + 1 < size && arr[j + 1] - arr[i] <= L) {
            /// 移动窗口右指针
            j++;
            cur++;
        }
        res = max(res, cur);
        /// 移动窗口左指针，重置右指针
        i++;
        j = i;
    }
    return res;
}

/**
 * 技巧二：打表找规律
 * */

/**
 * 题目二：商店里的苹果只提供 6 个每袋和 8 个每袋的包装，你想买恰好 n 个苹果，并且需要袋子越小越好。
 * 返回两种袋子的数量。如果不能购买恰好 n 个苹果，你将不会购买。
 * */
int buy_apple(int n) {
    if (n % 2 == 1) {
        return -1;
    }
    int res = INT_MAX;
    int x_max = n / 6;
    for (int x = 0; x <= x_max; x++) {  /// 小袋子越少，整体上，总袋子数就越少
        /// 实际上，剩余的苹果树一旦大于 24，就无需再继续尝试了
        int y = (n - 6 * x) / 8;
        if (6 * x + 9 * y == n) {
            res = min(res, x + y);
        }
    }
    if (res == INT_MAX) {
        return -1;
    }
    return res;
}

/**
 * 方法二：输入整数返回整数的题，可能存在潜在规律，直接打表分析规律，然后写出如下代码。
 * */
int buy_apple_print_table(int n) {
    if ((n & 1) != 0) {
        return -1;
    }
    if (n < 18) {
        return n == 0 ? 0 : (n == 6 || n == 8) ? 1 : (n == 12 || n == 14 || n == 16) ? 2 : -1;
    }
    return (n - 18) / 8 + 3;
}

/**
 * 题目三：A 和 B 两只牛轮流吃草，每次只能吃 4^p 份草（p=0,1,2...）。给定 n 份草，谁把所有草吃完谁赢，返回赢的那头牛。
 * */
string eat_grass(int n) {
    if (n < 5) {
        /// n == 0 时，认为 B 赢
        return (n == 0 || n == 2) ? "B" : "A";
    }
    int base = 1;
    while (base <= n) {
        /// 在先手的子过程中，先手将成为后手，因此这里判断返回值是否为 B，而非 A
        if (eat_grass(n - base) == "B") {
            return "A";
        }
        /// 下面这个判断是为了防止 4*base 溢出（如果给的 n 是一个非常巨大的数的话）
        if (base > n / 4) {
            break;
        }
        base *= 4;
    }
    /// 之前的每次尝试都宣告失败 —— 都没有返回 A，则是 B 赢
    return "B";
}

/**
 * 方法二：打表找规律：
 *
 * for (int n = 0; n <= 30; n++) {
 *     cout << n << ": " << eat_grass(n) << endl;
 * }
 * cout << endl;
 *
 * 发现呈现出 B A B A A 循环的特征。
 * */
string eat_grass2(int n) {
    if (n % 5 == 0 || n % 5 == 2) {
        return "B";
    }
    return "A";
}

/**
 * 技巧三：预处理
 * */

/**
 * 题目四：给定一个数组，每个元素存放的值要么为 0、要么为 1。我们需要通过翻转一些元素的值使得 0 全部在数组左侧、1 全部在数组右侧。
 * 计算最少需要翻转几个元素。
 *
 * O(n^2)
 * */
int flip(const int *arr, int size) {
    int res = INT_MAX;
    for (int i = 0; i < size; i++) {
        /// 左侧 0 的个数为 i，右侧 1 的个数为 size - i
        int flip_num = 0;
        for (int j = 0; j <= i; j++) {
            if (arr[j] == 1) {
                flip_num++;
            }
        }
        for (int j = i + 1; j < size; j++) {
            if (arr[j] == 0) {
                flip_num++;
            }
        }
        res = min(res, flip_num);
    }
    return res;
}

/**
 * 方法二：使用辅助数组，提前讲一些信息记录下来
 *
 * O(n)
 * */
int flip2(const int *arr, int size) {
    int *left_one = new int[size], *right_zero = new int[size];
    left_one[0] = arr[0] == 0 ? 0 : 1;
    right_zero[size - 1] = arr[size - 1] == 1 ? 0 : 1;

    /// arr[0...i] 中 1 的个数存放在 left_one[i] 中
    for (int i = 1; i < size; i++) {
        left_one[i] = arr[i] == 1 ? left_one[i - 1] + 1 : left_one[i - 1] + 0;
    }
    /// arr[i...size-1] 中 0 的个数存放在 right_zero[i] 中
    for (int i = size - 1; i >= 1; i--) {
        right_zero[i - 1] = arr[i - 1] == 0 ? right_zero[i] + 1 : right_zero[i] + 0;
    }
//    for (int i = 0; i < size; i++) {
//        cout << left_one[i] << " ";
//    }
//    cout << endl;
//    for (int i = 0; i < size; i++) {
//        cout << right_zero[i] << " ";
//    }
//    cout << endl;

    int res = min(right_zero[0], left_one[size - 1]);
    for (int i = 0; i <= size - 2; i++) {
        res = left_one[i] + right_zero[i + 1] < res ? left_one[i] + right_zero[i + 1] : res;
    }
    return res;
}

/**
 * 题目五：给定一个 N*N 的 01 矩阵，返回边框全是 1 的最大正方形的边长。
 * 例如，
 * 01111
 * 01001
 * 01001
 * 01111
 * 01011
 * 返回4。
 *
 * 方法一：暴力枚举
 * */
int max_square_len(int **mat, int size) {
    int res = INT_MIN;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (mat[i][j] == 1) {
                /// 判断以 mat[i][j] 为左上方顶点、边长为 k 的正方形是否存在
                for (int k = 1; k < size + 1 - i && k < size + 1 - j; k++) {
                    /// TODO: 这个 for 循环内部的过程可以使用预处理数组来简化
                    int exist = true;
                    for (int p = 0; p < k; p++) {
                        if (mat[i + p][j] != 1 || mat[i + p][j + k - 1] != 1) {
                            exist = false;
                            break;
                        }
                    }
                    for (int p = 1; p <= k - 2; p++) {
                        if (mat[i][j + p] != 1 || mat[i + k - 1][j + p] != 1) {
                            exist = false;
                            break;
                        }
                    }
                    if (exist) {
                        res = res < k ? k : res;
                    }
                }
            }
        }
    }
    return res;
}

/**
 * 方法二：使用预处理数组（矩阵）
 * */
int max_square_len2(int **mat, int size) {
    /// right 记录从每个位置开始，到最右侧有多少个连续的 1
    int **right = new int *[size];
    for (int i = 0; i < size; i++) {
        right[i] = new int[size];
        right[i][size - 1] = mat[i][size - 1] == 1 ? 1 : 0;
    }
    for (int i = 0; i < size; i++) {
        for (int j = size - 2; j >= 0; j--) {
            right[i][j] = mat[i][j] == 1 ? right[i][j + 1] + 1 : 0;
        }
    }
//    for (int i = 0; i < size; i++) {
//        for (int j = 0; j < size; j++) {
//            cout << right[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;

    /// down 记录从每个位置开始，到最下侧有多少个连续的 1
    int **down = new int *[size];
    for (int i = 0; i < size; i++) {
        down[i] = new int[size];
    }
    for (int j = 0; j < size; j++) {
        down[size - 1][j] = mat[size - 1][j] == 1 ? 1 : 0;
    }
    for (int j = 0; j < size; j++) {
        for (int i = size - 2; i >= 0; i--) {
            down[i][j] = mat[i][j] == 1 ? down[i + 1][j] + 1 : 0;
        }
    }
//    for (int i = 0; i < size; i++) {
//        for (int j = 0; j < size; j++) {
//            cout << down[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;

    int res = INT_MIN;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (mat[i][j] == 1) {
                /// 判断以 mat[i][j] 为左上方顶点、边长为 k 的正方形是否存在
                for (int k = 1; k < size + 1 - i && k < size + 1 - j; k++) {
                    if (right[i][j] >= k && down[i][j] >= k && right[i + k - 1][j] >= k && down[i][j + k - 1] >= k) {
                        res = res < k ? k : res;
                    }
                }
            }
        }
    }
    return res;
}

/**
 * 题目六：给定一个等概率返回 1～5 的随机函数，生曾一个等概率返回 1～7 的随机函数。
 * */
int rand5() {
    ::random_device rd;
    ::mt19937 mt(rd());
    uniform_int_distribution<int> rand_distribution(1, 5);
    return rand_distribution(mt);
}

/**
 * 等概率返回 0、1 的随机函数
 * */
int rand01() {
    int res;
    do {
        res = rand5();
    } while (res == 3);
    return res < 3 ? 0 : 1;
}

/**
 * 假设给定的随机函数生成范围为 a~b，目标随机函数生成范围为 c~d。
 * 先加工出 01 随机函数，然后确定 d-c 所需的二进制位个数，加工出生成范围为 0~d-c 的随机函数。
 * 最后，将结果 +c 即可。
 * */
int rand17() {
    int res;
    do {
        /// 用三个二进制位来实现 000 - 111
        /// 将 111 抛弃，然后让 000 - 110 的结果 +1，即可得到 1～7
        res = (rand01() << 2) + (rand01() << 1) + rand01();
    } while (res == 7);
    return res + 1;
}

/**
 * 题目七：给定一个以概率 p 返回 0、概率 1-p 返回 1 的函数，加工出等概率返回 0 和 1 的函数。
 *
 * e.g., p = 0.3
 * */
int rand01_p() {
    ::random_device rd;
    ::mt19937 mt(rd());
    uniform_int_distribution<int> rand_distribution(1, 10);
    return rand_distribution(mt) > 3? 1: 0;
}

int rand_equal_prob_01() {
    int res;
    do {
        /// 01: 0, 10: 1
        /// 00 和 11：重做
        res = (rand01_p() << 1) + rand01_p();
    } while (res == 0 || res == 3);
    return res == 1 ? 0: 1;
}


int main() {
//    int arr[] = {3, 6, 7, 9, 11}, size = 5;
//    int L = 4;
//    cout << dot_cover(arr, size, L) << endl;
//    cout << dot_cover2(arr, size, L) << endl;
//
//    cout << buy_apple(48) << endl;
//    cout << buy_apple_print_table(48) << endl;
//
//    for (int n = 0; n <= 30; n++) {
//        cout << eat_grass(n) << " " << eat_grass2(n) << endl;
//    }
//
//    int arr2[] = {0, 1, 1, 0, 1, 0};
//    cout << flip(arr2, 6) << endl;
//    cout << flip2(arr2, 6) << endl;
//
//    int size = 5;
//    int **mat = new int *[size];
//    for (int i = 0; i < size; i++) {
//        mat[i] = new int[size];
//    }
//    for (int i = 0; i < size; i++) {
//        mat[i][0] = 0;
//        mat[i][1] = mat[i][size - 1] = 1;
//    }
//    mat[0][2] = mat[0][3] = 1;
//    mat[1][2] = mat[1][3] = 0;
//    mat[2][2] = mat[2][3] = 0;
//    mat[3][2] = mat[3][3] = 1;
//    mat[4][2] = 0;
//    mat[4][3] = 1;
//    cout << max_square_len(mat, size) << endl;
//    cout << max_square_len2(mat, size) << endl;
//
//    for (int i = 0; i < 10; i++) {
//        cout << rand17() << endl;
//    }
    for (int i = 0; i < 10; i++) {
        cout << rand_equal_prob_01() << endl;
    }

    return 0;
}
