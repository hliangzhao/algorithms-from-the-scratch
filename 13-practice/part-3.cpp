//
// Created by hliangzhao on 3/2/23.
//
#include <iostream>
#include <stack>

using namespace std;

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题一：给定一个存入整型数据的栈，要求对栈内元素进行升序排列。最多只能使用一个额外的栈存放临时数据，
 * 但不能将元素复制到别的数据结构中。
 * */
void ascending(stack<int> &s) {
    stack<int> tmp;     /// tmp 存放降序排序的元素
    while (!s.empty()) {
        if (tmp.empty()) {
            tmp.push(s.top());
            s.pop();
            continue;
        }
        if (tmp.top() >= s.top()) {
            tmp.push(s.top());
            s.pop();
        } else {
            int cur = s.top();
            s.pop();
            while (!tmp.empty()) {
                s.push(tmp.top());
                tmp.pop();
                if ((tmp.empty()) || (!tmp.empty() && tmp.top() >= cur)) {
                    tmp.push(cur);
                    break;
                }
            }
        }
    }
    while (!tmp.empty()) {
        s.push(tmp.top());
        tmp.pop();
    }
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题二：规定 1 对应于 A，2 对应于 B，...，26 对应于 Z。
 * 给定一个由数字组成的字符串，返回可能的转换结果的个数。例如，"111"可以转换为"AAA"、"AZ"和"ZA"，因此返回 3。
 *
 * （本题最早出现在 ch07/04）
 * */

/**
 * 方法一：暴力递归
 * */
int cast(const string &s, int i) {
    if (i == s.size()) {
        return 1;
    }
    if (s[i] == '0') {
        return 0;
    }
    int res;
    if (s[i] == '1') {
        /// 转一个字符或两个字符均可
        res = cast(s, i + 1);
        if (i + 1 < s.size()) {
            res += cast(s, i + 2);
        }
    }
    if (s[i] == '2') {
        /// 转一个字符
        res = cast(s, i + 1);
        /// 转两个字符，但第二个字符必须 <= '6'
        if (i + 1 < s.size() && s[i + 1] >= '0' && s[i + 1] <= '6') {
            res += cast(s, i + 2);
        }
    }
    /// 只能转一个字符
    if (s[i] >= '3') {
        res = cast(s, i + 1);
    }
    return res;
}

int str_num(const string &s) {
    return cast(s, 0);
}

/**
 * 方法二：DP
 * */
int str_num_dp(const string &s) {
    int memory[s.size() + 1];
    memory[s.size()] = 1;
    for (int i = (int) s.size() - 1; i >= 0; i--) {
        if (s[i] == '0') {
            memory[i] = 0;
        } else if (s[i] == '1') {
            memory[i] = memory[i + 1];
            if (i + 1 < s.size()) {
                memory[i] += memory[i + 2];
            }
        } else if (s[i] == '2') {
            memory[i] = memory[i + 1];
            if (i + 1 < s.size() && s[i + 1] >= '0' && s[i + 1] <= '6') {
                memory[i] += memory[i + 2];
            }
        } else {
            memory[i] = memory[i + 1];
        }
    }
    return memory[0];
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题三：二叉树每个节点都有一个 int 值，给定一棵二叉树，计算从根节点到叶节点的所有路径中，权值和最大的值。
 *
 * 解法一：树型 DP
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

/*
 * 方式一
 * */
int max_sum(Node *root) {
    if (root == nullptr) {
        return 0;
    }
    return root->val + max(max_sum(root->left), max_sum(root->right));
}

/*
 * 方式二
 * */
int max_sum2(Node *root) {
    if (root->left == nullptr && root->right == nullptr) {
        return root->val;
    }
    int next = INT_MIN;
    if (root->left != nullptr) {
        next = max_sum2(root->left);
    }
    if (root->right != nullptr) {
        next = max(next, max_sum2(root->right));
    }
    return root->val + next;
}

/**
 * 解法二：传统方法
 *
 * 这是一个先序遍历
 * */
void p(Node *node, int pre, int &max_sum) {
    if (node->left == nullptr && node->right == nullptr) {
        /// 只有在叶节点，才会更新 max_sum
        max_sum = max(max_sum, pre + node->val);
        return;
    }
    if (node->left != nullptr) {
        p(node->left, pre + node->val, max_sum);
    }
    if (node->right != nullptr) {
        p(node->right, pre + node->val, max_sum);
    }
}

int max_sum3(Node *root) {
    int max_sum = INT_MIN;
    p(root, 0, max_sum);
    return max_sum;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题四：给定一个元素为非负整数的二维数组 mat，每行和每列都是从小到大有序的。
 * 给定一个非负整数 aim，判断 aim 是否在 mat 中。
 *
 * 复杂度 O(m + n)
 *
 * 相似的另一个问题是：给定一个 01 矩阵，每一行左边都是 0，右边都是 1。返回 1 最多的那行。
 * 解法：从右上角出发，找到第一行最左端的 1，然后往下走看能不能左扩。如果能，则更新目前已知的最大行；如果不能则直接往下走。
 * 如此反复。只可能往左下方走，因此复杂度为 O(m + n)。
 * */
bool is_aim_in(int **mat, int size1, int size2, int aim) {
    int i = 0, j = size2 - 1;
    while (i < size1 && j >= 0) {
        if (mat[i][j] == aim) {
            return true;
        }
        if (mat[i][j] > aim) {
            j -= 1;
        } else if (mat[i][j] < aim) {
            i += 1;
        }
    }
    return false;
}

int main() {
//    stack<int> s;
//    s.push(5);
//    s.push(6);
//    s.push(3);
//    s.push(2);
//    s.push(4);
//    ascending(s);
//    while (!s.empty()) {
//        cout << s.top() << " ";
//        s.pop();
//    }
//    cout << endl;
//
//    cout << str_num("11012349") << endl;
//    cout << str_num_dp("11012349") << endl;
//
//    Node *root = new_node(1);
//    root->left = new_node(2);
//    root->right = new_node(3);
//    root->left->left = new_node(5);
//    root->right->left = new_node(9);
//    root->right->right = new_node(7);
//    cout << max_sum(root) << endl;
//    cout << max_sum2(root) << endl;
//    cout << max_sum3(root) << endl;

    int **mat = new int *[3];
    for (int i = 0; i <3; i++) {
        mat[i] = new int[4];
    }
    mat[0][0] = 1;
    mat[0][1] = 5;
    mat[0][2] = 9;
    mat[0][3] = 10;
    mat[1][0] = 2;
    mat[1][1] = 6;
    mat[1][2] = 11;
    mat[1][3] = 13;
    mat[2][0] = 7;
    mat[2][1] = 9;
    mat[2][2] = 15;
    mat[2][3] = 17;
    cout << is_aim_in(mat, 3, 4, 7) << endl;

    return 0;
}