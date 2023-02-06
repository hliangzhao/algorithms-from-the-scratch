//
// Created by hliangzhao on 6/2/23.
//
#include <iostream>
#include <stack>
#include <queue>

using namespace std;

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目一：实现一个特殊的栈，在实现栈的基本功能的基础上，再实现返回栈中最小元素的操作。
 * 要求：pop、push、get_min 的时间复杂度都是 O(1)。
 * 可以使用现成的栈结构。
 * */

struct MyStack {
    stack<int> data_s;
    stack<int> min_s;
};

void push(MyStack &ms, int val) {
    ms.data_s.push(val);
    if (ms.min_s.empty()) {
        ms.min_s.push(val);
    } else {
        if (ms.min_s.top() < val) {
            ms.min_s.push(ms.min_s.top());
        } else {
            ms.min_s.push(val);
        }
    }
}

void pop(MyStack &ms) {
    if (ms.data_s.empty()) {
        return;
    }
    ms.data_s.pop();
    ms.min_s.pop();
}

int get_min(MyStack &ms) {
    return ms.min_s.top();
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目二：使用栈实现队列。
 * */

struct MyQueue {
    stack<int> push_s;
    stack<int> pop_s;
};

/**
 * 将 push_s 中的元素全部转移到 pop_s 中。
 * */
void transfer(MyQueue &mq) {
    if (mq.pop_s.empty()) {
        while (!mq.push_s.empty()) {
            mq.pop_s.push(mq.push_s.top());
            mq.push_s.pop();
        }
    }
}

void push(MyQueue &mq, int val) {
    mq.push_s.push(val);
    transfer(mq);
}

void pop(MyQueue &mq) {
    if (mq.pop_s.empty() && mq.push_s.empty()) {
        throw runtime_error("Queue is empty!");
    }
    transfer(mq);
    mq.pop_s.pop();
}

int front(MyQueue &mq) {
    if (mq.pop_s.empty() && mq.push_s.empty()) {
        throw runtime_error("Queue is empty!");
    }
    transfer(mq);
    return mq.pop_s.top();
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目三：使用队列实现栈。
 * */

struct MyStack2 {
    queue<int> q1;
    queue<int> q2;
    int cur_q = 0;
};

void push(MyStack2 &ms, int val) {
    if (ms.cur_q == 0) {
        ms.q1.push(val);
    } else {
        ms.q2.push(val);
    }
}

int pop(MyStack2 &ms) {
    if (ms.cur_q == 0) {
        /// 将 q1 除了末尾元素，其余全部转移到 q2 中
        while (ms.q1.size() > 1) {
            ms.q2.push(ms.q1.front());
            ms.q1.pop();
        }
        int ret = ms.q1.front();
        ms.q1.pop();
        ms.cur_q = 1;
        return ret;
    } else {
        /// 将 q2 除了末尾元素，其余全部转移到 q1 中
        while (ms.q2.size() > 1) {
            ms.q1.push(ms.q2.front());
            ms.q2.pop();
        }
        int ret = ms.q2.front();
        ms.q2.pop();
        ms.cur_q = 0;
        return ret;
    }
}

/**
 * 上述三个题目的应用案例：请你用栈实现树的宽度优先遍历。
 * 本质上，是需要用栈来实现一个队列，然后用该队列达成目的。
 * */

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目四：动态规划的空间压缩技巧。
 * 给定一个二维数组 mat，其中每个数都是正数，要求从左上角走到右下角，每一步只能向右或者向下，沿途经过的数组要累加起来。返回最小路径和。
 *
 * 方法一：递归
 * */
int f(int **mat, int s1, int s2, int i, int j) {
    if (i == s1 - 1 && j == s2 - 1) {
        return mat[i][j];
    }
    if (i == s1 - 1 && j != s2 - 1) {
        return mat[i][j] + f(mat, s1, s2, i, j + 1);
    }
    if (i != s1 - 1 && j == s2 - 1) {
        return mat[i][j] + f(mat, s1, s2, i + 1, j);
    }
    return mat[i][j] + min(f(mat, s1, s2, i, j + 1), f(mat, s1, s2, i + 1, j));
}

/**
 * 方法二：DP
 * */
int dp(int **mat, int s1, int s2) {
    int **mem = new int *[s1];
    for (int i = 0; i < 4; i++) {
        mem[i] = new int[s2];
    }

    for (int i = s1 - 1; i >= 0; i--) {
        for (int j = s2 - 1; j >= 0; j--) {
            if (i == s1 - 1 && j == s2 - 1) {
                mem[i][j] = mat[i][j];
            } else if (i == s1 - 1 && j != s2 - 1) {
                mem[i][j] = mat[i][j] + mem[i][j + 1];
            } else if (i != s1 - 1 && j == s2 - 1) {
                mem[i][j] = mat[i][j] + mem[i + 1][j];
            } else {
                mem[i][j] = mat[i][j] + min(mem[i][j + 1], mem[i + 1][j]);
            }
        }
    }

    return mem[0][0];
}

/**
 * 方法三：结合空间压缩技术的 DP
 *
 * 只需要申请一个数组（而非矩阵），然后交替覆盖更新数组元素即可。
 * 当矩阵或者张量某个维度非常大时，很有必要使用空间压缩。
 * */
int dp2(int **mat, int s1, int s2) {
    int *mem = new int[s1];

    /// 先填充最后一行（j 的值只和 j+1 有关）
    for (int j = s2 - 1; j >= 0; j--) {
        if (j == s2 - 1) {
            mem[j] = mat[s1 - 1][j];
        } else {
            mem[j] = mat[s1 - 1][j] + mem[j + 1];
        }
    }

    /// 从倒数第二行开始，从最后一列往前更新
    for (int i = s1 - 2; i >= 0; i--) {
        for (int j = s2 - 1; j >= 0; j--) {
            if (j == s2 - 1) {
                mem[j] += mat[i][j];
            } else {
                mem[j] = mat[i][j] + min(mem[j], mem[j + 1]);
            }
        }
    }

    return mem[0];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目五：给定一个数组，已知其中所有值都是非负的，将其看作一个容器，求能装多少水。
 * 例如，[3, 1, 2, 5, 2, 4]: 返回 5。
 * */

/*
 *  0 2 1 0 2 0
 *        _
 *       | |  _
 *  _    | | | |
 * | |  _| |_| |
 * | |_| | | | |
 * |_|_|_|_|_|_|
 *
 * */

/**
 * 问题分析：i 位置能存放的水量 = max{min{left_max_height, right_max_height} - arr[i], 0}
 * */
int water(const int *arr, int size) {
    int res[size];
    for (int i = 1; i < size - 1; i++) {
        int left_max = INT_MIN, right_max = INT_MIN;
        for (int k1 = 0; k1 < i; k1++) {
            left_max = left_max < arr[k1] ? arr[k1] : left_max;
        }
        for (int k2 = i + 1; k2 < size; k2++) {
            right_max = right_max < arr[k2] ? arr[k2] : right_max;
        }
        res[i] = max(0, min(left_max, right_max) - arr[i]);
    }
    int ans = 0;
    for (int i = 1; i < size - 1; i++) {
        ans += res[i];
    }
    return ans;
}

/**
 * 解法二：使用双指针对时间复杂度和空间复杂度全面优化
 * */
int water2(const int *arr, int size) {
    int all = 0;
    int left_max = arr[0], right_max = arr[size - 1];
    int l = 1, r = size - 1;

    /// 如果左边是瓶颈，则结算 r，然后 r--；反之则处理 l
    while (l <= r) {
        if (left_max >= right_max) {
            /// 瓶颈必然在右侧
            all += arr[r] < right_max ? right_max - arr[r] : 0;
            right_max = arr[r] > right_max ? arr[r] : right_max;
            r--;
        } else if (left_max < right_max) {
            /// 瓶颈必然在左侧
            all += arr[l] < left_max ? left_max - arr[l] : 0;
            left_max = arr[l] > left_max ? arr[l] : left_max;
            l++;
        }
    }
    return all;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目六：给定一个长度为 n 的数组 arr，将其分割为左右两个部分，返回最大的、左边最大值减去右边最大值的绝对值。
 *
 * 根据全局 max 在左还是在右进行分析。
 * */
int dividing(const int *arr, int size) {
    int g_max = arr[0];
    for (int i = 1; i < size; i++) {
        g_max = arr[i] > g_max ? arr[i] : g_max;
    }
    return max(g_max - arr[0], g_max - arr[size - 1]);
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目七：给定两个字符串，判断二者是否互为旋转词。
 * 把一个字符串任意前缀挪到尾部组成的新字符串是该字符串的一个旋转词。
 * */
bool rotation(const string &s1, const string &s2) {
    if (s1.size() != s2.size()) {
        return false;
    }
    for (int i = 0; i < s1.size(); i++) {
        /// substr 的第二个参数表示子串的长度，如果到末尾了但是不够这么长，则到末尾就返回
        string tmp = s1.substr(i + 1, s1.size()) + s1.substr(0, i + 1);
        if (tmp == s2) {
            return true;
        }
    }
    return false;
}

/**
 * 解法二：判断 s2 是否为 s1+s1 的子串
 * */
bool rotation2(const string &s1, const string &s2) {
    if (s1.size() != s2.size()) {
        return false;
    }
    string tmp = s1 + s1;
    for (int i = 0; i < tmp.size(); i++) {
//        cout << tmp.substr(i, s2.size()) << endl;
        if (tmp.substr(i, s2.size()) == s2) {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目八：
 * 给定一个数组，每个元素代表对应的咖啡机泡一杯咖啡的时间开销。
 * 给定 n 代表人的个数，每人需要喝一杯咖啡。每个人拿着自己的杯子到某个咖啡机前等待咖啡的制作，然后瞬间喝完。
 * 给定 a，代表唯一的一台清洗咖啡杯的机器（它只能串行洗咖啡杯）清洗一个杯子的时间开销，
 * 给定 b，这是手洗咖啡杯的时间开销。
 *
 * 计算从第一个人开始泡咖啡到所有人都完咖啡并且杯子也都洗干净为止的 makespan 的最小值。
 * 决策变量是，每个人选择哪台咖啡机进行咖啡的制作以及每个人的杯子将选择何种方法清洗。
 * */

/**
 * 解法：选咖啡机的子问题，是一个并行的问题，使用小根堆来处理；
 * 洗杯子的子问题，使用洗咖啡机是串行的，而且面临清洗方式的选择，可以用递归（DP）。
 * */

struct Machine {
    int idle_time;
    int cost;
};

Machine *new_machine(int idle, int cost) {
    auto *machine = new Machine;
    machine->idle_time = idle;
    machine->cost = cost;
    return machine;
}

struct Cmp {
    bool operator()(const Machine *m1, const Machine *m2) {
        return m2->cost + m2->idle_time < m1->cost + m1->idle_time;
    }
};

int wash_finish_time(const int *wash_begin_time, int n, int a, int b, int idx, int idle_time);

int wash_finish_time_dp(const int *wash_begin_time, int n, int a, int b);

int min_makespan(const int *arr, int size, int n, int a, int b);

int min_makespan(const int *arr, int size, int n, int a, int b) {
    priority_queue<Machine *, vector<Machine *>, Cmp> pq;
    for (int i = 0; i < size; i++) {
        pq.push(new_machine(0, arr[i]));
    }
    int *wash_begin_time = new int[n];
    for (int i = 0; i < n; i++) {
        /// 每个人用堆顶的咖啡机（这是最早空闲的机器）
        Machine *m = pq.top();
        pq.pop();
        m->idle_time += m->cost;
        wash_begin_time[i] = m->idle_time;
        pq.push(m);
    }
    /// 如果手洗的速度更快，则每个杯子都手洗即可，这样结果直接就是最后一个杯子的手洗开始时间 + b
    if (a >= b) {
        return wash_begin_time[n - 1] + b;
    }
//    return wash_finish_time(wash_begin_time, n, a, b, 0, 0);
    return wash_finish_time_dp(wash_begin_time, n, a, b);
}

/**
 * 还剩 n 个人的时候，把 n 的人的杯子都洗完的最早时刻
 * */
int wash_finish_time(const int *wash_begin_time, int n, int a, int b, int idx, int idle_time) {
    if (idx == n - 1) {
        /// 最后一个，选择洗咖啡机或者手洗中最早完成的那个
        return min(max(idle_time, wash_begin_time[idx]) + a, wash_begin_time[idx] + b);
    }

    /// 方式一，使用洗咖啡机清洗杯子
    int new_idle_time = max(idle_time, wash_begin_time[n]) + a;
    /// 使用方式一所得到的最早完成时间
    int p1 = max(new_idle_time, wash_finish_time(wash_begin_time, n, a, b, idx + 1, new_idle_time));

    /// 方式二，手洗
    int p2 = max(wash_begin_time[idx] + b, wash_finish_time(wash_begin_time, n, a, b, idx + 1, idle_time));

    return min(p1, p2);
}

/**
 * 将上述递归改写成 DP
 * */
int wash_finish_time_dp(const int *wash_begin_time, int n, int a, int b) {
    /// 想要使用二维数组，需要确定 idle_time 的最大值
    int max_idle_time = wash_begin_time[0] + a;
    for (int i = 1; i < n; i++) {
        max_idle_time = max(wash_begin_time[i], max_idle_time) + a;
    }
    int **mem = new int *[n];
    int col_num = wash_begin_time[n - 1] + n * a;
    for (int i = 0; i < n; i++) {
        /// 或者简单起见，直接将 wash_begin_time[n-1] + n*a 作为最大值，不会超过这个值
        mem[i] = new int[col_num];
    }
    for (int j = 0; j < col_num; j++) {
        mem[n - 1][j] = min(
                max(j, wash_begin_time[n - 1]) + a,
                wash_begin_time[n - 1] + b
        );
    }
    for (int i = n - 2; i >= 0; i--) {
        int max_idle_time_for_i = wash_begin_time[i] + (i + 1) * a;
        for (int j = 0; j < max_idle_time_for_i; j++) {
            int idle_time_after_i = max(j, wash_begin_time[i]) + a;
            /// min(p1, p2)，这里 j 实际上是 idle_time_before_i
            mem[i][j] = min(
                    max(idle_time_after_i, mem[i + 1][idle_time_after_i]),
                    max(wash_begin_time[i] + b, mem[i + 1][j])
            );
        }
    }
    return mem[0][0];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目九：给定一个数组 arr，如果通过调整可以使得 arr 中任意两个相邻的数字相乘是 4 的倍数，则返回 true，否则 false。
 *
 * 本题的难点仍然是分析业务。
 * 不妨设奇数有 a 个，没有 4 因子的偶数有 b 个，有 4 因子的偶数有 c 个。
 * 则：
 * 当 b == 0 时，按照 "奇数  4 因子数  奇数  4 因子数 ..." 的方式排列是最节省 4 因子数的；
 *      此时若 a == 1，则 c >= 1；a > 1，则 c >= a - 1。
 * 当 b != 0 时，2 因子数必然放一起，后面必然按照 "4 因子数  奇数  4 因子数 ..." 的方式排列。
 *      此时 a == 0 则 c >= 0；a == 1 则 c >= 1；a > 1 则 c >= a。
 *      注意，b = 1 && a = c = 0 也是不行的。
 * */
bool arrange(const int *arr, int size) {
    int odd = 0, even2 = 0, even4 = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] % 2 != 0) {
            odd++;
        } else if (arr[i] % 4 != 0) {
            even2++;
        } else {
            even4++;
        }
    }
    if (even2 == 0) {
        return odd == 1? even4 >= 1: even4 >= odd - 1;
    } else {
        if (even2 == 1 && odd == 0 && even4 == 0) {
            return false;
        }
        return even4 >= odd;
    }
}

int main() {
    MyQueue mq;
    for (int i = 0; i < 5; i++) {
        push(mq, i);
    }
    for (int i = 0; i < 5; i++) {
        cout << front(mq) << " ";
        pop(mq);
    }

    MyStack2 ms;
    for (int i = 0; i < 5; i++) {
        push(ms, i);
    }
    for (int i = 0; i < 5; i++) {
        cout << pop(ms) << " ";
    }

    int **mat = new int *[4];
    for (int i = 0; i < 4; i++) {
        mat[i] = new int[4];
        for (int j = 0; j < 4; j++) {
            mat[i][j] = 4 * i + j;
        }
    }
    cout << f(mat, 4, 4, 0, 0) << endl;
    cout << dp(mat, 4, 4) << endl;
    cout << dp2(mat, 4, 4) << endl;

    int arr[] = {3, 1, 2, 5, 2, 4};
    cout << water(arr, 6) << endl;
    cout << water2(arr, 6) << endl;
    cout << dividing(arr, 6) << endl;

    cout << rotation("xyz", "yzx") << endl;
    cout << rotation2("xyz", "yzx") << endl;

    return 0;
}
