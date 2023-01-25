//
// Created by hliangzhao on 21/1/23.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

/**
 * 贪心策略：和暴力尝试对比，通过举反例去除一些必然不好的贪心策略，从而找到一种合适的贪心策略。
 * */

/**
 * 问题一：一些项目要占用一个会议室进行宣讲，会议室不能同时容纳两个项目的宣讲。
 * 给你每个项目开始和结束的时间，要求安排宣讲日程，使得会议室进行的宣讲的场次最多。
 * 返回这个最多的场次个数。
 *
 * 贪心策略：JCT earliest first
 * */
struct Program {
    int start, end;
};

Program *new_prog(int start, int end) {
    auto *p = new Program;
    p->start = start;
    p->end = end;
    return p;
}

/**
 * 如果 p1->end < p2->end，则认为 p1 小于 p2
 * */
bool cmp_for_prog(Program *p1, Program *p2) {
    return p1->end < p2->end;
}

int arrange_program(vector<Program *> &programs, int time_point) {
    ::sort(programs.begin(), programs.end(), cmp_for_prog);
    int res = 0;
    for (auto &p: programs) {
        if (time_point <= p->start) {
            res++;
            time_point = p->end;
        }
    }
    return res;
}

/**
 * 问题二：给定一个字符串数组，将这些字符串拼接成一个大的字符串，要求该字符串字典序最小。
 *
 * 贪心策略：将这些字符串按照 order 函数所示规则排序，然后依次拼接即可。
 *
 * 记 "a 拼接 b" 为 a \cat b，将字符串映射到一个 k 进制数，则 a \cat b = a * k^{len(b)} + b，进一步记为 a * lm(b) + b。
 * lm(b) 表示将 1 左移 len(b) 位。现在，我们试图证明：
 * 给定 a \cat b \leq b \cat c, b \cat c \leq c \cat b，证明 a \cat c \leq c \cat a。
 *
 * 只有保证比较的传递性，才能保证不同的初始字符串顺序，最终都可以保证取得相同的排序结果。
 *
 * Proof：
 * 由 a \cat b \leq b \cat c 可得 a * lm(b) + b \leq b * lm(a) + a，进一步有
 * a * lm(b) * c \leq b * lm(a) * c + a * c - b * c。
 * 由 b \cat c \leq c \cat b 可得 b * lm (c) * a + c * a - b * a \leq c * lm(b) * a。对比二者，可得
 * b * lm(c) * a - b * a \leq b * lm(a) * c - b * c，进一步有 a * lm(c) + c \leq c * lm(a) + a，
 * 即 a \cat c \leq c \cat a。
 *
 * 上面的分析表明我们的比较策略是正确的，对于任意一组字符串，不论它们原本以何种顺序排列，排序之后，顺序都相同。
 * 接下来，我们要证明：将排好序的字符串从前到后依次拼接所得的解就是最优解。
 *
 * 场景一：设 [... a b ...] 是排好序后的状态，证明 [... a b ...] 一定比 [... b a ...] 更好。
 * 因为 a 排在 b 前面，故必然有 a \cat b \leq b \cat a，结论显然成立。
 *
 * 场景二：设 [... a m1 m2 ... mk b ...] 是排好序后的状态，证明 [... a m1 m2 ... mk b ...] 一定比 [... b m1 m2 ... mk a  ...] 更好。
 * a \cat m1 \leq m1 \cat a，故 [... a m1 m2 ... mk b ...] 好于 [... m1 a m2 ... mk b ...]；
 * 再依次将 a 和 m2、m3、...、mk 交换，可以发现 [... a m1 m2 ... mk b ...] 好于 [... m1 m2 ... mk b a ...]。
 * 然后将 b 依次和 mk、m_{k-1}、...m1 交换，可以发现 [... m1 m2 ... mk b a ...] 好于 [... b m1 m2 ... mk a ...]。证毕。
 *
 * 以上说明将排好序的字符串依次拼接得到的就是最优解。
 * */

/**
 * 基于字典序比较 s1 和 s2
 * 若 s1 的字典序 <= s2，则返回 true
 * */
bool cmp_for_str(const string &s1, const string &s2) {
    int pos;
    for (pos = 0; pos < s1.size() && pos < s2.size(); pos++) {
        if (s1[pos] < s2[pos]) {
            return true;
        } else if (s1[pos] > s2[pos]) {
            return false;
        }
    }
    if (pos == s1.size()) {
        return true;
    }
    return false;
}

bool order(const string &s1, const string &s2) {
    string res1 = s1 + s2, res2 = s2 + s1;
    return cmp_for_str(res1, res2);
}

string composite(vector<string> &vec) {
    /// 字符串的 sort 默认按字典序排序，我们需要它按 order 指定的方式排序
    sort(vec.begin(), vec.end(), order);
    string res;
    /// 然后只需要依次拼接起来即可
    for (auto &str: vec) {
        res += str;
    }
    return res;
}

