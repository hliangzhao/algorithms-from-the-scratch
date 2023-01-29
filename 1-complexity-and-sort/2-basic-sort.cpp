//
// Created by hliangzhao on 29/1/23.
//
#include <iostream>

using namespace std;

/**
 * 基本排序方法：冒泡排序、选择排序以及插入排序。
 * */

void swap(int *arr, int i, int j) {
    /// 注意，i \neq j，否则 arr[i] 会变成 0
    if (i == j) {
        return;
    }
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
}

/**
 * 选择排序：从 [i, n) 中找到最小的元素，放在第 i 个位置上。i 从 0 开始遍历。
 * 复杂度：O(n^2)
 * */
void select_sort(int *arr, int size) {
    // do not forget the bound condition
    if (arr == nullptr || size < 2) {
        return;
    }
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < size; j++) {
            min_idx = arr[j] < arr[min_idx] ? j : min_idx;
        }
        swap(arr, i, min_idx);
    }
}

/**
 * 冒泡排序：第 i 个位置的元素和第 i+1 个位置的元素比较，大的放后面 —— 由此最大的元素像气泡那样一到了队尾。
 * 复杂度：O(n^2)
 * 可以发现，冒泡排序和选择排序具有相同的解构，前者控制探索范围的 right bound，后者控制 left bound。
 * */
void bubble_sort(int *arr, int size) {
    // do not forget the bound condition
    if (arr == nullptr || size < 2) {
        return;
    }
    for (int j = size - 1; j > 0; j--) {
        // j is the right bound
        // find the largest value from [0, j] and move it to the position j
        for (int i = 0; i < j; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr, i, i + 1);
            }
        }
    }
}

/**
 * 插入排序：把 arr[i] 插入到 [0,i] 范围内比它大的数前面，i 从 0 开始遍历。
 * */
void insert_sort(int *arr, int size) {
    // do not forget the bound condition
    if (arr == nullptr || size < 2) {
        return;
    }
    for (int i = 1; i < size; i++) {
        // 方法一：比较后移位
//        int j = i - 1;
//        while (j >= 0) {
//            if (arr[i] < arr[j]) {
//                j--;
//            } else {
//                break;
//            }
//        }
//        // 将 arr[i] 插入到 arr[j] 之后
//        // 这里涉及数组的移位操作，复杂度为 O(n)
//        int tmp = arr[i];
//        for (int k = i - 1; k > j; k--) {
//            arr[k + 1] = arr[k];
//        }
//        arr[j + 1] = tmp;

        // 方法二：边比较边移位
        for (int j = i - 1; j >= 0 && arr[j] > arr[j+1]; j--) {
            swap(arr, j, j+1);
        }
    }
}

int main() {
    int arr[8] = {4, 10, 5, 6, 89, 10, -9, -10};
    select_sort(arr, 8);
    int arr2[8] = {4, 10, 5, 6, 89, 10, -9, -10};
    bubble_sort(arr2, 8);
    int arr3[8] = {4, 10, 5, 6, 89, 10, -9, -10};
    insert_sort(arr3, 8);
    for (int i: arr) {
        cout << i << " ";
    }
    for (int i: arr2) {
        cout << i << " ";
    }
    for (int i: arr3) {
        cout << i << " ";
    }

    return 0;
}
