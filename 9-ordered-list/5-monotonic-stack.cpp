//
// Created by hliangzhao on 27/1/23.
//
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

/**
 * 问题一：给定一个数组，返回任意元素离它最近的、左右两边比它大的值。
 *
 * 方法一：下面的方法复杂度为 O(n^2)
 * */
void get_neighbor_max(const int *arr, int size, int *left_res, int *right_res) {
    for (int i = 0; i < size; i++) {
        int left_max = arr[i], right_max = arr[i];
        for (int j = i - 1; j >= 0; j--) {
            if (arr[j] > left_max) {
                left_max = arr[j];
                break;
            }
        }
        for (int j = i + 1; j < size; j++) {
            if (arr[j] > right_max) {
                right_max = arr[j];
                break;
            }
        }
        left_res[i] = left_max == arr[i] ? -1 : left_max;
        right_res[i] = right_max == arr[i] ? -1 : right_max;
    }
}

/**
 * 问题一方法二：使用单调栈，复杂度为 O(n)
 *
 * 给定一个栈，从栈底到栈顶元素值依次增大。每当试图新入栈的元素大于栈顶元素时（此时直接入栈会破环单调性），
 * 我们此时要先把栈顶元素弹出。对于该元素而言，新的栈顶元素就是其左边最大值，而即将入栈的元素就是其右边最大值。
 * 此时每个元素只进出栈一次，因此复杂度为 O(n)。
 *
 * 下面的代码只适用于数组中没有重复值的情形。
 * */
void use_mono_stack(const int *arr, int size, int *left_res, int *right_res) {
    stack<int> s;
    for (int i = 0; i < size; i++) {
        while (!s.empty() && arr[i] > arr[s.top()]) {
            int cur_idx = s.top();
            s.pop();
            if (!s.empty()) {
                left_res[cur_idx] = arr[s.top()];
            } else {
                left_res[cur_idx] = -1;
            }
            right_res[cur_idx] = arr[i];
        }
        s.push(i);
    }
    while (!s.empty()) {
        int cur_idx = s.top();
        s.pop();
        if (!s.empty()) {
            left_res[cur_idx] = arr[s.top()];
        } else {
            left_res[cur_idx] = -1;
        }
        right_res[cur_idx] = -1;
    }
}

/**
 * 如果给定数组中存在相同值元素，则需要压入栈的，是链表节点指针，相同值元素的下标将依次链如对应栈顶元素链表最后
 * */

struct Node {
    int idx;
    Node *next;
};

Node *new_node(int idx) {
    auto *node = new Node;
    node->idx = idx;
    node->next = nullptr;
    return node;
}

/**
 * 下面的代码能够处理给定数组中存在相同值元素的情况。
 * TODO：代码可以进一步梳理并简化。此外，栈中存放的元素最好是 vector，此时 vector 中元素的存取操作开销都是 O(1)。
 *     如果是链表，那么是 O（n）。
 * */
void use_mono_stack2(const int *arr, int size, int *left_res, int *right_res) {
    stack<Node *> s;
    for (int i = 0; i < size; i++) {
        /// s 为空，此时需要把 i 插入栈顶
        if (s.empty()) {
            Node *node_of_i = new_node(i);
            s.push(node_of_i);
        } else {
            /// s 不为空，要和栈顶元素对比。有三种情况
            bool non_push = false;
            while (!s.empty()) {
                if (arr[i] < arr[s.top()->idx]) {
                    /// arr[i] < arr[s.top()->idx]，此时直接把 i 放入栈顶
                    Node *node_of_i = new_node(i);
                    s.push(node_of_i);
                    break;
                } else if (arr[i] > arr[s.top()->idx]) {
                    /// i 所在链表中的所有元素都需要设置
                    Node *cur = s.top();
                    s.pop();
                    while (cur != nullptr) {
                        if (!s.empty()) {
                            left_res[cur->idx] = arr[s.top()->idx];
                        } else {
                            left_res[cur->idx] = -1;
                        }
                        right_res[cur->idx] = arr[i];
                        cur = cur->next;
                    }
                    /// 如此此时栈为空，则 i 无法被放入栈中，因此我们需要在 while 外面将这种情况下的 i 主动放入栈中
                    non_push = true;
                } else {
                    /// arr[i] == arr[s.top()->idx]，此时需要把 i 链接到 s.top() 所在链表的最后
                    Node *p = s.top();
                    while (p->next != nullptr) {
                        p = p->next;
                    }
                    p->next = new_node(i);
                    break;
                }
            }
            /// 因为栈空所以没有把 i 放入栈中
            if (s.empty() && non_push) {
                Node *node_of_i = new_node(i);
                s.push(node_of_i);
            }
        }
    }
    while (!s.empty()) {
        Node *cur = s.top();
        s.pop();
        while (cur != nullptr) {
            if (!s.empty()) {
                left_res[cur->idx] = arr[s.top()->idx];
            } else {
                left_res[cur->idx] = -1;
            }
            right_res[cur->idx] = -1;
            cur = cur->next;
        }
    }
}