/**
 * 问题三：给定数组，返回最小分割代价。
 * {10, 20, 30}：
 * 方案一：60 --> 10 + 50, 50 --> 20 + 30，代价 60 + 50 = 110；
 * 方案二：60 --> 30 + 30, 30 --> 10 + 20, 代价 60 + 30 = 90。
 *
 * 基于哈夫曼编码解决。
 * */
int min_cutting_cost(const int *arr, int size) {
    priority_queue<int, vector<int>, greater<int>> pq(arr, arr + size);
    int res = 0;
    while (pq.size() > 1) {
        int v1 = pq.top();
        pq.pop();
        int v2 = pq.top();
        pq.pop();
        res += v1 + v2;
        pq.push(v1 + v2);
    }
    return res;
}

/**
 * 问题四：正数数组 costs 和 profits，k 表示最多可同时做的项目个数，m 表示初始资金。
 * 每一个做完的项目可以立即获得收益，可以支持去做下一个项目。求最后获得的最大钱数。
 *
 * 让项目按照成本升序排列（小根堆），然后按照利润降序排列，依次选择项目进行。
 * */

struct Project {
    int cost;
    int profit;
};

Project *new_project(int c, int p) {
    auto *proj = new Project;
    proj->cost = c;
    proj->profit = p;
    return proj;
}

struct MinCost {
    bool operator()(const Project *p1, const Project *p2) {
        /// 默认是大根堆，想要改成小根堆，less 函数应按照 cost 降序来写
        /// 从而使得 cost 小的，在堆顶
        return p1->cost > p2->cost;
    }
};

struct MaxProfit {
    bool operator()(const Project *p1, const Project *p2) {
        /// profit 大的，在堆顶
        return p1->profit < p2->profit;
    }
};

int max_profits(int *costs, int *profits, int size, int k, int m) {
    priority_queue<Project *, vector<Project *>, MinCost> locked_p;
    priority_queue<Project *, vector<Project *>, MaxProfit> unlocked_p;
    for (int i = 0; i < size; i++) {
        locked_p.push(new_project(costs[i], profits[i]));
    }
    for (int i = 0; i < k; i++) {
        while (!locked_p.empty() && locked_p.top()->cost <= m) {
            unlocked_p.push(locked_p.top());
            locked_p.pop();
        }
        if (unlocked_p.empty()) {
            return m;
        }
        m += unlocked_p.top()->profit;
        unlocked_p.pop();
    }
    return m;
}

/**
 * 问题五：源源不断给数，要求在任意时刻，都能返回目前已给数的中位数。
 *
 * 同时维护一个大根堆和一个小根堆，通过调整保证大根堆存放的是较小的那一部分数，小根堆存放的是较大的那一部分数。
 * */
float find_mid(const vector<int> &vec, priority_queue<int> &big_end,
               priority_queue<int, vector<int>, greater<int>> &small_end) {
    int inqueued_num = (int) (big_end.size() + small_end.size());
    for (int i = 0; i < vec.size(); i++) {
        if (i >= inqueued_num) {
            int to_be_input = vec[i];

            if (big_end.empty()) {
                big_end.push(to_be_input);
            } else {
                if (to_be_input <= big_end.top()) {
                    big_end.push(to_be_input);
                } else {
                    small_end.push(to_be_input);
                }
            }

            if (big_end.size() - small_end.size() == 2) {
                small_end.push(big_end.top());
                big_end.pop();
            } else if (small_end.size() - big_end.size() == 2) {
                big_end.push(small_end.top());
                small_end.pop();
            }
        }
    }
    float res;
    if ((big_end.size() + small_end.size()) % 2 == 1) {
        if (big_end.size() > small_end.size()) {
            res = (float) big_end.top();
        } else {
            res = (float) small_end.top();
        }
    } else {
        res = (float) (big_end.top() + small_end.top()) / 2;
    }
    return res;
}

int main() {
    vector<Program *> programs;
    programs.push_back(new_prog(0, 2));
    programs.push_back(new_prog(8, 10));
    programs.push_back(new_prog(6, 8));
    programs.push_back(new_prog(5, 7));
    programs.push_back(new_prog(1, 4));
    programs.push_back(new_prog(3, 9));
    cout << arrange_program(programs, 0) << endl;

    string arr[] = {"ba", "bc", "b"};
    vector<string> vec(arr, arr + 3);
    cout << composite(vec) << endl;

    int arr2[] = {10, 20, 30};
    cout << min_cutting_cost(arr2, 3) << endl;

    int costs[] = {3, 1, 4, 9, 9};
    int profits[] = {1, 2, 3, 7, 4};
    cout << max_profits(costs, profits, 5, 4, 1) << endl;

    int arr3[] = {3, 1, 4, 9, 9};
    vector<int> vec2(arr3, arr3 + 5);
    priority_queue<int> big_end;
    priority_queue<int, vector<int>, greater<int>> small_end;
    cout << find_mid(vec2, big_end, small_end) << endl;
    vec2.push_back(5);
    cout << find_mid(vec2, big_end, small_end) << endl;
    vec2.push_back(6);
    cout << find_mid(vec2, big_end, small_end) << endl;

    return 0;
}
