//
// Created by hliangzhao on 11/1/23.
//
#include <iostream>

using namespace std;

/**
 * 使用主方法估计递归算法的复杂度：T(N) = a*T(N/b) + O(N^d)
 * 若 log_b a < d: 结果为 O(N^d)
 * 若 log_b a > d: 结果为 O(N^{log_b a})
 * 若 log_b a == d: 结果为 O(N^d * log N)
 * */

/**
 * 使用递归查找数组 arr[l...r] 中的最大值。
 * 这个过程是一个二叉树的后序遍历，结合入栈和出栈。
 * T(N) = 2 * T(N/2) + O(1)，进一步地，T(N) = 4 * T(N/4) = ... = 2^(log N) * T(1) = O(N)
 * 这和直接遍历的复杂度相同
 * */
int get_max_val(const int *arr, int l, int r) {
    if (l == r) {
        return arr[l];
    }
    int mid = l + ((r - l) >> 1);
    // 如果数组长度很大，下面这种写法可能会溢出！
    // int mid = (l + r) / 2;
    int left_max = get_max_val(arr, l, mid);
    int right_max = get_max_val(arr, mid + 1, r);
    return max(left_max, right_max);
}

void proc(int *, int, int);

void merge(int *, int, int, int);

/**
 * 归并排序：递归调用 proc 使左右半边各自有序，然后通过 merge 操作使得结果有序。
 * T(N) = 2 T(N/2) + O(N)
 * 带入主公式，得到归并排序的复杂度为 O(n*log(n))。
 * 优化了比较行为，但是额外空间复杂度为 O(n)
 * */
void merge_sort(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    proc(arr, 0, size - 1);
}

void proc(int *arr, int l, int r) {
    if (l == r) {
        return;
    }
    int mid = l + ((r - l) >> 1);
    proc(arr, l, mid);
    proc(arr, mid + 1, r);
    merge(arr, l, mid, r);
}

void merge(int *arr, int l, int m, int r) {
    int *tmp = new int[r - l + 1];
    int i = 0;
    int p1 = l, p2 = m + 1;
    while (p1 <= m && p2 <= r) {
        tmp[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
    }
    // add the left elements to the end of tmp
    while (p1 <= m) {
        tmp[i++] = arr[p1++];
    }
    while (p2 <= r) {
        tmp[i++] = arr[p2++];
    }
    for (i = 0; i < r - l + 1; i++) {
        arr[l + i] = tmp[i];
    }
}

int main() {
    int arr[10] = {1, 3, 4, -1, -100, 98, 67, 234, 10, 9};
    cout << get_max_val(arr, 0, 7);
    merge_sort(arr, 10);
    for (int i: arr) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
