//
// Created by hliangzhao on 26/1/23.
//
#include <iostream>

using namespace std;

/**
 * 问题：给定两个字符串，判断其中一个字符串是否是另一个的子串。
 * 若是，返回长字符串匹配短字符串的起始 index。
 * */

/**
 * 基本解法：遍历判断每一个开头是否可以成功匹配。
 * 此时复杂度为 O(nm)。
 * */
int is_sub_str(const string &s1, const string &s2) {
    for (int i = 0; i <= s1.size() - s2.size(); i++) {
        /// 判断 i 开头的子串是否匹配
        bool match = true;
        for (int j = i; j < i + s2.size(); j++) {
            if (s1[j] != s2[j - i]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i;
        }
    }
    return -1;
}

/**
 * KMP 算法：基本过程和解法一相同，区别是：某些开头必然不可能匹配，我们要识别出这些开头并且跳过。
 * */

/**
 * 计算 next 数组：对于 s2，返回它的 next 数组。
 * next[i] 存放的是 s[i] 之前的子串 s2[0, ..., i-1] 中，当前缀和后缀相等时，最大的这个相等的数。
 * 这个值不能取 i-1，因为长度为 i-1 的前缀和后缀是同一个串，二者必然相等，此时无意义。
 * e.g., "1121129" 这个串中，'9' 对应的值为 3。
 *
 * 这是实现复杂度过高。更好的实现只需要 O(n)，其中 n 为 s 的 size。
 * */
int *get_next(const string &s) {
    int *next = new int[s.size()];
    next[0] = -1;
    next[1] = 0;
    for (int i = 2; i < s.size(); i++) {
        int max_len = 0;
        for (int j = 1; j < i; j++) {
            /// 判断长度为 j 的前缀 s[0...j-1] 和后缀 s[i-j...i-1] 是否相同
            int same = true;
            for (int k = 0; k < j; k++) {
                if (s[k] != s[k + i - j]) {
                    same = false;
                    break;
                }
            }
            if (same) {
                max_len = j;
            }
        }
        next[i] = max_len;
    }
    return next;
}

/**
 * 更好的实现。next[i] 可以基于 next[i-1] 的值和一个判断来直接计算（动态规划）。
 * 具体地，若 s[next[i-1]] == s[i]，皆大欢喜，next[i] = next[i-1] + 1；
 * 否则递归往前判断 s[next[next[i-1]]] 和 s[i] 是否相等。
 * */
int *get_next2(const string &s) {
    if (s.size() == 1) {
        /// 注意，C++ 中，这种写法要求 arr length 要指明，不能为空
        return new int[1]{-1};
    }
    int *next = new int[s.size()];
    next[0] = -1;
    next[1] = 0;
    int i = 2;
    int cn = 0;             /// cn 存放 next 的嵌套
    while (i < s.size()) {
        if (s[i-1] == s[cn]) {
            next[i++] = ++cn;   /// cn 自增是为了下一个位置，也就是 i+1 的 next 值的计算而使用
        } else if (cn > 0) {    /// cn > 0 意味着 cn 还可以往前跳
            cn = next[cn];
        } else {                /// s[i-1] \neq s[cn] 且 cn 为 0 或 -1
            next[i++] = 0;
        }
    }
    return next;
}

/**
 * x 和 y 是二者第一处不匹配的位置。
 * 此时，经典算法：x 回到 i+1，判断 i+1 开始的子串是否匹配；y 回到 0，用于重新一一对应。
 * KMP：x 不变，y 跳到 k 位置。这相当于跳过了 i+1,...,j-1 这些位置开头的子串，它们中一定不存在能和 s2 匹配的开头。
 * 这要求 next[y] 的值比 next[y] 更大。显然，这不可能（反证法）。
 * 此外，根据前缀和后缀相等的特性，我们可以保证 s1[j...x-1] 和 s2[0...k-1] 相等，因此从 k 位置开始比较是合理的。
 * s1: [i,i+1,...........|j.....x]
 *
 *      ======            ======
 * s2: [012345|k.........|......y]
 * */
int kmp(const string &s1, const string &s2) {
    if (s1.empty() || s2.empty() || s1.size() < s2.size()) {
        return -1;
    }
    int *next = get_next(s2);               /// O（M）。M 是 s2 的长度
    int i = 0, j = 0;
    while (i < s1.size() && j < s2.size()) {   /// O（N）。N 是 s1 的长度。用 i 和 i-j 的最大移动来估计，即 2*N
        if (s1[i] == s2[j]) {
            i++;
            j++;
        } else if (j == 0) { /// next[j] == -1，此时无法再往回跳了，只能换一个开头重开
            i++;
        } else {
            j = next[j];
        }
    }
    /// j 成功走到末尾，匹配成功了。否则意味着 i 已经走到了 s1 的末尾，但是 j 还没走到 s2 的末尾，这意味着匹配失败
    return j == s2.size() ? i - j : -1;
}

int main() {
    string s1 = "123112311230123421234", s2 = "1234";
    cout << is_sub_str(s1, s2) << endl;

    string s = "1234129";
    int *next = get_next(s);
    for (int i = 0; i < s.size(); i++) {
        cout << next[i] << " ";
    }
    cout << endl;
    int *next2 = get_next2(s);
    for (int i = 0; i < s.size(); i++) {
        cout << next2[i] << " ";
    }
    cout << endl;

    cout << kmp(s1, s2) << endl;

    return 0;
}