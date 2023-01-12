//
// Created by hliangzhao on 12/1/23.
//
#include <iostream>

using namespace std;

/*
 * 逆序对问题：在一个数组中，若左边的数比右边的数大，则这两个数构成一个逆序对。给定一个数组，打印所有逆序对。
 * */
void reverse_pair(const int *arr, int size) {
    if (arr == nullptr || size == 1) {
        ::exit(1);
    }
    for (int j = 1; j < size; j++) {
        for (int i = 0; i < j; i++) {
            if (arr[i] > arr[j]) {
                cout << "(" << arr[i] << ", " << arr[j] << ")\n";
            }
        }
    }
    cout << endl;
}

/**
 * 逆序对问题等价于「求任意一个数右边有多少个数比当前数小」。小和问题是「右边有多少个数比当前数大」。
 * 因此，直接改造小和问题代码就可以得到逆序对问题的代码实现。
 * */

void proc(int *, int, int);

void merge(int *, int, int, int);

void fast_reverse_pair(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    proc(arr, 0, size - 1);
}

void proc(int *arr, int l, int r) {
    if (l == r) {
        return;
    }
    int m = l + ((r - l) >> 1);
    proc(arr, l, m);
    proc(arr, m + 1, r);
    merge(arr, l, m, r);
}

void merge(int *arr, int l, int m, int r) {
    int *tmp = new int[r - l + 1];
    int i = 0, p1 = l, p2 = m + 1;
    while (p1 <= m && p2 <= r) {
        // 这里将"小于"改为"大于"，因为数组要逆序排列
        if (arr[p1] > arr[p2]) {
            for (int k = p2; k<= r; k++) {
                cout << "(" << arr[p1] << ", " << arr[k] << ")" << endl;
            }
            tmp[i++] = arr[p1++];
        } else {
            tmp[i++] = arr[p2++];
        }
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
}

int main() {
    int arr2[5] = {3, 2, 4, 5, 0};

    reverse_pair(arr2, 5);
    fast_reverse_pair(arr2, 5);

    return 0;
}
