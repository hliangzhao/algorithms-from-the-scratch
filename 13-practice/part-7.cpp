//
// Created by hliangzhao on 8/2/23.
//
#include <iostream>
#include <regex>
#include <map>
#include <utility>

using namespace std;

/**
 * 问题一：给定一个字符串，如果该字符串符合人们日常书写一个整数的形式，返回 int 类型的这个数；否则报错。
 *
 * 分析：
 * （1）数字之外只能有 "-"；
 * （2）如果有 "-"，则必然在开头，且只能出现一次，后面必然跟数字且不为0；
 * （3）如果开头字符为 "0"，则后续必无字符。
 *
 * TODO：使用正则表达式实现 check 函数。
 * */
bool check(const string &s) {
    /// 第一个字符不是 '-' 也不是数字
    if (s[0] != '-' && (s[0] < '0' || s[0] > '9')) {
        return false;
    }
    /// "-" 或 "-0"
    if (s[0] == '-' && (s.size() == 1 || s[1] == '0')) {
        return false;
    }
    /// '0' 开头且后续有字符
    if (s[0] == '0' && s.size() > 1) {
        return false;
    }
    /// 开头已经检查完毕，必然是数字或 '-'
    for (int i = 1; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

/**
 * 尤其注意 -2147483648 和溢出的处理
 * */
int convert(const string &s) {
    if (check(s)) {
        bool neg = s[0] == '-';
        int min_q = INT_MIN / 10;
        int min_r = INT_MIN % 10;
        int res = 0, cur;
        for (int i = neg ? 1 : 0; i < s.size(); i++) {
            /// 每次都按照负数和来处理，保证 -2147483648 可以被正确转换
            cur = '0' - s[i];
            /// 判断中间值是否已经溢出
            if ((res < min_q) || (res == min_q && cur < min_r)) {
                throw runtime_error("Underflow! cannot convert");
            }
            res = res * 10 + cur;
        }
        if (!neg && res == INT_MIN) {
            /// 此时是 2147483648，向上溢出
            throw runtime_error("Overflow! cannot convert");
        }
        return neg ? res : -res;
    } else {
        throw runtime_error("cannot convert");
    }
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目二：给定一个字符串数组，每个字符串代表一个路径。把这些路径中蕴含的目录结构画出来。同级的需要按字母顺序排列。
 * 例如：{"b\\cst", "d\\", "a\\d\\e", "a\\b\\c"}，
 * 画出：
 * a
 *   b
 *     c
 *   d
 *     e
 * b
 *   cst
 * d
 *
 * 解：前缀树
 * */

struct TrieNode {
    string name;
    /// C++ 中，map 就是 ordered_map，默认就是按照 key 的字典序升序排列
//    map<string, TrieNode *, less<string>> next;
    map<string, TrieNode *> next;
};

TrieNode *new_trie_node(string name) {
    auto *node = new TrieNode;
    node->name = std::move(name);
    return node;
}

/**
 * C++ 分割字符串的标准写法，必须记住！！！
 * */
vector<string> split(const string &s, const string &pattern) {
    vector<string> res;
    if (s.empty()) {
        return res;
    }
    string s2(s + pattern);
    ::size_t pos = s2.find(pattern);
    while (pos != string::npos) {
        string subs = s2.substr(0, pos);
        if (!subs.empty()) {
            res.push_back(subs);
        }
        s2 = s2.substr(pos + 1, s2.size());
        pos = s2.find(pattern);
    }
    return res;
}

TrieNode *generate_tree(const string *folder_paths, int size) {
    auto *root = new_trie_node("");
    for (int i = 0; i < size; i++) {
        vector<string> paths = split(folder_paths[i], "\\");
        TrieNode *cur = root;
        for (auto &p: paths) {
            if (cur->next.find(p) == cur->next.end()) {
                cur->next.insert({p, new_trie_node(p)});
            }
            cur = cur->next[p];
        }
    }
    return root;
}

void print_paths(TrieNode *root, int level) {
    if (level != 0) {
        /// 根节点是空的，不要给空格，也不要打印
        for (int i = 0; i < level - 1; i++) {
            cout << "  ";
        }
        cout << root->name << endl;
    }
    for (auto &next: root->next) {
        if (next.second != nullptr) {
            print_paths(next.second, level + 1);
        }
    }
}

void solve(const string *folder_paths, int size) {
    auto *root = generate_tree(folder_paths, size);
    print_paths(root, 0);
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目三：如果把 last 看成是 left、next 看成是 right 的话，可以认为双向链表和二叉树的结构是一样的。
 * 给一棵 BST 的头节点 head，转换成一条有序的双向链表，并返回链表的头节点。
 *
 * 解：二叉树的递归套路。
 * */

struct Node {
    int val;
    Node *left, *right;
};

Node *new_node(int val) {
    auto *node = new Node;
    node->val = val;
    node->left = node->right = nullptr;
    return node;
}

struct Info {
    Node *first;
    Node *last;
};

Info *new_info(Node *f, Node *l) {
    auto *info = new Info;
    info->first = f;
    info->last = l;
    return info;
}

Info *construct(Node *root) {
    if (root == nullptr) {
        return nullptr;
    }
    auto *info_l = construct(root->left);
    auto *info_r = construct(root->right);
    if (info_l == nullptr && info_r == nullptr) {
        return new_info(root, root);
    } else if (info_l != nullptr && info_r == nullptr) {
        info_l->last->right = root;
        root->left = info_l->last;
        return new_info(info_l->first, root);
    } else if (info_l == nullptr) {
        info_r->first->left = root;
        root->right = info_r->first;
        return new_info(root, info_r->last);
    } else {
        info_l->last->right = root;
        root->left = info_l->last;
        info_r->first->left = root;
        root->right = info_r->first;
        return new_info(info_l->first, info_r->last);
    }
}

Node *bst2list(Node *root) {
    auto *info = construct(root);
    if (info == nullptr) {
        return nullptr;
    }
    return info->first;
}

void traverse(Node *head) {
    Node *cur = head;
    while (cur != nullptr) {
        cout << cur->val << " ";
        cur = cur->right;
    }
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目四：找到一棵二叉树中最大的搜索二叉子树（注意「子树」和「子部分」的区别），返回该子树的节点个数和该子树的根。
 *
 * 解一：层序遍历每个节点，以中序遍历判断以每个节点为根的子树是否为 BST，若是，记录其根和节点个数。
 * 解二：二叉树的递归套路。
 *
 * 下面给出解二。
 * */

struct Info2 {
    Node *head;         /// 当前子树中，如果存在 BST，该 BST 的根节点
    bool is_bst;
    int max_val;        /// 当前子树的节点最小值
    int min_val;        /// 当前子树的节点最大值
    int max_bst_size;   /// 当前子树中，如果存在 BST，则该 BST 的节点个数
};

Info2 *new_info2(Node *h, bool is_bst, int max_v, int min_v, int num) {
    auto *info = new Info2;
    info->head = h;
    info->is_bst = is_bst;
    info->max_val = max_v;
    info->min_val = min_v;
    info->max_bst_size = num;
    return info;
}

Info2 *judge(Node *root) {
    if (root == nullptr) {
        return nullptr;
    }
    Info2 *info_l = judge(root->left);
    Info2 *info_r = judge(root->right);

    /// max_v 和 min_v 是最好设置的，它们与左右子树是否是 BST 无关
    int max_v = root->val, min_v = root->val;
    if (info_l != nullptr) {
        max_v = max(max_v, info_l->max_val);
        min_v = min(min_v, info_l->min_val);
    }
    if (info_r != nullptr) {
        max_v = max(max_v, info_r->max_val);
        min_v = min(min_v, info_r->min_val);
    }

    /// 跨 root 的情况作为可能性三，只要存在，可能性一和二就必然不会是最终结果
    /// 因此把可能性一和二放到前面分析和讨论
    int node_num = 0;
    Node *head = nullptr;
    if (info_l != nullptr) {
        node_num = info_l->max_bst_size;
        head = info_l->head;
    }
    if (info_r != nullptr && info_r->max_bst_size > node_num) {
        node_num = info_r->max_bst_size;
        head = info_r->head;
    }

    bool is_bst = false;
    if (((info_l == nullptr) || info_l->is_bst) && ((info_r == nullptr) || info_r->is_bst)) {
        if (((info_l == nullptr) || (info_l->max_val < root->val)) &&
            ((info_r == nullptr) || (info_r->min_val > root->val))) {
            /// 如果这个 if 不成立，则以 root 为根的子树必然不是 BST，因此上面把 is_bst 初始化为 false 毫无问题！
            is_bst = true;
            head = root;
            int left_num = info_l == nullptr ? 0 : info_l->max_bst_size;
            int right_num = info_r == nullptr ? 0 : info_r->max_bst_size;
            node_num = left_num + right_num + 1;
        }
    }

    return new_info2(head, is_bst, max_v, min_v, node_num);
}

void solve2(Node *root) {
    if (root == nullptr) {
        return;
    }
    auto *info = judge(root);
    cout << "node num: " << info->max_bst_size << ", root: " << info->head->val << endl;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目五：给定一个数组，计算所有连续子数组中的最大的累加和。
 *
 * 解法一：双重遍历
 * */
int max_sum(const int *arr, int size) {
    int res[size];
    for (int i = 0; i < size; i++) {
        /// 计算以 i 为头的子数组的最大累加和
        int cur_max = arr[i], cur_sum = arr[i];
        for (int j = i + 1; j < size; j++) {
            cur_sum += arr[j];
            cur_max = cur_sum > cur_max ? cur_sum : cur_max;
        }
        res[i] = cur_max;
    }
    int ans = res[0];
    for (int i = 1; i < size; i++) {
        ans = ans < res[i] ? res[i] : ans;
    }
    return ans;
}

/**
 * 解法二：Kadane's algorithm（可以基于 DP 得到）
 * 从左到右扫一遍，综合小于 0 时重置计数位置，每次取最大值。
 *
 *
 * 如果数组中全是负数，则每次 cur 都是以 0 的姿态参与累加的，因此 res 返回的是最大的那个负数。
 * 如果数组中有负数和零，上述理解仍然正确，0 不会对累加和造成影响。
 * 如果数组中有正数、负数和零，则不妨设 [i...j] 是累加和最大的子序列中最长的那个。
 * 则必然有
 *
 * [...i.........j....]
 *   k      p
 * i <= p <= j 时，[i...p] 的累加和必然 >= 0；否则 [i...j] 应当变为 [p+1...j]；
 * 0 <= k <= i 时，[k...i] 的累加和必然 < 0；否则 [i...j] 应当变为 [k...j]；
 *
 * 为了让 res 只计算 [i...j] 的累加和，我们需要让进入 i 的时候 cur = 0。
 * */
int Kadane(const int *arr, int size) {
    int cur = 0, res = INT_MIN;
    for (int i = 0; i < size; i++) {
        cur += arr[i];
        res = max(res, cur);
        if (cur < 0) {
            cur = 0;
        }
    }
    return res;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目六：给定一个矩阵，计算所有子矩阵中的最大的累加和。
 * */
int max_sub_mat(int **mat, int h, int w) {
    if (mat == nullptr || h == 0 || w == 0) {
        return 0;
    }
    int res = INT_MIN;
    int cur;
    int *tmp;
//    ::memset(tmp, 0, sizeof(int) * w);
    for (int i = 0; i < h; i++) {
        tmp = new int[w]{};
        for (int j = i; j < h; j++) {
            /// 分析第 i 到第 j 行的累加和
            cur = 0;
            for (int k = 0; k < w; k++) {
                /// 把第 i 到第 j 行每一列的位置上的元素累加起来
                tmp[k] += mat[j][k];
                cur += tmp[k];
                res = max(res, cur);
                cur = cur < 0 ? 0 : cur;
            }
        }
    }
    return res;
}

// --------------------------------------------------------------------------------------------------------------

int main() {
//    cout << convert("-2147483648") << endl;
//    cout << convert("-0") << endl;
//    cout << convert("-123asd") << endl;
//    cout << convert("2147483648") << endl;
//
//    vector<string> res = split("\\a\\\\\\b\\", "\\");
//    for (auto &v: res) {
//        cout << v << endl;
//    }
//
//    string s[] = {"b\\cst", "d\\", "a\\d\\e", "a\\b\\c"};
//    solve(s, 4);
//
//    Node *root1 = new_node(3);
//    root1->left = new_node(1);
//    root1->left->left = new_node(0);
//    root1->left->right = new_node(2);
//    root1->right = new_node(5);
//    root1->right->left = new_node(4);
//    root1->right->right = new_node(9);
//    root1->right->right->right = new_node(11);
//    auto *head = bst2list(root1);
//    traverse(head);
//
//    Node *root2 = new_node(12);
//    root2->left = new_node(9);
//    root2->left->left = new_node(7);
//    root2->left->right = new_node(10);
//    root2->left->left->left = new_node(4);
//    root2->left->left->right = new_node(11);
//    root2->right = new_node(18);
//    root2->right->left = new_node(17);
//    root2->right->right = new_node(20);
//    root2->right->left->left = new_node(14);
//    root2->right->left->right = new_node(19);
//    solve2(root2);
//
//    Node *root3 = new_node(12);
//    root3->left = new_node(9);
//    root3->right = new_node(18);
//    root3->left->right = new_node(13);
//    root3->right->left = new_node(17);
//    root3->right->right = new_node(23);
//    root3->right->right->left = new_node(19);
//    root3->right->right->right = new_node(25);
//    solve2(root3);

    int arr[] = {1, 1, -1, -10, 11, 4, -6, 9, 20, -10, -2};
    cout << max_sum(arr, 11) << endl;

    int arr2[] = {3, 2, -1, 4, -9, 4, -2, 3, 4, -2, 6};
    cout << max_sum(arr2, 11) << endl;
    cout << Kadane(arr2, 11) << endl;

    int **mat = new int *[3];
    for (int i = 0; i < 3; i++) {
        mat[i] = new int[4];
    }
    mat[0][0] = -5;
    mat[0][1] = 3;
    mat[0][2] = 6;
    mat[0][3] = 4;
    mat[1][0] = -7;
    mat[1][1] = 9;
    mat[1][2] = -5;
    mat[1][3] = 3;
    mat[2][0] = -10;
    mat[2][1] = 1;
    mat[2][2] = -200;
    mat[2][3] = 5;
    cout << max_sub_mat(mat, 3, 4) << endl;

    return 0;
}