//
// Created by hliangzhao on 7/2/23.
//
#include <iostream>
#include <vector>
#include <map>

using namespace std;

/**
 * 本次主要讲解斐波那契数列在各种问题中的应用。
 * */

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题一：严格递归问题的矩阵求法
 *
 * 严格意义上的递归式，例如斐波那契数列：f(n) = f(n-1) + f1(n-2)
 * 其复杂度必为 O(log n)。如果递归式包含选择分支，则不满足。
 *
 * 以斐波那契数列为例，其递归表达式满足：
 * [f(n), f(n-1)] = [f(n-1), f1(n-2)] * [a,b;c,d]；
 * 我们可以代入将参数矩阵中的 abcd 求出来。
 * 则对于任意的 f1(n)，我们可以通过下式将其求出：
 * [f(n), f(n-1)] = [f(2), f1(1)] * [a,b;c,d]^{n-2}。
 *
 * 此时的难点是：矩阵的幂 [a,b;c,d]^{n-2} 如何在 O(log n) 时间内计算出来。
 * 做法：将 n-2 以二进制位的模式表达，例如 75 = 1001011，初始化 res = 单位矩阵 I，每次乘上 t = [a,b;c,d]^k，
 * 其中 k 从 75 的低位到高位遍历（第 k 个二进制位必须为 1 才参与相乘）。
 * */

int fibonacci(int n);

void mat_power(int res[2][2], int base[2][2], int p);

void mat_multi(int A[2][2], int B[2][2]);

int fibonacci(int n) {
    if (n < 1) {
        return 0;
    }
    if (n == 1 || n == 2) {
        return 1;
    }
    int base[2][2] = {{1, 1},
                      {1, 0}};
    /// 初始化 res 为单位矩阵
    int res[2][2] = {{1, 0},
                     {0, 1}};
    mat_power(res, base, n - 2);
    return res[0][0] + res[1][0];
}

void mat_power(int res[2][2], int base[2][2], int p) {
    for (; p != 0; p >>= 1) {
        if ((p & 1) != 0) {
            mat_multi(res, base);
        }
        mat_multi(base, base);
    }
}

/**
 * 将 A 和 B 相乘的结果写入 A
 * */
