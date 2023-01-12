//
// Created by hliangzhao on 12/1/23.
//
#include <iostream>

using namespace std;

/*
 * 小和问题：在一个数组中，每一个数左边比当前数小的数累加起来，叫做这个数组的小和。给定一个数组，求它的小和。
 * O (n^2)
 * */
int basic_little_sum(const int *arr, int size) {
    if (arr == nullptr) {
        ::exit(1);
    }
    int res = 0;
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                res += arr[j];
            }
        }
    }
    return res;
}

/**
 * 将小和问题转换为：对于每个元素，右边有多少个数比该元素大？
 * [1, 3, 4, 2, 5]:
 * 1：右边有 4 个数比 1 大：1 * 4 = 4；
 * 3：右边有 2 个数比 3 大：3 * 2 = 6；
 * 4：右边有 1 个数比 4 大：4 * 1 = 4；
 * 2：右边有 1 个数比 2 大：2 * 1 = 2；
 * 4 + 6 + 4 + 2 = 16。
 * 小和问题可以通过改造归并排序得到。
 */

void proc1(int *, const int *, int, int);

void merge1(int *, const int *, int, int, int);

/*
 * 引入了 cnt 数组，因此额外的空间复杂度为 O(n)
 * */
int fast_little_sum(const int *arr, int size) {
    if (arr == nullptr || size < 2) {
        ::exit(1);
    }
    int *cnt = new int[size];
    ::memset(cnt, 0, sizeof(int) * size);

    int res = 0;
    proc1(cnt, arr, 0, size - 1);
    for (int i = 0; i < size; i++) {
        res += arr[i] * cnt[i];
    }
    return res;
}

void proc1(int *cnt, const int *arr, int l, int r) {
    if (l == r) {
        return;
    }
    int mid = l + ((r - l) >> 1);
    proc1(cnt, arr, l, mid);
    proc1(cnt, arr, mid + 1, r);
    merge1(cnt, arr, l, mid, r);
}

void merge1(int *cnt, const int *arr, int l, int m, int r) {
    // 这里没有排序，因此不得不双重遍历！
    for (int p1 = l; p1 <= m; p1++) {
        for (int p2 = m + 1; p2 <= r; p2++) {
            if (arr[p2] > arr[p1]) {
                cnt[p1]++;
            }
        }
    }
}

/**
 * 下面给出左老师的实现（有排序）
 * */

int proc2(int *, int, int);

int merge2(int *, int, int, int);

int fast_little_sum2(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return 0;
    }
    return proc2(arr, 0, size - 1);
}

int proc2(int *arr, int l, int r) {
    if (l == r) {
        return 0;
    }
    int m = l + ((r - l) >> 1);
    return proc2(arr, l, m) + proc2(arr, m + 1, r) + merge2(arr, l, m, r);
}

int merge2(int *arr, int l, int m, int r) {
    int *tmp = new int[r - l + 1];
    int i = 0, p1 = l, p2 = m + 1;
    int res = 0;
    while (p1 <= m && p2 <= r) {
        // p2+1 ~ r 无需遍历了。因为有序保证这些位置的元素必然大于 arr[p1]
        res += arr[p1] < arr[p2]? (r - p2 + 1) * arr[p1]: 0;
        tmp[i++] = arr[p1] < arr[p2]? arr[p1++]: arr[p2++];
    }
    while (p1 <= m) {
        tmp[i++] = arr[p1++];
    }
    while (p2 <= r) {
        tmp[i++] = arr[p2++];
    }
    for (i = 0; i < r - l + 1; i++) {
        arr[l + i] = tmp[i];
    }
    return res;
}

int main() {
    int arr2[5] = {3, 2, 4, 5, 0};
    cout << basic_little_sum(arr2, 5) << endl;
    cout << fast_little_sum(arr2, 5) << endl;
    cout << fast_little_sum2(arr2, 5) << endl;

    return 0;
}
