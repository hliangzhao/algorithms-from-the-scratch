//
// Created by hliangzhao on 30/1/23.
//
#include <iostream>
#include <ctime>
#include <set>
#include <cmath>

using namespace std;

/**
 * 动态规划。
 *
 * 第一步，尝试各种递归方法（基本可以划分为 (1) 从左到右；(2) 范围）。
 *      单个可变参数，最好是零维（例如，一个整数）；
 *      可变参数尽可能少。
 *
 * 第二步，加缓存，实现记忆化搜索。
 *
 * 第三步，直接根据递归表达式更新表元素值，得到基于严格表结构的动态规划。
 *
 * 第四步，可能的进一步优化（例如斜率优化等），直接利用表中相关元素之间的关系更新元素值。
 * */

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题一：给定 n 个格子排成一排，有一个机器人，初始位置为 s (1<=s<=n)，目标位置为 e (1<=e<=n)。
 * 要求机器人在 k 步从 s 到 e，每一步必须走且不能越界，求出有多少种走法。
 *
 * 解法一：暴力递归。我们用 f1(s,k) 表示从当前位置 s 出发，走 k 步，抵达 e 的可能走法个数。
 *
 *                  f1(2,4)
 *                  /    \
 *             f(1,3)    f1(3,3)
 *              /       /    \
 *           f(2,2)  f(2,2) f1(4,2)
 *           ...      ...    ...
 *
 * 可以发现，f(2,2) 被计算了两次，并且之前的决定不影响 f1(2,2) 的计算结果，这就是「无后效性」。
 * 此外，重复这是不必要的，我们可以使用一个表将已经计算的结果暂存下来，从而用于直接 retrieve。这就是「记忆化搜索」。
 *
 * 分析暴力递归的时间复杂度。假设从中间出发，则往左右走会形成一棵高度为 k 的二叉树，因此最快 O(2^k)。
 * */
int moving_ways(int n, int s, int k, int e) {
//    if (k == 1) {
//        int res = 0;
//        if (s + 1 <= n && s + 1 == e) {
//            res++;
//        }
//        if (s - 1 >= 1 && s - 1 == e) {
//            res++;
//        }
//        return res;
//    }
    /// 判断 k == 0 更简洁
    if (k == 0) {
        return s == e ? 1 : 0;
    }
    if (s == 1) {
        return moving_ways(n, s + 1, k - 1, e);
    }
    if (s == n) {
        return moving_ways(n, s - 1, k - 1, e);
    }
    return moving_ways(n, s + 1, k - 1, e) + moving_ways(n, s - 1, k - 1, e);
}

/**
 * 解法一
 * */
int moving_ways_pruned(int n, int s, int k, int e) {
    if (k == 0) {
        return s == e ? 1 : 0;
    }
    /// 剪枝一部分重复解：在不浪费任何一步的前提下（不走回头路），也无法抵达 e，则说明当前探索无解，提前返回
    if (s - e > k && e - s > k) {
        return 0;
    }
    if (s == 1) {
        return moving_ways_pruned(n, s + 1, k - 1, e);
    }
    if (s == n) {
        return moving_ways_pruned(n, s - 1, k - 1, e);
    }
    return moving_ways_pruned(n, s + 1, k - 1, e) + moving_ways_pruned(n, s - 1, k - 1, e);
}

/**
 * 解法二：加缓存，实现记忆化搜索。
 *
 * 分析本方法的复杂度：O(NK)。每个格子最多只会被递归计算一次。
 * */
int proc_of_memorized(int n, int s, int k, int e, int **memory) {
    if (memory[k][s] != -1) {
        return memory[k][s];
    }
    /// 递归返回前，先讲记录更新
    if (k == 0) {
        memory[k][s] = s == e ? 1 : 0;
        return memory[k][s];
    }
    if (s == 1) {
        memory[k][s] = proc_of_memorized(n, s + 1, k - 1, e, memory);
    } else if (s == n) {
        memory[k][s] = proc_of_memorized(n, s - 1, k - 1, e, memory);
    } else {
        memory[k][s] = proc_of_memorized(n, s + 1, k - 1, e, memory) +
                       proc_of_memorized(n, s - 1, k - 1, e, memory);
    }
    return memory[k][s];
}