void mat_multi(int A[2][2], int B[2][2]) {
    int tmp[2][2] = {{0, 0},
                     {0, 0}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                tmp[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            A[i][j] = tmp[i][j];
        }
    }
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题二：牛数量问题。
 * 一开始农场只有一头成熟的牛，第二年开始，该牛每年生产一头母牛。每只母牛需要三年成熟，成熟也可以生产牛。问第 n 年后农场有多少头牛。
 *
 * 本题仍然是递归。f(n) = f(n-1) + f1(n-3)。
 * 第 n 年结束时的牛数量，等于去年的牛数量加上今年生产的牛的数量。今年能生产多少牛呢？三年前有多少头牛就能生产多少牛。
 *
 * [f(n), f(n-1), f(n-2)] = [f(n-1), f(n-2), f(n-3)] * [a,b,c;d,e,f1;g,h,i]
 *
 * 参数矩阵中待求解的参数个数为 9。
 *
 * 这种问题的变种很多。例如，每头牛每年可以生两只母牛，但是每头牛的寿命是 5 年。则
 * f1(n) = f(n-1) + 2 * f(n-3) - f(n-5)。
 * */

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题三：给定一个 01 字符串，如果每个出现 0 的位置的左侧是一个 1，这样的 01 字符串被称为达标字符串。
 * 给定 01 字符串的长度 n，返回达标字符串的数量。
 *
 * 该问题的递归表达式是一个斐波那契数列。
 * 用 f1(i) 表示长度为 i 的字符串中达标的个数，
 * 当 1 <= i <= n 时，f(i) = f(i-1) + f1(i-2)。这是对第一位为 1 或 0 分类讨论的结果。
 * 如果第一位为 1，则是 f(i-1)，若第一位为 0，则第二位必为 1，因此是 f1(i-2)。
 * */

int f1(int n, int i) {
    if (i == 1) {
        return 1;
    }
    if (i == 2) {
        return 2;
    }
    return f1(n, i - 1) + f1(n, i - 2);
}

int str_num(int n) {
    return f1(n, n);
}

/**
 * 解法二：改成 DP。
 * */
int str_num_dp(int n) {
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 2;
    }
    int mem[n + 1];
    mem[1] = 1;
    mem[2] = 2;
    for (int i = 3; i <= n; i++) {
        mem[i] = mem[i - 1] + mem[i - 2];
    }
    return mem[n];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题四：有 n 根木棍，第 i 根木棍的长度为 i，计算最少去掉多少根木棍，使得剩余的木棍中，
 * 任选 3 个都不能组成三角形。
 *
 * 问题分析：最少去掉多少根等于最多保留多少根，计算不大于 n 的斐波那契数列最后一项，
 * 该项的 index 减 1 就是最多保留的木棍的个数（之所以减 1 是因为个数为 1 的木棍只有 1 个）。
 *
 * 记住，三角形要联想到斐波那契数列上，因为斐波那契数列就是三角形不存在的边界条件。
 * */
int impossible_triangle(int n) {
    if (n < 3) {
        /// 无意义
        return 0;
    }
    int idx = 2;
    int a = 1, b = 1, tmp;
    while (a <= n) {
        tmp = a;
        a = a + b;
        b = tmp;
        idx++;
    }
    return n - (idx - 2);
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题五：背包问题的变种
 *
 * 给定容量为 w 的背包，现有 n 个物品，每个物品的体积为 v[i]，在总体积不超过 w 的情况下，一共有多少种放法？
 * （总体积为 0 也是一种放法）。
 * */

/**
 * 解法一：先递归计算总体积刚好为 p 时的放法个数 f(p)，然后把 0 <= p <= w 时候的这些 f1(p) 累加起来就是答案。
 *
 * 对于「刚好和为 p」这个子问题，它和 ch11/01 中的问题二「硬币的组合」有些相似。
 * 我们定义 f(idx, rest) 表示自 idx 位置开始自由选择，使得总体积为 rest 的放法个数。
 * */
int f2(const int *v, int size, int idx, int rest) {
    /// 所需要的体积数是负数，或者虽是正数但是没有物品可以选择了，都会造成无效解
    if (rest < 0 || (rest > 0 && idx == size)) {
        return -1;
    }
    /// 是一个合法解
    if (rest == 0) {
        return 1;
    }
    /// 将第 idx 个物品放入背包 or not
    int p1 = f2(v, size, idx + 1, rest);
    int p2 = f2(v, size, idx + 1, rest - v[idx]);
    if (p1 == -1 && p2 == -1) {
        return -1;
    } else if (p2 == -1) {
        return p1;
    } else if (p1 == -1) {
        return p2;
    } else {
        return p1 + p2;
    }
}

int packing(const int *v, int size, int w) {
    int res = 0;
    for (int p = 0; p <= w; p++) {
        res += f2(v, size, 0, p);
    }
    return res;
}

/**
 * 解法二：将解法一从递归改写成 DP
 * */
int f2_dp(const int *v, int size, int rest) {
    int **memory = new int *[size + 1];
    for (int i = 0; i < size + 1; i++) {
        memory[i] = new int[rest + 1];
    }

    for (int i = 0; i < size + 1; i++) {
        memory[i][0] = 1;
    }
    for (int j = 1; j < rest + 1; j++) {
        memory[size][j] = -1;
    }
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 1; j < rest + 1; j++) {
            int p1 = memory[i + 1][j];
            int p2 = -1;
            if (j - v[i] >= 0) {
                p2 = memory[i + 1][j - v[i]];
            }
            if (p1 == -1 && p2 == -1) {
                memory[i][j] = -1;
            } else {
                if (p1 == -1) {
                    memory[i][j] = p2;
                } else if (p2 == -1) {
                    memory[i][j] = p1;
                } else {
                    memory[i][j] = p1 + p2;
                }
            }
        }
    }
    return memory[0][rest];
}

int packing_dp(const int *v, int size, int w) {
    int res = 0;
    for (int p = 0; p <= w; p++) {
        res += f2_dp(v, size, p);
    }
    return res;
}

/**
 * 解法三：将 f(idx, cur_w) 定义为：目前的累计重量为 cur_w，自 idx 位置自由选择，合法的放法个数。
 * */
int f3(const int *v, int size, int w, int idx, int cur_w) {
    if (idx == size) {
        if (cur_w > w) {
            return 0;
        } else {
            return 1;
        }
    }
    if (cur_w > w) {
        return 0;
    } else if (cur_w + v[idx] > w) {
        /// v[idx] 不能选
        return f3(v, size, w, idx + 1, cur_w);
    } else {
        /// v[idx] 选或不选均可
        return f3(v, size, w, idx + 1, cur_w) + f3(v, size, w, idx + 1, cur_w + v[idx]);
    }
}

