//
// Created by hliangzhao on 27/1/23.
//
#include <iostream>

using namespace std;

/**
 * 求解字符串中的最长回文子串的长度。
 * 解法：在每个字符之间插入特殊字符（保证偶数回文串也可以被找到），以每个字符为对称轴向左右两边扩散。
 * 这个特殊字符可以是任意字符，即使和字符串中已有字符重复也无所谓。
 *
 * 该方法复杂度为 O(n^2)
 * */
int find_max_palindrome(const string &s) {
    if (s.empty()) {
        return 0;
    }
    string s_copy = "#";
    for (auto &c: s) {
        s_copy.append(1, c);
        s_copy.push_back('#');  /// C++ 中，字符串是容器，因此具备容器的方法
    }
    int max_len = INT_MIN;
    for (int i = 0; i < s_copy.size(); i++) {
        int k = 0;
        while (i - k >= 0 && i + k < s_copy.size()) {
            if (s_copy[i - k] != s_copy[i + k]) {
                break;
            } else {
                k++;
            }
        }
        max_len = max_len > 2 * (k - 1) + 1 ? max_len : 2 * (k - 1) + 1;
    }
    return max_len / 2;
}

/**
 * Manacher 算法：将上述问题的复杂度降到 O(n)
 * 1、相关概念：
 *      回文直径 diameter[]；
 *      回文半径 radius[]；
 *      之前所扩的所有位置中，所能抵达的最右边界的位置 R；
 *      更新最右边界时，中心点的位置 c。
 *
 * 2、算法加速
 *      来到任意位置 i 时，如果 i > R（即 i 不在 c 的右回文半径区域内），则无加速。注意，一定有 i>= c。
 *      如果 i >= R，则可以加速。具体地，
 *
 *      情况一：i 以 c 为中心的左对称点 j 在 c 的回文直径范围内（以 j 为中心的回文串左边界 > L），
 *      则 i 的回文半径和回文直径必然和 j 相同，不用再算了，直接赋值过来即可；
 *
 *      情况二：i 以 c 为中心的左对称点 j 不在 c 的回文直径范围内（以 j 为中心的回文串左边界 < L）。
 *      此时 i 的回文半径就是 R - i，直接赋值。
 *
 *      情况三：以 j 为中心的回文串左边界 == L（压线）。此时只需要从 R+1 的位置开始探索，而不需要从 i 开始一个一个比对。
 *
 * Case #1:
 * {     [   |===|     |===|   ]     }
 *       L     j    c    i     R
 *
 * Case #2:
 * { |==[===============]==|         }
 *      L  j    c    i  R
 * */
int manacher(const string &s) {
    if (s.empty()) {
        return 0;
    }

    string s_copy = "#";
    for (auto &c: s) {
        s_copy.append(1, c);
        s_copy.push_back('#');
    }

    /// R 是最右边界右边的第一个位置
    int R = -1, c = -1;
    int radius[s_copy.size()];
    int max_len = INT_MIN;
    for (int i = 0; i < s_copy.size(); i++) {
        if (i < R) {    /// 此时不用算，下面的写法将可加速的三种情况进行了合并
            radius[i] = min(radius[2 * c - i], R - i);  /// 2*c-i 就是 j
        } else {
            radius[i] = 1;      /// 否则回文子串的长度至少是 1，解析来要开始验证
        }
//        radius[i] = i < R ? min(radius[2 * c - i], R - i) : 1;

        /// 从 i 的回文半径的右边界第一个位置开始，继续探索，看看能否进一步扩大回文半径
        /// 注意，虽然 4 种情况最终都会执行这一句，但是我们知道，可加速的情况一和情况二是不可能被继续扩展回文半径
        /// 所以对于这两种情况，必然在 if 处直接 break，因此但写无妨
        while (i + radius[i] < s_copy.size() && i - radius[i] >= 0) {
            if (s_copy[i + radius[i]] == s_copy[i - radius[i]]) {
                radius[i]++;
            } else {
                break;
            }
        }
        /// 更新
        if (i + radius[i] > R) {
            R = i + radius[i];
            c = i;
        }
        max_len = max(max_len, radius[i]);
    }
    /// s_copy 中，任意中心的最大回文半径和 s 该位置的最大回文直径的关系满足下式
    return max_len - 1;
}

int main() {
    string s = "9011123232987";
    cout << find_max_palindrome(s) << endl;
    cout << manacher(s) << endl;
    return 0;
}