int moving_ways_memorized(int n, int s, int k, int e) {
    /// 上缓存（数组大小依据状态个数进行分配）
    int **memory = new int *[k + 1];
    for (int i = 0; i < k + 1; i++) {
        memory[i] = new int[n + 1];
    }
    /// -1，代表无效信息（同时充当标志位）
    for (int i = 0; i < k + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            memory[i][j] = -1;
        }
    }
    return proc_of_memorized(n, s, k, e, memory);
}

/**
 * 解法三：解法二虽然使用了记忆化搜索，但是没有处理不同结果之间的依赖。
 * 依赖关系通过观察递归表达式来找到。
 *
 * 在二维数组 memory 中，非边界点的值为左上方和右上方值的和；左边界点的值 = 右上方点的值；右边界点的值 = 左上方点的值。
 * 因此，直接将二维数组的元素值按照上述规律计算即可。
 *
 * 这就是「基于严格表结构的动态规划」。
 *
 * 综上，可以发现，所谓的「状态转移方程」，其实在解法一中就已经写出来了。但是本方法才是本问题动态规划的根本形态。
 * */
int moving_ways_dp(int n, int s, int k, int e) {
    int **memory = new int *[k + 1];
    for (int i = 0; i < k + 1; i++) {
        memory[i] = new int[n + 1];
    }
    /// 边界条件
    memory[0][e] = 1;

    for (int i = 1; i < k + 1; i++) {
        for (int j = 1; j < n + 1; j++) {
            if (j == 1) {
                /// 左边界
                memory[i][j] = memory[i - 1][2];
            } else if (j == n) {
                /// 右边界
                memory[i][j] = memory[i - 1][n - 1];
            } else {
                /// 中间点
                memory[i][j] = memory[i - 1][j - 1] + memory[i - 1][j + 1];
            }
        }
    }
    return memory[k][s];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题二：给定一个正数数组，每个元素代表一枚硬币的面值（允许含重复值）。
 * 给定一个目标数额 aim（正数），返回组成该数额所需的、最少的硬币个数。
 *
 * 解法一：proc_of_coins(arr, size, idx, rest) 表示自 idx 开始自由选择，
 * 使得选择的硬币数额之和刚好等于 rest 时，所选择的最少的硬币数。
 * */

int proc_of_coins(const int *arr, int size, int idx, int rest) {
    /// -1 是标志位，表示无效解
    /// （1）所需的硬币数额之和是负数；
    /// （2）rest > 0，但是没有硬币可以选了。
    /// 均说明本次尝试失败
    if (rest < 0 || (rest > 0 && idx == size)) {
        return -1;
    }
    /// 是一个解，此时无需再选任何硬币，因此返回 0
    if (rest == 0) {
        return 0;
    }
    /// 递归（需要剪枝 —— 将无效解抛弃）
    int res_of_not_choose_idx = proc_of_coins(arr, size, idx + 1, rest);
    int res_of_choose_idx = proc_of_coins(arr, size, idx + 1, rest - arr[idx]);
    if (res_of_not_choose_idx == -1 && res_of_choose_idx == -1) {
        return -1;
    } else {
        if (res_of_not_choose_idx == -1) {
            return res_of_choose_idx + 1;
        }
        if (res_of_choose_idx == -1) {
            return res_of_not_choose_idx;
        }
        return min(res_of_not_choose_idx, 1 + res_of_choose_idx);
    }
}

int least_coins(const int *arr, int size, int aim) {
    return proc_of_coins(arr, size, 0, aim);
}

/**
 * 解法二：结合记忆化搜索
 * */

int proc_of_coins_memorized(const int *arr, int size, int idx, int rest, int **memory) {
    if (rest < 0) {
        /// 对应的缓存不存在，永远不可能命中
        return -1;
    }

    if (memory[idx][rest] != -2) {
        /// 缓存命中
        return memory[idx][rest];
    }

    /// 分三种情况讨论
    if (rest == 0) {
        memory[idx][rest] = 0;
    } else if (idx == size) {
        memory[idx][rest] = -1;
    } else {
        int p1 = proc_of_coins(arr, size, idx + 1, rest);
        int p2 = proc_of_coins(arr, size, idx + 1, rest - arr[idx]);
        if (p1 == -1 && p2 == -1) {
            return -1;
        } else {
            if (p1 == -1) {
                memory[idx][rest] = p2 + 1;
            } else if (p2 == -1) {
                memory[idx][rest] = p1;
            } else {
                memory[idx][rest] = min(p1, 1 + p2);
            }
        }
    }

    return memory[idx][rest];
}

int least_coins_memorized(const int *arr, int size, int aim) {
    int **memory = new int *[size + 1];
    for (int i = 0; i < size + 1; i++) {
        memory[i] = new int[aim + 1];
    }
    /// -1 已经被占用，因此用 -2 表示尚未设置过
    for (int i = 0; i < size + 1; i++) {
        for (int j = 0; j < aim + 1; j++) {
            memory[i][j] = -2;
        }
    }

    return proc_of_coins_memorized(arr, size, 0, aim, memory);
}

/**
 * 解法三：记忆化表结构
 *
 * */
int least_coins_dp(const int *arr, int size, int aim) {
    int **memory = new int *[size + 1];
    for (int i = 0; i < size + 1; i++) {
        memory[i] = new int[aim + 1];
    }

    for (int i = 0; i < size + 1; i++) {
        memory[i][0] = 0;
    }
    for (int j = 1; j < aim + 1; j++) {
        memory[size][j] = -1;
    }
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 1; j < aim + 1; j++) {
            int p1 = memory[i + 1][j];
            int p2 = -1;
            if (j - arr[i] >= 0) {
                p2 = memory[i + 1][j - arr[i]];
            }
            if (p1 == -1 && p2 == -1) {
                memory[i][j] = -1;
            } else {
                if (p1 == -1) {
                    memory[i][j] = p2 + 1;
                } else if (p2 == -1) {
                    memory[i][j] = p1;
                } else {
                    memory[i][j] = min(p1, p2 + 1);
                }
            }
        }
    }
    return memory[0][aim];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题三：给定一个整型数组排成一排，玩家 A 和 B 依次拿走最左或最右的牌，二者都绝顶聪明，返回最后获胜者的分数。
 * 要求 A 先拿。（该问题第一次出现在 ch07/file-4）
 * */

