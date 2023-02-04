//
// Created by hliangzhao on 3/2/23.
//
#include <iostream>
#include <stack>
#include <iomanip>

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
 * 问题三解法二：传统方法
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

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题五：给定一个非负整数数组，每个元素代表每个位置上物品的个数。每个物品可以在相邻的位置上移动。
 * 计算在最小移动轮数的前提下，使得每个位置上的物品数量相同。如果无法做到，返回 -1。
 *
 * e.g. [1, 0, 5]:
 * 1 0 <-- 5 得到 1 1 4
 * 1 <-- 1 <-- 4 得到 2 1 3
 * 2 1 <-- 3 得到 2 2 2
 * 返回 3。
 * */

/**
 * 错解！！！
 * */
bool equal(const int *arr, int size) {
    int val = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] != val) {
            return false;
        }
    }
    return true;
}

int get_max(const int *arr, int size) {
    int res = arr[0];
    int idx = 0;
    for (int i = 1; i < size; i++) {
        if (res < arr[i]) {
            idx = i;
            res = arr[i];
        }
    }
    return idx;
}

int get_min(const int *arr, int size) {
    int res = arr[0];
    int idx = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < res) {
            idx = i;
            res = arr[i];
        }
    }
    return idx;
}

int move(int *arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    if (sum % size != 0) {
        return -1;
    }
    int ans = 0;
    while (!equal(arr, size)) {
        int min_idx = get_min(arr, size);
        int max_idx = get_max(arr, size);
        arr[min_idx]++;
        arr[max_idx]--;
        ans++;
    }
    return ans;
}

int get_sum(const int *arr, int start, int end) {
    int ans = 0;
    for (int i = start; i < end; i++) {
        ans += arr[i];
    }
    return ans;
}

/**
 * 正确解法（贪心）。
 * 分析每个位置左右两侧各需要多少，找到最大的瓶颈。
 * */