int packing2(const int *v, int size, int w) {
    return f3(v, size, w, 0, 0);
}

/**
 * 解法四：将解法三改成 DP
 * */
int packing2_dp(const int *v, int size, int w) {
    int **mem = new int *[size + 1];
    for (int i = 0; i <= size; i++) {
        mem[i] = new int[w + 1];
    }
    for (int j = 0; j <= w; j++) {
        mem[size][j] = 1;
    }
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 0; j <= w; j++) {
            if (j + v[i] > w) {
                mem[i][j] = mem[i + 1][j];
            } else {
                mem[i][j] = mem[i + 1][j] + mem[i + 1][j + v[i]];
            }
        }
    }
    return mem[0][0];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题六：每个 job 有对应的报酬和难度值。给定一个 job 数组，记录所有工作的信息。
 * 给定一个 int 类型的数组 arr，记录所有人的能力值。我们要为每个人按照如下标准选择工作：
 * 在不超过此人能力值的情况下，选择报酬最高的。返回一个 int 数组，表示每个人按照此标准选择工作后所能获得的报酬。
 * 注意，每份工作都有无限多个岗位。
 *
 * 解：先将这些工作按照难度升序排列，相同难度的，按照报酬降序排列。然后，从中删除非「组长」工作。
 * 对于剩下的组长工作，删除不满足报酬递增的。剩下的工作就是难度递增、报酬也递增的。
 * 此时，对每个人而言，选择最大的、难度不超过其能力值的工作即可。
 * */

struct Job {
    int hard;
    int money;
};

Job *new_job(int h, int m) {
    auto *j = new Job;
    j->hard = h;
    j->money = m;
    return j;
}

/**
 * 自定义比较器
 * */
struct Cmp {
    bool operator()(const Job *j1, const Job *j2) {
        if (j2->hard != j1->hard) {
            /// 难度升序
            return j2->hard > j1->hard;
        }
        /// 报酬降序
        return j2->money < j1->money;
    }
};

/**
 * 从 vec 中删除这些不用的元素相对困难（C++不提供 TreeMap）。此处的解决办法是：将可用的元素加入 map 中。
 * */
int *working(vector<Job *> &jobs, const int *abilities, int worker_num) {
    sort(jobs.begin(), jobs.end(), Cmp());
    int *res = new int[worker_num];
    map<int, int> h2m;

    h2m.insert({jobs[0]->hard, jobs[0]->money});
    int cur_h = jobs[0]->hard;

    for (auto v: jobs) {
        if (v->hard > cur_h) {
            h2m.insert({v->hard, v->money});
            cur_h = v->hard;
        }
    }

    for (int i = 0; i < worker_num; i++) {
        int pre = -1;
        for (auto &kv: h2m) {
            if (kv.first > abilities[i]) {
                break;
            }
            pre = kv.first;
        }
        if (pre == -1) {
            res[i] = 0;
        } else {
            res[i] = h2m[pre];
        }
    }

    return res;
}

// --------------------------------------------------------------------------------------------------------------

int main() {
//    for (int i = 1; i <= 10; i++) {
//        cout << fibonacci(i) << " ";
//    }

//    cout << str_num(4) << " " << str_num_dp(4) << endl;

//    cout << impossible_triangle(13) << endl;

//    int arr[] = {1, 2, 3, 5, 7, 9};
//    for (int w = 5; w <= 17; w++) {
//        cout << packing(arr, 6, w) << " ";
//        cout << packing_dp(arr, 6, w) << " ";
//        cout << packing2(arr, 6, w) << " ";
//        cout << packing2_dp(arr, 6, w) << endl;
//    }

    vector<Job *> jobs;
    jobs.push_back(new_job(3, 5));
    jobs.push_back(new_job(2, 7));
    jobs.push_back(new_job(9, 100));
    jobs.push_back(new_job(1, 4));
    jobs.push_back(new_job(2, 6));
    jobs.push_back(new_job(3, 3));
    jobs.push_back(new_job(1, 1));
    jobs.push_back(new_job(2, 8));
    int abilities[] = {0, 1, 2, 4, 8, 10};
    int worker_num = 6;
    int *res = working(jobs, abilities, worker_num);
    for (int i = 0; i < worker_num; i++) {
        cout << res[i] << " ";
    }
    cout << endl;

    return 0;
}