/**
 * 解法一：当时的解法 —— 暴力递归
 * */

int f1(const int *arr, int l, int r);

int s1(const int *arr, int l, int r);

int winner_score1(const int *arr, int size);

/**
 * 先手函数
 * */
int f1(const int *arr, int l, int r) {
    if (l == r) {
        return arr[l];
    }
    return max(arr[l] + s1(arr, l + 1, r), arr[r] + s1(arr, l, r - 1));
}

/**
 * 后手函数
 * */
int s1(const int *arr, int l, int r) {
    if (l == r) {
        return 0;
    }
    /// 对方绝顶聪明，因此总会把差的情况留给我
    return min(f1(arr, l + 1, r), f1(arr, l, r - 1));
}

int winner_score1(const int *arr, int size) {
    /// 返回 A 和 B 中分数高的
    return max(f1(arr, 0, size - 1), s1(arr, 0, size - 1));
}

/**
 * 解法二：直接将上述过程改造成记忆化搜索。
 * 此时，二维数组在范围上枚举所有的状态，因为 l <= r 恒成立，因此 f 矩阵和 s 矩阵的左下三角是无意义的。
 *
 * 难点在于填表的顺序，具体问题具体分析。对角线上的数，几乎总是先填（对应 l== r，极端情况）。
 * */