int move2(const int *arr, int size) {
    int sum = get_sum(arr, 0, size);
    if (sum % size != 0) {
        return -1;
    }
    int aver = sum / size;
    int final;
    int ans;
    int left_sum = 0;
    for (int i = 0; i < size; i++) {
        int left_out = left_sum - aver * i;
        int right_out = (sum - left_sum - arr[i]) - aver * (size - i - 1);
        if (left_out < 0 && right_out < 0) {
            ans = -left_out - right_out;
        } else {
            ans = max(abs(left_out), abs(right_out));
        }
        if (ans > final) {
            final = ans;
        }
        left_sum += arr[i];
    }
    return final;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题六：使用 zigzag 的方式打印矩阵。
 * 0 1 2 3
 * 4 5 6 7
 * 8 9 10 11
 *
 * 打印 0 1 4 8 5 2 3 6 9 10 7 11
 * */
void zigzag(int **mat, int width, int height) {
    int i = 0, j = 0;
    cout << mat[i][j] << " ";
    while (true) {
        /// 前半部分往右走一步，后半部分往下走一步
        if (j + 1 < height) {
            j++;
        } else {
            i++;
        }
        if (i == width - 1 && j == height - 1) {
            cout << mat[i][j] << " ";
            break;
        }
        /// 往左下走
        while (i + 1 < width && j - 1 >= 0) {
            cout << mat[i][j] << " ";
            i++;
            j--;
        }
        cout << mat[i][j] << " ";
        /// 前半部分往下走一步，后半部分往右走一步
        if (i + 1 < width) {
            i++;
        } else {
            j++;
        }
        /// 往右上走
        while (i - 1 >= 0 && j + 1 < height) {
            cout << mat[i][j] << " ";
            i--;
            j++;
        }
        cout << mat[i][j] << " ";
    }
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题七：蛇形填数
 * 0 1 2 3
 * 4 5 6 7
 * 8 9 10 11
 *
 * 打印 0 1 2 3 7 11 10 9 8 4 5 6
 *
 * 本函数会改变传入的 mat，如果题目不允许，先复制 mat。
 * */
void snake(int **mat, int width, int height) {
    int i = 0, j = 0;
    while (true) {
        while (j < height && mat[i][j] != -1) {
            cout << mat[i][j] << " ";
            mat[i][j] = -1;
            j++;
        }
        j -= 1;
        i += 1;
        while (i < width && mat[i][j] != -1) {
            cout << mat[i][j] << " ";
            mat[i][j] = -1;
            i++;
        }
        i -= 1;
        j -= 1;
        while (j >= 0 && mat[i][j] != -1) {
            cout << mat[i][j] << " ";
            mat[i][j] = -1;
            j--;
        }
        j += 1;
        i -= 1;
        while (i >= 0 && mat[i][j] != -1) {
            cout << mat[i][j] << " ";
            mat[i][j] = -1;
            i--;
        }
        i += 1;
        j += 1;
        bool unfinished = false;
        for (int pi = 0; pi < width; pi++) {
            for (int pj = 0; pj < height; pj++) {
                if (mat[pi][pj] != -1) {
                    unfinished = true;
                    break;
                }
            }
            if (unfinished) {
                break;
            }
        }
        if (!unfinished) {
            break;
        }
    }
}

/**
 * 问题七解法二：从整体到局部
 * */

/**
 * 子程序：打印从 (a,b) 到 (c,d) 的边
 * */
void print_edge(int **mat, int a, int b, int c, int d) {
    if (a == c) {
        /// 一条水平线
        for (int j = b; j <= d; j++) {
            cout << mat[a][j] << " ";
        }
    } else if (b == d) {
        for (int i = a; i <= c; i++) {
            cout << mat[i][b] << " ";
        }
    } else {
        int cur_col = b, cur_row = a;
        while (cur_col != d) {
            cout << mat[a][cur_col] << " ";
            cur_col++;
        }
        while (cur_row != c) {
            cout << mat[cur_row][d] << " ";
            cur_row++;
        }
        while (cur_col != b) {
            cout << mat[c][cur_col] << " ";
            cur_col--;
        }
        while (cur_row != a) {
            cout << mat[cur_row][b] << " ";
            cur_row--;
        }
    }
}

void snake2(int **mat, int width, int height) {
    int a = 0, b = 0;
    int c = width - 1, d = height - 1;
    while (a <= c && b <= d) {
        print_edge(mat, a, b, c, d);
        a++;
        b++;
        c--;
        d--;
    }
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 问题八：只用有限几个变量，实现矩阵中每个位置的数顺时针转动 90 度。
 *
 * 0  1  2  3
 * 4  5  6  7
 * 8  9 10 11
 * 12 13 14 15
 *
 * \to
 *
 * 12  8  4  0
 * 13  9  5  1
 * 14 10  6  2
 * 15 11  7  3
 *
 * 思路：从整体到局部。外面一圈转完之后不可能有元素转到里面去。因此，可以从外到内一圈一圈地去考虑。
 * 对于每一圈，将其上的元素分组，每一组内部的元素交换位置。
 * */

void rotate_edge(int **mat, int a, int b, int c, int d) {
    int tmp;
    for (int grp_idx = 0; grp_idx != d - b; grp_idx++) {
        tmp = mat[a][b + grp_idx];
        mat[a][b + grp_idx] = mat[c - grp_idx][b];
        mat[c - grp_idx][b] = mat[c][d - grp_idx];
        mat[c][d - grp_idx] = mat[a + grp_idx][c];
        mat[a + grp_idx][c] = tmp;
    }
}

void rotate(int **mat, int width, int height) {
    int a = 0, b = 0;
    int c = width - 1, d = height - 1;
    while (a <= c && b <= d) {
        rotate_edge(mat, a, b, c, d);
        a++;
        b++;
        c--;
        d--;
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            cout << setw(3) << mat[i][j];
        }
        cout << endl;
    }
}

// --------------------------------------------------------------------------------------------------------------

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
//
//    int **mat = new int *[3];
//    for (int i = 0; i <3; i++) {
//        mat[i] = new int[4];
//    }
//    mat[0][0] = 1;
//    mat[0][1] = 5;
//    mat[0][2] = 9;
//    mat[0][3] = 10;
//    mat[1][0] = 2;
//    mat[1][1] = 6;
//    mat[1][2] = 11;
//    mat[1][3] = 13;
//    mat[2][0] = 7;
//    mat[2][1] = 9;
//    mat[2][2] = 15;
//    mat[2][3] = 17;
//    cout << is_aim_in(mat, 3, 4, 7) << endl;
//
//    int arr[] = {100, 0, 0, 0};
//    cout << move2(arr, 4) << endl;
//
//    int **mat2 = new int *[4];
//    for (int i = 0; i < 4; i++) {
//        mat2[i] = new int[5];
//        for (int j = 0; j < 5; j++) {
//            mat2[i][j] = 5 * i + j;
//        }
//    }
//    zigzag(mat2, 4, 5);
//    cout << endl;
//    snake2(mat2, 3, 4);
//    cout << endl;
//    snake(mat2, 3, 4);
//    cout << endl;

    int **mat3 = new int *[4];
    for (int i = 0; i < 4; i++) {
        mat3[i] = new int[4];
        for (int j = 0; j < 4; j++) {
            mat3[i][j] = 4 * i + j;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << setw(3) << mat3[i][j];
        }
        cout << endl;
    }
    cout << endl;
    rotate(mat3, 4, 4);
    cout << endl;

    return 0;
}