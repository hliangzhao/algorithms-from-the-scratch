//
// Created by hliangzhao on 22/2/23.
//

#include <iostream>

using namespace std;

/**
 * 问题一：给定一个字符串。计算其中「不同非空子序列」的个数。因为结果可能很大，故返回答案时需要对 10^9+7 取余。
 *
 * 问题分析：
 * 1、所谓子序列，即每个字符选或者不选，一共有 2^len(s) 种。我们需要返回的，是其中不同的子序列个的数。
 *
 * 2、取余运算对加法是可拆分的，例如：
 * a % m = x; b % m = y，则 (a + b) % m = (x + y) % m。
 *
 * 对减法则需要补 m 防止取模后出现负数所造成的影响：(a - b) % m = (x - y + m) % m。
 *
 * 3、我们记录全部以 x 字符结尾的子序列的个数，到目前为止已经统计了的全部子序列的个数减去以 x 字符结尾的子序列的个数，就是新增的、
 * 以 x 字符结尾的子序列个数。
 * */
int distinct_sub_seq(const string &s) {
    if (s.empty()) {
        return 0;
    }
    int cnt[26];
    int all = 1;
    int m = 1000000007;
    for (auto &c: s) {
        /// new_add 是来到本位置后，新增的、不重复的子序列个数
        /// cnt[x] 统计了以 x 结尾的子序列的个数
        int new_add = (all - cnt[c - 'a'] + m) % m;
        all = (all + new_add + m) % m;
        /// 更新 cnt[x]，因为本字符 x 被视察过后，新增的这些子序列必然以 x 结尾
        cnt[c - 'a'] = (cnt[c - 'a'] + new_add + m) % m;
    }
    /// 减 1 是为了去掉空集
    return (all - 1 + m) % m;
}

int main() {
    int a = 213, b = 15;
    cout << (a + b) % 7 << " " << (a % 7 + b % 7) % 7 << endl;
    cout << distinct_sub_seq("aba") << endl;
    return 0;
}