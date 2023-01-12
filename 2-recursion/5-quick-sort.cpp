//
// Created by hliangzhao on 12/1/23.
//
#include <iostream>
#include <random>

using namespace std;

void solve1(int *, int, int, int);

/**
 * 方法一：借助简单荷兰国旗问题的方法二。将区间划分为"<="和">"，每次确定一个元素的位置，然后递归。
 * */
void quick_sort_v1(int *arr, int size) {
    if (arr == nullptr || size < 3) {
        return;
    }
    solve1(arr, size, 0, size - 1);
}

void solve1(int *arr, int size, int l, int r) {
    // TODO: 找到正确的边界条件！
    if (l >= r || r < 0 || l >= size) {
        return;
    }
    // "i" is the exploration pointer
    // "j" is the right bound of the "<=" region
    int i = l, j = i - 1;
    int target = arr[r];
    while (i <= r) {
        if (arr[i] <= target) {
            // swap arr[i] and arr[j+1] and update j
            int tmp = arr[i];
            arr[i] = arr[j + 1];
            arr[j + 1] = tmp;
            j++;
        }
        i++;
    }
    // for debugging
//    for (int k = l; k <= r; k++) {
//        cout << arr[k] << " ";
//    }
//    cout << endl;

    // now arr[j] is sorted, its position will never be changed.
    // now we solve1 the left part and the right part in the same way
    solve1(arr, size, l, j - 1);
    solve1(arr, size, j + 1, r);
}

/**
 * 方法二：借助荷兰国旗问题的方法二。将区间划分为"<"、"="和">"，每次确定一组元素（中间区间）的位置，然后递归。
 * 外圈是递归，平均是 O (log n，内圈是国旗问题，复杂度为 O(n)，因此平均复杂度为 O (n log n)。
 * 最坏是 O (n^2)。
 * */

void solve2(int *, int, int, int);

void quick_sort_v2(int *arr, int size) {
    if (arr == nullptr || size < 3) {
        return;
    }
    solve2(arr, size, 0, size - 1);
}

void solve2(int *arr, int size, int l, int r) {
    if (l >= size || r < 0 || l >= r) {
        return;
    }
    int i = l - 1, j = r + 1, k = l;
    int target = arr[r];
    while (k < j) {
        if (arr[k] < target) {
            // swap arr[k] and arr[i+1]
            int tmp = arr[k];
            arr[k] = arr[i + 1];
            arr[i + 1] = tmp;
            // update right bound of "<"
            i++;
        } else if (arr[k] > target) {
            // swap arr[k] and arr[j-1]
            int tmp = arr[k];
            arr[k] = arr[j - 1];
            arr[j - 1] = tmp;
            // update left bound of ">"
            j--;
            continue;
        }
        k++;
    }
    solve2(arr, size, l, i);
    solve2(arr, size, j, r);
}

/**
 * 方法三：标准写法 —— 等概率随机选择一个位置，而不是最后一个位置。
 * */

void solve3(int *, int, int);

void swap(int *, int, int);

int *partition(int *, int, int);

void quick_sort_v3(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    solve3(arr, 0, size - 1);
}

void solve3(int *arr, int l, int r) {
    ::random_device rd;
    ::mt19937 mt(rd());
    // 在 [l...r] 内等概率随机选择一个位置，将其与 r-th 元素交换
    uniform_int_distribution<int> dist(l, r);
    if (l < r) {
        swap(arr, dist(mt), r);
        // partition 就是在求解荷兰国旗问题，返回的是等于 target 的区间的左右边界
        int *p = partition(arr, l, r);
        solve3(arr, l, p[0] - 1);
        solve3(arr, p[1] + 1, r);
    }
}

int *partition(int *arr, int l, int r) {
    int less = l - 1;
    int more = r;
    while (l < more) {
        if (arr[l] < arr[r]) {
            swap(arr, ++less, l++);
        } else if (arr[l] > arr[r]) {
            swap(arr, --more, l);
        } else {
            l++;
        }
    }
    swap(arr, more, r);
    return new int[2] {less + 1, more};
}

void swap(int *arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

int main() {
    int arr1[] = {4, 3, 5, 6, 5, 0, 1, 7, 8, 5, 1, 3, 8};
    int *arr2 = new int[13];
    int *arr3 = new int[13];
    ::memcpy(arr2, arr1, sizeof(int) * 13);
    ::memcpy(arr3, arr1, sizeof(int) * 13);

    quick_sort_v1(arr1, 13);
    for (int i: arr1) {
        cout << i << " ";
    }
    cout << endl;

    quick_sort_v2(arr2, 13);
    for (int i = 0; i < 13; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    quick_sort_v3(arr3, 13);
    for (int i = 0; i < 13; i++) {
        cout << arr3[i] << " ";
    }
    cout << endl;

    return 0;
}