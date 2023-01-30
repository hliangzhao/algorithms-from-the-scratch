//
// Created by hliangzhao on 24/1/23.
//
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

/**
 * 要点：暴力递归（回溯）
 * 暴力递归和动态规划的基础。后者在本质上去对前者做剪枝 —— 让解只沿着最优子结构递归，而不是每一条路都去尝试。
 * */

// --------------------------------------------------------------------------------

/**
 * 问题一：汉诺塔问题。
 * 问题分解：用 proc(i, A, C, B) 表示把前 i 个盘子从 A 借助 B 移动到 C 上。
 * proc(i, A, C, B) 分解为
 * proc(i-1, A, B, C);
 * i 从 A 移动到 C;
 * proc(i-1, B, C, A);
 * */

void proc_of_hanoi(int i, char from, char to, char other, int &cnt) {
    if (i == 1) {   /// base case
        cout << i << ": " << from << " --> " << to << endl;
        cnt++;
    } else {
        proc_of_hanoi(i - 1, from, other, to, cnt);
        cout << i << ": " << from << " --> " << to << endl;
        cnt++;
        proc_of_hanoi(i - 1, other, to, from, cnt);
    }
}

int hanoi(int n) {
    int cnt = 0;
    proc_of_hanoi(3, 'A', 'C', 'B', cnt);
    return cnt;
}

// --------------------------------------------------------------------------------

/**
 * 问题二：打印一个字符串的全部子序列（包含空串）。
 * */

void proc_of_print_substr(const string &s, int i, const vector<char> &res) {
    if (i == s.size()) {    /// base case
        for (auto &c: res) {
            cout << c;
        }
        cout << endl;
        return;
    }

    /// 从 res 分叉出来两条路
    /// 第一条路：考虑 s[i]
    vector<char> res_keep(res);
    res_keep.push_back(s[i]);
    proc_of_print_substr(s, i + 1, res_keep);

    /// 第二条路：不考虑 s[i]
    vector<char> res_ignore(res);
    proc_of_print_substr(s, i + 1, res_ignore);
}

void print_substr(const string &s) {
    vector<char> res;
    proc_of_print_substr(s, 0, res);
}

/**
 * 问题二的方法二：节省 O(n) 的空间复杂度。
 * */

void proc_of_print_substr2(string &s, int i) {
    if (i == s.size()) {
        for (auto &c: s) {
            if (c != 0) {
                cout << c;
            }
        }
        cout << endl;
        return;
    }

    /// 分出来两条路（利用函数调用栈的暂存机制）
    /// 第一条：要 s[i]
    proc_of_print_substr2(s, i + 1);
    char tmp = s[i];

    /// 第二条：不要 s[i]
    s[i] = 0;
    proc_of_print_substr2(s, i + 1);
    s[i] = tmp;
}

void print_substr2(string &s) {
    proc_of_print_substr2(s, 0);
}

// --------------------------------------------------------------------------------

/**
 * 问题三：打印一个字符串的全部排列，要求不出现重复排列。
 *
 * 下面给出方法一。该方法不适合传入的字符串本身有重复字符的情况。
 * */

void proc_of_arrange1(const string &s, int i, const vector<char> &cur_arrange, vector<vector<char>> &res) {
    if (i == s.size()) {
        vector<char> one_solution;
        for (auto &c: cur_arrange) {
            one_solution.push_back(c);
        }
        res.push_back(one_solution);
        return;
    }
    for (auto &c: s) {
        vector<char> cur_arrange_copy(cur_arrange);
        if (find(cur_arrange_copy.begin(), cur_arrange_copy.end(), c) == cur_arrange_copy.end()) {
            cur_arrange_copy.push_back(c);
            proc_of_arrange1(s, i + 1, cur_arrange_copy, res);
        }
    }
}

vector<vector<char>> arrange1(const string &s) {
    vector<vector<char>> res;
    vector<char> cur;
    proc_of_arrange1(s, 0, cur, res);
    return res;
}

/**
 * 问题三方法二：省空间版本
 *
 * string s 自 i 位置往后的所有字符都可以放在 i 位置上。
 * 当传入的字符串有重复字符时，本方法会造成重复排列被当成两个。
 * */