int winner_score_dp(const int *arr, int size) {
    int **f_arr = new int *[size];
    for (int i = 0; i < size; i++) {
        f_arr[i] = new int[size];
    }
    int **s_arr = new int *[size];
    for (int i = 0; i < size; i++) {
        s_arr[i] = new int[size];
    }

    /// 设置边界
    for (int i = 0; i < size; i++) {
        f_arr[i][i] = arr[i];
        s_arr[i][i] = 0;
    }
    /// 根据递归填表
    for (int i_max = size - 2; i_max >= 0; i_max--) {
        for (int i = 0; i <= i_max; i++) {
            int j = i + size - 1 - i_max;
            f_arr[i][j] = max(arr[i] + s_arr[i + 1][j], arr[j] + s_arr[i][j - 1]);
            s_arr[i][j] = min(f_arr[i + 1][j], f_arr[i][j - 1]);
        }
    }
    return max(f_arr[0][size - 1], s_arr[0][size - 1]);
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题四：给定大小为 (9,10) 的象棋棋盘，马从 (0,0) 位置开始，问：跳 step 步，抵达 (x,y) 的跳法有多少种。
 * */

/**
 * 方法一：暴力递归
 *
 * 注意，从 (x,y) 到 (0,0) 和 从 (0,0) 到 (x,y) 是一样的。
 * */

int proc_of_horse(int x, int y, int step) {
    if (x < 0 || x > 8 || y < 0 || y > 9) {
        return 0;
    }
    if (step == 0) {
        return (x == 0 && y == 0) ? 1 : 0;
    }
    return proc_of_horse(x - 1, y + 2, step - 1) +
           proc_of_horse(x + 1, y + 2, step - 1) +
           proc_of_horse(x + 2, y - 1, step - 1) +
           proc_of_horse(x + 2, y + 1, step - 1) +
           proc_of_horse(x - 1, y - 2, step - 1) +
           proc_of_horse(x + 1, y - 2, step - 1) +
           proc_of_horse(x - 2, y - 1, step - 1) +
           proc_of_horse(x - 2, y + 1, step - 1);
}

int horse(int x, int y, int step) {
    return proc_of_horse(x, y, step);
}

/**
 * 方法二：记忆化搜索
 * */

int proc_of_horse_dp(int ***memory, int i, int j, int k) {
    if (i < 0 || i > 8 || j < 0 || j > 9) {
        return 0;
    }
    return memory[i][j][k];
}

int horse_dp(int x, int y, int step) {
    int ***memory = new int **[9];
    for (int i = 0; i < 9; i++) {
        memory[i] = new int *[10];
        for (int j = 0; j < 10; j++) {
            memory[i][j] = new int[step + 1];
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 10; j++) {
            memory[i][j][0] = 0;
        }
    }
    memory[0][0][0] = 1;

    for (int k = 1; k <= step; k++) {
        /// i 和 j 各自的顺序无所谓
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                memory[i][j][k] = proc_of_horse_dp(memory, i - 1, j + 2, k - 1) +
                                  proc_of_horse_dp(memory, i + 1, j + 2, k - 1) +
                                  proc_of_horse_dp(memory, i + 2, j - 1, k - 1) +
                                  proc_of_horse_dp(memory, i + 2, j + 1, k - 1) +
                                  proc_of_horse_dp(memory, i - 1, j - 2, k - 1) +
                                  proc_of_horse_dp(memory, i + 1, j - 2, k - 1) +
                                  proc_of_horse_dp(memory, i - 2, j - 1, k - 1) +
                                  proc_of_horse_dp(memory, i - 2, j + 1, k - 1);
            }
        }
    }

    return memory[x][y][step];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题五：给定一个大小为 (n,m) 的二维平面，Bob 所在位置为 (a,b)（可能在平面外），在任意位置 Bob 随机选择上下左右中的一个位置前进。
 * 一旦越界，当场认为 Bob 任务失败。问：走 k 步，Bob 任务成功的走法总个数。
 * */

/**
 * 解法一：暴力递归
 * */
int proc_of_mission(int n, int m, int a, int b, int k) {
    if (a < 0 || a >= n || b < 0 || b >= m) {
        return 0;
    }
    if (k == 0) {
        return 1;
    }
    return proc_of_mission(n, m, a + 1, b, k - 1) + proc_of_mission(n, m, a - 1, b, k - 1) +
           proc_of_mission(n, m, a, b + 1, k - 1) + proc_of_mission(n, m, a, b - 1, k - 1);
}

int mission(int n, int m, int a, int b, int step) {
    return proc_of_mission(n, m, a, b, step);
}

/**
 * 解法二：记忆化搜索。本题无枝可减，因此两个解法的时间开销几乎相同。
 * */
int proc_of_mission_dp(int n, int m, int a, int b, int k, int ***memory) {
    if (a < 0 || a >= n || b < 0 || b >= m) {
        return 0;
    }
    return memory[a][b][k];
}

int mission_dp(int n, int m, int a, int b, int step) {
    int ***memory = new int **[n];
    for (int i = 0; i < n; i++) {
        memory[i] = new int *[m];
        for (int j = 0; j < m; j++) {
            memory[i][j] = new int[step + 1];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            memory[i][j][0] = 1;
        }
    }
    for (int k = 1; k <= step; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                memory[i][j][k] = proc_of_mission_dp(n, m, i - 1, j, k - 1, memory) +
                                  proc_of_mission_dp(n, m, i + 1, j, k - 1, memory) +
                                  proc_of_mission_dp(n, m, i, j - 1, k - 1, memory) +
                                  proc_of_mission_dp(n, m, i, j + 1, k - 1, memory);
            }
        }
    }
    return memory[a][b][step];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题六：给定一个正数数组，每个元素代表一枚硬币的面值（不含重复值）。每个面值的硬币有任意个。
 * 给定一个目标数额 aim（正数），返回组成该数额的硬币组合方法个数。
 * */

