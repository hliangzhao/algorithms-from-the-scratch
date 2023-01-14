//
// Created by hliangzhao on 11/1/23.
//
#include <iostream>

using namespace std;

/*
 * 在有序数组中使用二分查找找到目标数字：遍历方法
 * */
void basic_find_num(const int *arr, int size, int target) {
    if (arr == nullptr) {
        return;
    }
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (target == arr[i]) {
            cout << i << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Not found" << endl;
    }
}

/**
 * 二分查找：在有序数组中使用二分查找找到目标数字。
 * 复杂度：O(log(n))
 * */
void binary_search(const int *arr, int size, int target) {
    if (arr == nullptr) {
        return;
    }
    bool found = false;
    int begin = 0; int end = size - 1;
    while (begin < end) {
        int medium = (begin + end) / 2;
        if (arr[medium] > target) {
            end = medium;
        } else if (arr[medium] < target) {
            begin = medium;
        } else {
            cout << medium << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Not found" << endl;
    }
}

/**
 * 使用二分查找在给定有序数组中查找元素值 >=target 的最小位置。
 * */
void binary_search_geq(const int *arr, int size, int target) {
    if (arr == nullptr) {
        ::exit(1);
    }
    int begin = 0, end = size - 1;
    while (true) {
        if (begin == end || begin + 1 == end) {
            break;
        }
        int medium = (begin + end) / 2;
        if (arr[medium] >= target) {
            end = medium;
        } else {
            begin = medium;
        }
    }
    if (arr[begin] >= target) {
        cout << begin << endl;
    } else {
        cout << end << endl;
    }
}

/**
 * 使用二分查找在给定有序数组中查找元素值 <=target 的最大位置。
 * */
void binary_search_leq(const int *arr, int size, int target) {
    if (arr == nullptr) {
        ::exit(1);
    }
    int begin = 0, end = size - 1;
    while (true) {
        if (begin == end || begin + 1 == end) {
            break;
        }
        int medium = (begin + end) / 2;
        if (arr[medium] <= target) {
            begin = medium;
        } else {
            end = medium;
        }
    }
    if (arr[end] <= target) {
        cout << end << endl;
    } else {
        cout << begin << endl;
    }
}


/// 并非只有有序数组才能采用二分的思路。下面给出一些例子。

/**
 * 无序数组上的二分查找：给定一个无序数组，该数组任意两个相邻数不相等。要求在小于 O(n) 的时间内找到一个局部最小值。
 * 任意一个元素，若它比左右两边相邻的数都小，则它是一个局部最小。
 * 本题难点在于 "罗尔定理"。这意味着，使用二分查找时，只要端点不满足，则两个子段中必然有一个子段内存在局部最小。
 * 要判断在哪个子段继续二分查找。
 * */
void find_local_min(const int *arr, int size) {
    if (arr == nullptr) {
        ::exit(1);
    }
    int begin = 0, end = size - 1;
    while (true) {
        if (arr[begin] < arr[begin + 1]) {
            cout << begin << endl;
            return;
        }
        if (arr[end] < arr[end - 1]) {
            cout << end << endl;
            return;
        }
        int medium = (begin + end) / 2;
        if (arr[medium] < arr[medium - 1] && arr[medium] < arr[medium + 1]) {
            cout << medium << endl;
            return;
        }
        if (arr[medium - 1] < arr[medium]) {
            // begin \to begin + 1: down
            // medium - 1 \to medium: up
            // end - 1 \to end: up
            // left part
            end = medium;
        } else {
            // begin \to begin + 1: down
            // medium - 1 \to medium: down
            // end - 1 \to end: up
            // right part
            begin = medium - 1;
        }
    }
}

int main() {
    int arr1[8] = {1, 2, 4, 6, 7, 9, 10, 23};
    basic_find_num(arr1, 8, 2);
    binary_search(arr1, 8, 2);

    int arr2[21] = {1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4,
                    4, 4, 4, 4, 4, 4, 5, 5, 5, 5};
    binary_search_geq(arr2, 21, 4);
    binary_search_leq(arr2, 21, 4);

    int arr3[] = {9, 6, 3, 2, 1, -4, 5, 6, 7};
    find_local_min(arr3, 9);
}