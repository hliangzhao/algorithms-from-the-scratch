//
// Created by hliangzhao on 2/2/23.
//
#include <iostream>
#include <stack>
#include <map>
#include <set>

using namespace std;

/**
 * 题目一：给定非负整数 n，返回可以组成的不同的二叉树的个数。
 * n = 0 时，是一棵空树，返回 1。
 * */
int tree_num(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    int res = 0;
    for (int i = 0; i <= n - 1; i++) {
        res += tree_num(i) * tree_num(n - 1 - i);
    }
    return res;
}

/**
 * 解法二：DP
 * */
int tree_num_dp(int n) {
    if (n < 2) {
        return 1;
    }
    int *memory = new int[n + 1];
    memory[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= i - 1; j++) {
            /// 左侧节点个数为 j，右侧节点个数为 i-j-1
            memory[i] += memory[j] * memory[i - j - 1];
        }
    }
    return memory[n];
}

/**
 * 问题二：给定一个括号字符串 s，计算至少需要添加多少个括号使其成为一个完成的括号字符串。
 * 例如，对于 "(()()"，我们只需在 idx = 4 的位置添加一个右括号即可。
 *
 * 解法一：用栈
 * */
int brackets(string s) {
    if (s.empty()) {
        return 0;
    }
    stack<char> st;
    for (char c1: s) {
        if (st.empty()) {
            st.push(c1);
            continue;
        }
        char c2 = st.top();
        if (c2 == '(' && c1 == ')') {
            st.pop();
        } else {
            st.push(c1);
        }
    }
    return (int) st.size();
}

/**
 * 解法二：使用计数器
 * */
int brackets2(const string &s) {
    if (s.empty()) {
        return 0;
    }
    int cnt = 0, ans = 0;
    for (auto &c: s) {
        if (c == '(') {
            cnt++;
        } else {
            if (cnt == 0) {
                ans++;
            } else {
                cnt--;
            }
        }
    }
    return cnt + ans;
}

/**
 * 问题三：给定一个数组 arr，求差值为 k 的去重数字对。
 * e.g., [3, 2, 5, 7, 0, 0], k = 2，返回 (0,2), (3,5), (5,7)
 * */
void num_pair(const int *arr, int size, int k) {
    set<pair<int, int>> s;
    for (int i = 0; i < size; i++) {
        /// 找到比 arr[i] 大 k 的元（无需找比 arr[i] 小 k 的）
        for (int j = 0; j < size; j++) {
            if (arr[j] - arr[i] == k) {
                s.insert({arr[i], arr[j]});
            }
        }
    }
    for (auto &e: s) {
        cout << "(" << e.first << "," << e.second << ")" << endl;
    }
}

void num_pair2(const int *arr, int size, int k) {
    set<int> s;
    for (int i = 0; i < size; i++) {
        s.insert(arr[i]);
    }
    for (auto &e: s) {
        if (s.find(e + k) != s.end()) {
            cout << "(" << e << "," << e + k << ")" << endl;
        }
    }
}

/**
 * 问题四：给定一个包含 n 个整数元素的集合 A，一个包含 m 个整数元素的集合 B。
 * 定义 Magic 操作为：从一个集合中取出一个元素，放到另一个集合中，且操作过后每个集合的平均值都大于操作前。
 * 要求：
 * （1）不可以把一个集合的元素取空；
 * （2）值为 x 的元素从某个集合（不妨是 A）中取出放入另一个集合 B，
 *      但是 B 中已有值为 x 的元素，则 B 的平均值不变，A 的平均值可能会改变。
 * 问：最多可以进行多少次 Magic 操作？
 *
 * 解答：业务题，侧重于对问题本身的分析。
 * 分析后可以发现，若 A 和 B 平均值相等，则不可能执行任何 Magic 操作；
 * 若 A 的平均值大于 B，则不可能从 B 中取出元素满足 Magic 操作；
 * 但是从 A 到 B，可以拿出那些介于 aver(B) 和 aver(A) 之间的数。
 * */

double aver(const set<int> &s) {
    int res = 0;
    for (auto &e: s) {
        res += e;
    }
    return (double) res / (double) s.size();
}

int magic(set<int> &A, set<int> &B) {
    if (aver(A) == aver(B)) {
        return 0;
    }
    set<int> *max_s = aver(A) > aver(B) ? &A : &B;
    set<int> *min_s = max_s == &A ? &B : &A;
    int ans = 0;
    while (aver(*max_s) > aver(*min_s)) {
        /// 拿范围内最小的数，从而使得 A 和 B 平均值之差变小变得慢一些
        int cur_min = INT_MAX;
        for (auto &e: *max_s) {
            if ((double) e > aver(*min_s) && (double) e < aver(*max_s) &&
                min_s->find(e) == min_s->end()) {
                cur_min = min(cur_min, e);
            }
        }
        if (cur_min == INT_MAX) {
            break;
        }
        max_s->erase(cur_min);
        min_s->insert(cur_min);
        ans++;
        cout << aver(*max_s) << " " << aver(*min_s) << endl;
    }
    return ans;
}

/**
 * 问题五：给定一个合法括号序列，计算其深度。
 * */
int depth(const string &s) {
    if (s.empty()) {
        return 0;
    }
    int cnt = 0, ans = 0;
    for (auto &c: s) {
        if (c == '(') {
            cnt++;
        } else {
            cnt--;
        }
        ans = max(ans, cnt);
    }
    return ans;
}

/**
 * 问题六：给定一个括号组成的字符串，返回最长的合法括号子串。
 *
 * 定义 memory[i]：子串必须以 i 位置字符结尾，最长的有效长度。
 *
 * ( xxxx )：4+1
 * ) xxxx )：0
 * */
int find_max_substr(const string &s) {
    int memory[s.size()];
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            memory[i] = 0;
        }
    }
    memory[0] = 0;
    for (int i = 1; i < s.size(); i++) {
        if (s[i] == '(') {
            continue;
        }
        /// 重点
        int pre = i - memory[i - 1] - 1;
        if (pre >= 0 && s[pre] == '(') {
            memory[i] = memory[i - 1] + 2;
            if (pre - 1 >= 0) {
                memory[i] += memory[pre - 1];
            }
        }
    }

    int ans = 0;
    for (auto &e: memory) {
        ans = e > ans ? e : ans;
    }
    return ans;
}

int main() {
    cout << tree_num(5) << endl;
    cout << tree_num_dp(5) << endl;

    cout << brackets("(()(()()(())))())))))()()()((((()))(") << endl;
    cout << brackets2("(()(()()(())))())))))()()()((((()))(") << endl;

    int arr[] = {3, 2, 5, 7, 0, 0};
    num_pair(arr, 6, 2);
    cout << endl;
    num_pair2(arr, 6, 2);
    cout << endl;

    set<int> A = {1, 3, 5, 7, 9};
    set<int> B = {1, 2, 4};
    cout << magic(A, B) << endl;

    cout << depth("()()()") << endl;
    cout << depth("()((()))(())") << endl;
    cout << depth("(()())") << endl;

    cout << find_max_substr("())()(())()))(())") << endl;

    return 0;
}