/**
 * 解法一：暴力递归
 * */

int combine_of_coins(const int *arr, int size, int aim);

/// 可以自由使用 arr[idx...]，需要达到的累加数额是 rest，返回组合的个数
int proc_of_combine(const int *arr, int size, int idx, int rest);

int combine_of_coins(const int *arr, int size, int aim) {
    return proc_of_combine(arr, size, 0, aim);
}

int proc_of_combine(const int *arr, int size, int idx, int rest) {
    if (idx == size) {
        return rest == 0 ? 1 : 0;
    }
    int combinations = 0;
    for (int num = 0; num * arr[idx] <= rest; num++) {
        combinations += proc_of_combine(arr, size, idx + 1, rest - num * arr[idx]);
    }
    return combinations;
}

/**
 * 解法二：记忆化搜索。
 * 解法一包含枚举行为，计算缓存值的时候也需要枚举。
 *
 * 时间复杂度为 O(size * aim^2)
 * */
int combine_of_coins_dp(const int *arr, int size, int aim) {
    int **memory = new int *[size + 1];
    for (int i = 0; i < size + 1; i++) {
        memory[i] = new int[aim + 1];
    }
    memory[size][0] = 1;
    for (int j = 1; j <= aim; j++) {
        memory[size][j] = 0;
    }
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 0; j <= aim; j++) {
            int combinations = 0;
            for (int num = 0; num * arr[i] <= j; num++) {
                combinations += memory[i + 1][j - num * arr[i]];
            }
            memory[i][j] = combinations;
        }
    }
    return memory[0][aim];
}

/**
 * 解法三：斜率优化 —— 观察 memory 数组的临近元素之间的依赖关系，去除枚举行为。
 * 斜率优化时，其实不需要再去找其物理递归意义。
 *
 *           x  y
 * ... d  c  b  a
 *
 * y = x + a，无需枚举 a b c d...
 * */
int combine_of_coins_dp2(const int *arr, int size, int aim) {
    int **memory = new int *[size + 1];
    for (int i = 0; i < size + 1; i++) {
        memory[i] = new int[aim + 1];
    }
    memory[size][0] = 1;
    for (int j = 1; j <= aim; j++) {
        memory[size][j] = 0;
    }
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 0; j <= aim; j++) {
            memory[i][j] = memory[i+1][j];
            if (j - arr[i] >= 0) {
                memory[i][j] += memory[i][j - arr[i]];
            }
        }
    }
    return memory[0][aim];
}

int main() {
    ::clock_t st, ed;

    int n = 40, s = 2, e = 4, k = 30;
    st = ::clock();
    cout << moving_ways(n, s, k, e) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << moving_ways_pruned(n, s, k, e) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << moving_ways_memorized(n, s, k, e) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << moving_ways_dp(n, s, k, e) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    int arr[] = {2, 7, 3, 5, 2, 100, 7, 9, 2, 78, 257, 12, 123, 34, 23, 1};
    int size = 16, aim = 24;
    st = ::clock();
    cout << least_coins(arr, size, aim) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << least_coins_memorized(arr, size, aim) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << least_coins_dp(arr, size, aim) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    int arr2[] = {1, 2, 100, 4};
    cout << winner_score1(arr2, 4) << endl;
    cout << winner_score_dp(arr2, 4) << endl;

    st = ::clock();
    cout << horse(1, 1, 8) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << horse_dp(1, 1, 8) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    int n2 = 20, m2 = 30, a2 = 2, b2 = 2, step2 = 12;

    st = ::clock();
    cout << mission(n2, m2, a2, b2, step2) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << mission_dp(n2, m2, a2, b2, step2) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    int arr3[] = {1, 100, 3, 5, 10, 9, 234, 90, 123};
    int size3 = 9, aim3 = 123;
    st = ::clock();
    cout << combine_of_coins(arr3, size3, aim3) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << combine_of_coins_dp(arr3, size3, aim3) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    st = ::clock();
    cout << combine_of_coins_dp2(arr3, size3, aim3) << endl;
    ed = ::clock();
    cout << "finished in " << (double) (ed - st) / CLOCKS_PER_SEC * 1e3 << "ms" << endl;

    return 0;
}