void proc_of_arrange2(string &s, int i, vector<string> &res) {
    if (i == s.size()) {
        res.push_back(s);
    }
    for (int j = i; j < s.size(); j++) {
        /// 从 i 位置开始，每个元素都可以放在第 i 个位置上，因此这个 for 循环是遍历每一个可以放的
        /// 一旦交换完毕，就意味着我们决定把原来在 j 上的元素放到 i 上了
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;

        /// 然后，我们进入第 i+1 个字符的选择中
        proc_of_arrange2(s, i + 1, res);

        /// 递归结束之后，记得把 i 和 j 的字符调换回来
        /// 最终 s 不会被改变
        /// 这就是「回溯」
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

vector<string> arrange2(string &s) {
    vector<string> res;
    proc_of_arrange2(s, 0, res);
    return res;
}

/**
 * 问题三方法三：在方法二的基础上去重。使用一个 visited 数组来标记某个特定字符是否已经被访问过即可
 * */
void proc_of_arrange3(string &s, int i, vector<string> &res) {
    if (i == s.size()) {
        res.push_back(s);
    }
    bool visited[26] = {};
    ::memset(visited, false, sizeof(bool) * 26);
    for (int j = i; j < s.size(); j++) {
        if (!visited[s[j] - 'a']) {
            visited[s[j] - 'a'] = true;

            /// 从 i 位置开始，每个元素都可以放在第 i 个位置上，因此这个 for 循环是遍历每一个可以放的
            /// 一旦交换完毕，就意味着我们决定把原来在 j 上的元素放到 i 上了
            char tmp = s[i];
            s[i] = s[j];
            s[j] = tmp;

            /// 然后，我们进入第 i+1 个字符的选择中
            proc_of_arrange3(s, i + 1, res);

            /// 递归结束之后，记得把 i 和 j 的字符调换回来
            /// 最终 s 不会被改变
            /// 这就是「回溯」
            tmp = s[i];
            s[i] = s[j];
            s[j] = tmp;
        }
    }
}

vector<string> arrange3(string &s) {
    vector<string> res;
    proc_of_arrange3(s, 0, res);
    return res;
}

// --------------------------------------------------------------------------------

/**
 * 问题四：给定一个整型数组排成一排，玩家 A 和 B 依次拿走最左或最右的牌，二者都绝顶聪明，返回最后获胜者的分数。
 * 要求 A 先拿。
 * */


/**
 * 下面的解法是错的！它没有反映出「A 和 B 都绝顶聪明」这一点，只是简单的枚举。
 * */
void proc_of_card(const int *arr, int l, int r, int &score_a, int &score_b, vector<int> &res_a) {
    if (l + 1 <= r) {
        /// A 拿走左边的，然后 B 也拿走左边的
        int score_a_left(score_a + arr[l]);
        int score_b_left(score_b + arr[l + 1]);
        proc_of_card(arr, l + 2, r, score_a_left, score_b_left, res_a);
        /// A 拿走左边的，B 拿走右边的
        int score_b_right(score_b + arr[r]);
        proc_of_card(arr, l + 1, r - 1, score_a_left, score_b_right, res_a);

        /// A 拿走右边的，B 拿走左边的
        int score_a_right(score_a + arr[r]);
        int score_b_left2(score_b + arr[l]);
        proc_of_card(arr, l + 1, r - 1, score_a_right, score_b_left2, res_a);
        /// A 拿走右边的，B 也拿走右边的
        int score_b_right2(score_b + arr[r - 1]);
        proc_of_card(arr, l, r - 2, score_a_right, score_b_right2, res_a);
    } else if (l == r) {
        score_a += arr[l];
        res_a.push_back(score_a);
        return;
    } else {
        // l > r
        res_a.push_back(score_a);
        return;
    }
}

int play_card(const int *arr, int size) {
    int l = 0, r = size - 1;
    int score_a = 0, score_b = 0;
    vector<int> res;
    proc_of_card(arr, l, r, score_a, score_b, res);
    int max_score_of_a = INT_MIN;
    for (auto &s: res) {
        if (s > max_score_of_a) {
            max_score_of_a = s;
        }
    }
    return max_score_of_a;
}

/**
 * 下面给出问题四的正确解法。
 * */

int f1(const int *arr, int l, int r);

int s1(const int *arr, int l, int r);

/**
 * 先手函数
 * */
int f1(const int *arr, int l, int r) {
    if (l == r) {
        return arr[l];
    }
    return max(arr[l] + card_s(arr, l + 1, r), arr[r] + card_s(arr, l, r - 1));
}

/**
 * 后手函数
 * */
int s1(const int *arr, int l, int r) {
    if (l == r) {
        return 0;
    }
    /// 对方绝顶聪明，因此总会把差的情况留给我
    return min(card_f(arr, l + 1, r), card_f(arr, l, r - 1));
}

int winner_score1(const int *arr, int size) {
    /// 返回 A 和 B 中分数高的
    return max(card_f(arr, 0, size - 1), card_s(arr, 0, size - 1));
}

// --------------------------------------------------------------------------------

/**
 * 问题五：逆序一个栈。要求：不能申请额外的数据结构，只能使用递归函数。
 * */

/**
 * 抽象出递归子结构：移除栈底元素并返回该值
 * e.g.,
 *  ----------
 *  1  2  3  |
 * ----------
 *
 * f1: 1, last
 *         |
 * f2:     2, last
 *             |
 * f3:         3
 *
 *
 * */
int get_bottom(stack<int> &s) {
    /// 弹出栈顶元素，如果弹出之后栈空了，说明这就是栈底元素，返回它
    int res = s.top();
    s.pop();
    if (s.empty()) {
        return res;
    } else {
        /// 如果弹出之后栈不为空，说明上面的 res 不是栈底元素，此时我们把「弹出栈底元素」这项任务交给我们的递归后继者
        /// 递归后继者拿到的，是一个栈内元素减少了 1 的栈，因此，任务会「容易」一些
        /// 我们相信这个后继者一定可以拿到栈底元素，之后只需要把取出来的非栈底元素再按照「后取先进」的顺序塞回栈中即可。
        /// 该顺序由递归调用栈自动实现。
        int last = get_bottom(s);
        s.push(res);
        return last;
    }
}

/**
 * 抽象出递归子结构：取出栈底元素，然后将剩余部分逆序
 * */
void reverse_stack(stack<int> &s) {
    if (s.empty()) {
        return;
    }
    /// 每次取出栈底元素，紧接着把新栈逆序
    int var = get_bottom(s);
    reverse_stack(s);
    /// 然后把原栈底元素放到栈顶
    s.push(var);
    /// 如此反复，直到栈为空
}

// --------------------------------------------------------------------------------

/**
 * 问题六：规定 1 对应于 A，2 对应于 B，...，26 对应于 Z。
 * 给定一个由数字组成的字符串，返回可能的转换结果的个数。例如，"111"可以转换为"AAA"、"AZ"和"ZA"，因此返回 3。
 * */

/**
 * 解法一：该方法不适用于输入字符串含有 '0' 的情形
 * 抽象出递归子结构：将 [l,r] 中的前 1 或 2 个字符映射为一个字母，所形成的解的个数
 * */

char legal_mapping(const string &s, int l, int r);

void cast(const string &s, int l, int r, string &casted, vector<string> &res);

int get_cast_res(const string &s);

char legal_mapping(const string &s, int l, int r) {
    int val = 0;
    for (int i = l; i <= r; i++) {
        val = val * 10 + (s[i] - '0');
    }
    if (val > 26) {
        return '\0';
    }
    return 'A' + val - 1;
}

void cast(const string &s, int l, int r, string &casted, vector<string> &res) {
    if (l == r) {
        casted += legal_mapping(s, l, l);
        res.push_back(casted);
        return;
    } else if (l > r) {
        res.push_back(casted);
        return;
    } else {
        /// 方式一：转换 1 个数字
        string casted_copy(casted);
        casted_copy += legal_mapping(s, l, l);
        cast(s, l + 1, r, casted_copy, res);

        /// 方式二：转换 2 个数字
        char c = legal_mapping(s, l, l + 1);
        if (c != '\0') {
            string casted_copy2(casted + c);
            cast(s, l + 2, r, casted_copy2, res);
        }
    }
}

int get_cast_res(const string &s) {
    int l = 0, r = s.size() - 1;
    string casted;
    vector<string> res;
    cast(s, l, r, casted, res);
    for (auto &casted_s: res) {
        cout << casted_s << endl;
    }
    return res.size();
}

/**
 * 解法二：弥补解法一的缺陷，含 '0' 的字符串也可以处理
 * */

char legal_mapping2(const string &s, int l, int r);

void cast2(const string &s, int l, int r, string &casted, vector<string> &res, bool &legal);

int get_cast_res2(const string &s);

char legal_mapping2(const string &s, int l, int r) {
    if (s[l] == '0') {
        return '\0';
    }
    int val = 0;
    for (int i = l; i <= r; i++) {
        val = val * 10 + (s[i] - '0');
    }
    if (val == 0 || val > 26) {
        return '\0';
    }
    return 'A' + val - 1;
}

void cast2(const string &s, int l, int r, string &casted, vector<string> &res, bool &legal) {
    if (!legal) {
        return;
    }
    if (l == r) {
        if (!legal_mapping2(s, l, l)) {
            legal = false;
            return;
        }
        legal = true;
        casted += legal_mapping2(s, l, l);
        res.push_back(casted);
        return;
    } else if (l > r) {
        res.push_back(casted);
        return;
    } else {
        /// 方式一：转换 1 个数字
        if (!legal_mapping2(s, l, l)) {
            legal = false;
            return;
        }
        legal = true;
        string casted_copy(casted);
        casted_copy += legal_mapping2(s, l, l);
        cast2(s, l + 1, r, casted_copy, res, legal);

        /// 方式二：转换 2 个数字
        if (!legal_mapping2(s, l, l + 1)) {
            legal = false;
            return;
        }
        legal = true;
        char c = legal_mapping2(s, l, l + 1);
        if (c != '\0') {
            string casted_copy2(casted + c);
            cast2(s, l + 2, r, casted_copy2, res, legal);
        }
    }
}

int get_cast_res2(const string &s) {
    int l = 0, r = s.size() - 1;
    string casted;
    vector<string> res;
    bool legal = true;
    cast2(s, l, r, casted, res, legal);
    for (auto &casted_s: res) {
        cout << casted_s << endl;
    }
    return res.size();
}


/**
 * 解法三：来自左神的解法。因为题目不要求返回具体的转化结果，因此只需要对个数进行递归，开销更低
 * 递归子结构抽象：自 i 位置开始进行转化，返回所有可能的转化的个数
 * */

int proc_of_cast(const string &s, int i) {
    if (i == s.size()) {
        /// 之前的转化都合法，抵达末尾，因此这对应于一个合法解
        return 1;
    }
    if (s[i] == '0') {
        /// '0' 不能单独被转化，也不能和后继一起被转化，因此本次尝试失败，不对应任何解
        return 0;
    }
    if (s[i] == '1') {
        /// 转化 1 个字符或 2 个字符的结果都考虑进来
        int res = proc_of_cast(s, i + 1);
        if (i + 1 < s.size()) {
            res += proc_of_cast(s, i + 2);
        }
        return res;
    }
    if (s[i] == '2') {
        /// 转化 1 个字符或 2 个字符的结果都考虑进来
        /// 如果是 2 个字符，则需要 check 是否合法
        int res = proc_of_cast(s, i + 1);
        if (i + 1 < s.size() && (s[i + 1] >= '0' && s[i + 1] <= '6')) {
            res += proc_of_cast(s, i + 2);
        }
        return res;
    }
    /// s[i] >= 3，只能转化一个字符，往前走一步
    return proc_of_cast(s, i + 1);
}

// --------------------------------------------------------------------------------

/**
 * 问题七：给定两个长度为 n 的数组 weights 和 values，分别代表 n 个货物的重量和价值。
 * 给定一个载重为 m 的袋子，返回能装下的物品的最大 value 和。
 * */

/**
 * 解法一：抽象出递归子结构：自 i 开始的货物自由选择，将所形成的最大 value 和放入 res 中
 * */

void proc_of_value_sum(const int *weights, const int *values, int i, int size,
                       int capacity, int cur_weight, int cur_value, vector<int> &res) {
    if (i == size) {
        res.push_back(cur_value);
        return;
    }
    if (cur_weight + weights[i] <= capacity) {
        /// 第一条路：要
        int new_weight1(cur_weight + weights[i]);
        int new_value1(cur_value + values[i]);
        proc_of_value_sum(weights, values, i + 1, size, capacity, new_weight1, new_value1, res);

        /// 第二条路：不要
        proc_of_value_sum(weights, values, i + 1, size, capacity, cur_weight, cur_value, res);
    } else {
        proc_of_value_sum(weights, values, i + 1, size, capacity, cur_weight, cur_value, res);
    }
}

int get_max_value_sum(const int *weights, const int *values, int size, int capacity) {
    int i = 0;
    int cur_weight = 0, cur_value = 0;
    vector<int> res;
    proc_of_value_sum(weights, values, i, size, capacity, cur_weight, cur_value, res);
    int max_val_sum = INT_MIN;
    for (auto &v: res) {
        cout << v << endl;
        if (v > max_val_sum) {
            max_val_sum = v;
        }
    }
    return max_val_sum;
}

/**
 * 解法二：递归子结构：自 i 开始的货物自由选择，返回「自 i 开始的货物所形成的」最大 value 和
 * 我现在的思考问题的方式，必须将解的过程描述出来。然而，这是不必要的，解法二的递归更为简洁。
 * */

int proc_of_value_sum2(const int *weights, const int *values, int size, int capacity, int i, int cur_weight) {
    if (cur_weight > capacity) {
        /// 当前解不是一个合法解，将非法的解的 value 和直接置为负无穷，这样必然不会被选中
        return INT_MIN;
    }
    if (i == size) {
        /// 没有可以选的货物了，因此，自"本货物"开始所形成的 value 和为 0
        return 0;
    }
    /// 要还是不要，选大的
    return max(proc_of_value_sum2(weights, values, size, capacity, i + 1, cur_weight),
               values[i] + proc_of_value_sum2(weights, values, size, capacity, i + 1, cur_weight + weights[i]));
}

/**
 * 解法三：递归子结构：自 i 开始的货物自由选择，返回最大 value 和。
 * 整体和解法二类似，但是解存放到入参中了。
 * */

int proc_of_value_sum3(const int *weights, const int *values, int size, int capacity, int i, int cur_weight, int cur_value) {
    if (cur_weight > capacity) {
        /// 当前解不是一个合法解，将非法的解的 value 和直接置为负无穷，这样必然不会被选中
        return INT_MIN;
    }
    if (i == size) {
        /// 没有可以选的货物了，因此，自"本货物"开始所形成的 value 和为 0
        return cur_value;
    }
    /// 要还是不要，选大的
    return max(proc_of_value_sum3(weights, values, size, capacity, i + 1, cur_weight, cur_value),
               proc_of_value_sum3(weights, values, size, capacity, i + 1, cur_weight + weights[i], cur_value + values[i]));
}

int main() {
//    cout << hanoi(3) << endl;
//
//    string s = "abc";
//    print_substr(s);
//    print_substr2(s);

//    string s2 = "abc";
//    vector<vector<char>> res = arrange1(s2);
//    for (auto &solution: res) {
//        for (auto &c: solution) {
//            cout << c;
//        }
//        cout << endl;
//    }
//    cout << endl;
//
//    string s3 = "abcc";
//    vector<string> res2 = arrange2(s3);
//    for (auto &solution: res2) {
//        for (auto &c: solution) {
//            cout << c;
//        }
//        cout << endl;
//    }
//    cout << endl;
//    vector<string> res3 = arrange3(s3);
//    for (auto &solution: res3) {
//        for (auto &c: solution) {
//            cout << c;
//        }
//        cout << endl;
//    }
//    cout << endl;

//    int arr[] = {1, 2, 100, 4};
//    cout << play_card(arr, 4) << endl;
//    cout << winner_score(arr, 4) << endl;

//    stack<int> s;
//    for (int i = 1; i <= 3; i++) {
//        s.push(i);
//    }
//    stack<int> s_kept(s);
//    while (!s_kept.empty()) {
//        cout << s_kept.top() << endl;
//        s_kept.pop();
//    }
//    cout << endl;
//
//    reverse_stack(s);
//    while (!s.empty()) {
//        cout << s.top() << endl;
//        s.pop();
//    }

//    cout << get_cast_res("1112") << endl;
//    cout << get_cast_res2("11012349") << endl;
//    cout << proc_of_cast("11012349", 0) << endl;

    int weights[] = {3, 6, 2, 3};
    int values[] = {30, 100, 50, 90};
    int size = 3, capacity = 6;
    cout << get_max_value_sum(weights, values, size, capacity) << endl << endl;
    cout << proc_of_value_sum2(weights, values, size, capacity, 0, 0) << endl << endl;
    cout << proc_of_value_sum3(weights, values, size, capacity, 0, 0, 0) << endl;

    return 0;
}