/**
 * 问题二：给定一个正数数组，定义指标 A 为数组中累积和与最小值的乘积，返回子数组中指标 A 的最大值。
 * 注意，该数组最小值乘以该数组元素和并不是答案！！！不要想当然了。
 *
 * 为了让复杂度降到 O(n)，我们可以使用单调栈 ——
 * 遍历每一个位置 i，计算以 arr[i] 为最小值且累加和最大的子数组的指标 A，我们想要的 A 一定在这些 A 中。
 * 对每个位置 i，如何找到这样的子数组呢？我们可以借助单调栈返回 i 左右两边、离它最近的、小于 arr[i] 的位置。
 * */
int get_target_val(const int *arr, int size) {
    /// 先计算前缀和，减少遍历次数
    int *pre_sum = new int[size];
    for (int i = 1; i < size; i++) {
        pre_sum[i] = pre_sum[i - 1] + arr[i];
    }

    stack<vector<int> *> s;
    int *left_bounds = new int[size], *right_bounds = new int[size];
    int global_max = INT_MIN;

    for (int i = 0; i < size; i++) {
        /// 找到 i 左右两边离它最近的、大于它的元素的位置
        if (s.empty()) {
            /// 注意，仍然要主动使用 new，否则插入的 vec 是空的！
            auto *v = new vector<int>;
            v->push_back(i);
            s.push(v);
        } else {
            bool non_push = false;
            while (!s.empty()) {
                if (arr[i] < arr[(*s.top())[0]]) {
                    /// 栈顶元素更大，则栈顶弹出，设置栈顶 vec 内全部元素的左边界和右边界
                    vector<int> *cur = s.top();
                    s.pop();
                    for (auto &idx: *cur) {
                        if (!s.empty()) {
                            /// 这里很关键，是最近的元素的索引，因此是 vec 中的最后一个
                            left_bounds[idx] = (*s.top())[s.top()->size() - 1];
                        } else {
                            left_bounds[idx] = -1;
                        }
                        right_bounds[idx] = i;

                        /// 每设置完一个就计算一下（直接利用前缀和计算子数组元素和）
                        int cur_res = left_bounds[idx] == -1 ? pre_sum[right_bounds[idx] - 1] * arr[idx] :
                                      (pre_sum[right_bounds[idx] - 1] - pre_sum[left_bounds[idx]]) * arr[idx];
                        global_max = max(global_max, cur_res);
                    }
                    non_push = true;
                } else if (arr[i] > arr[(*s.top())[0]]) {
                    /// 栈顶元素小，i 直接入栈
                    auto *v = new vector<int>;
                    v->push_back(i);
                    s.push(v);
                    break;
                } else {
                    /// 栈顶元素和 i 相等，i 链接到栈顶元素最后
                    vector<int> *cur = s.top();
                    cur->push_back(i);
                    break;
                }
            }
            if (s.empty() && non_push) {
                auto *v = new vector<int>;
                v->push_back(i);
                s.push(v);
            }
        }
    }
    while (!s.empty()) {
        vector<int> *cur = s.top();
        s.pop();
        for (auto &idx: *cur) {
            if (!s.empty()) {
                /// 这里很关键，是最近的元素的索引，因此是 vec 中的最后一个
                left_bounds[idx] = (*s.top())[s.top()->size() - 1];
            } else {
                left_bounds[idx] = -1;
            }
            /// 这里很关键，右边界若不存在，应当设为 size
            right_bounds[idx] = size;

            /// 每设置完一个就计算一下（直接利用前缀和计算子数组元素和）
            int cur_res = left_bounds[idx] == -1 ? pre_sum[right_bounds[idx] - 1] * arr[idx] :
                          (pre_sum[right_bounds[idx] - 1] - pre_sum[left_bounds[idx]]) * arr[idx];
            global_max = max(global_max, cur_res);
        }
    }
//    for (int i = 0; i < size; i++) {
//        cout << "(" << left_bounds[i] << ", " << right_bounds[i] << ")" << endl;
//    }
    return global_max;
}

int main() {
    int arr[] = {5, 4, 3, 6, 1, 2, 0};
    int size = 7;
    int *left_res = new int[size], *right_res = new int[size];
    get_neighbor_max(arr, size, left_res, right_res);
    for (int i = 0; i < size; i++) {
        cout << "(" << left_res[i] << ", " << right_res[i] << ")\n";
    }
    cout << endl;

    int *left_res2 = new int[size], *right_res2 = new int[size];
    use_mono_stack(arr, size, left_res2, right_res2);
    for (int i = 0; i < size; i++) {
        cout << "(" << left_res2[i] << ", " << right_res2[i] << ")\n";
    }
    cout << endl;

    int arr3[] = {5, 3, 3, 4, 5, 3, 7, 6, 9};
    int size3 = 9;
    int *left_res3 = new int[size3], *right_res3 = new int[size3];
    use_mono_stack2(arr3, size3, left_res3, right_res3);
    for (int i = 0; i < size3; i++) {
        cout << "(" << left_res3[i] << ", " << right_res3[i] << ")\n";
    }
    cout << endl;

    int arr4[] = {5, 3, 2, 1, 6, 6, 7, 4};
    int size4 = 8;
    cout << get_target_val(arr4, size4) << endl;

    return 0;
}
