//
// Created by hliangzhao on 12/1/23.
//
#include <iostream>

using namespace std;

/**
 * 简单荷兰国旗问题：给定一个数组和一个数 num，要求把不大于 num 的数放在数组左边，大于 num 的数放在数组右边。
 * 要求时间复杂度 O(n)，额外空间复杂度 O(1)。
 * */

/*
 * 方法一：分段、合并。
 * 合并方式为 arr[l...p1-1] + arr[m+1...p2-1] + arr[p1...m] + arr[p2...r]
 * 然而，此方法的空间复杂度为 O(n)！因为用到了临时数组。
 * */

void proc(int *, int, int, int);

void merge(int *, int, int, int, int);

void solve1(int *arr, int size, int target) {
    if (arr == nullptr || size < 2) {
        return;
    }
    proc(arr, 0, size - 1, k);
}

void proc(int *arr, int l, int r, int target) {
    if (l == r) {
        return;
    }
    int m = l + ((r - l) >> 1);
    proc(arr, l, m, target);
    proc(arr, m + 1, r, target);
    merge(arr, l, m, r, target);
}

void merge(int *arr, int l, int m, int r, int target) {
    int *tmp = new int[r - l + 1];
    int i = 0, p1 = l, p2 = m + 1;
    while (p1 <= m) {
        if (arr[p1] <= target) {
            tmp[i++] = arr[p1++];
        } else {
            break;
        }
    }
    while (p2 <= r) {
        if (arr[p2] <= target) {
            tmp[i++] = arr[p2++];
        } else {
            break;
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

/**
 * 方法二：使用双指针
 * */
void solve2(int *arr, int size, int target) {
    // "i" is the search pointer
    // "j" is the right bound of the "<=" region
    // 实际上，[j+1...i]是已经明确了的 the ">" region
    int i = 0, j = -1;
    while (i < size) {
        if (arr[i] <= target) {
            // swap arr[i] and arr[j+1]
            int tmp = arr[i];
            arr[i] = arr[j + 1];
            arr[j + 1] = tmp;
            // extend the right bound
            j++;
        }
        i++;
    }
}


/**
 * 荷兰国旗问题：给定一个数组和一个数 num，要求把小于 num 的数放在数组左边，等于 num 的数放在数组中间，
 * 大于 num 的数放在数组右边。
 * 要求时间复杂度 O(n)，额外空间复杂度 O(1)。
 * */

/*
 * 方法一：使用三指针，不同情况进行分类。某些情况下，不是仅仅交换这么简单，需要移动数据。
 * |_<_|___=___|___>___|____________|
 *    i       j         k undecided
 * */
void flag(int *arr, int size, int target) {
    int i = -1, j = -1, k = 0;
    while (k < size) {
        if (arr[k] < target) {
            // insert arr[k] to the right of arr[i]
            // move arr[i+1...k-1] to arr[i+2...k]
            int tmp = arr[k];
            for (int p = k - 1; p >= i + 1; p--) {
                arr[p + 1] = arr[p];
            }
            arr[i + 1] = tmp;
            // extend the right bound of "<" and "=" regions
            i++;
            j++;
        } else if (arr[k] == target) {
            // swap arr[k] and arr[j+1]
            int tmp = arr[k];
            arr[k] = arr[j + 1];
            arr[j + 1] = tmp;
            j++;
        }
        k++;
    }
}

/**
 * 方法二：三指针的另一种使用方式。此时无需移动数组的子部分，完全通过交换即可。
 * |_<_|_____________________|___>___|
 *    i   k (to be decided)   j
 * "i" is "<" region's right bound
 * "j" is ">" region's left bound
 * */
void flag2(int *arr, int size, int target) {
    int i = -1, j = size, k = 0;
    while (k < j) {
        if (arr[k] < target) {
            // swap arr[k] and arr[i+1]
            int tmp = arr[k];
            arr[k] = arr[i+1];
            arr[i+1] = tmp;
            // update right bound of "<"
            i++;
        } else if (arr[k] > target) {
            // swap arr[k] and arr[j-1]
            int tmp = arr[k];
            arr[k] = arr[j-1];
            arr[j-1] = tmp;
            // update left bound of ">"
            j--;
            /// 本 if 下面执行 k++ 会出问题！因为这个元素所以刚交换过来的，还没有看过，因此此时 k 不应该自增
            continue;
        }
        k++;
    }
}

int main() {
    int arr1_1[] = {6, 3, 2, -1, 5, 1, 8, -5};
    int target = 3;
    solve(arr1_1, 8, target);
    for (int i: arr1_1) {
        cout << i << " ";
    }
    cout << endl;
    int arr1_2[] = {6, 3, 2, -1, 5, 1, 8, -5};
    solve2(arr1_2, 8, target);
    for (int i: arr1_2) {
        cout << i << " ";
    }
    cout << endl;

    int arr2_1[] = {4, 3, 4, 6, -1, 45, 9, 90, 10, 2, 2, 3, 3};
    int target2 = 3;
    flag(arr2_1, 13, target2);
    for (int i: arr2_1) {
        cout << i << " ";
    }
    cout << endl;
    int arr2_2[] = {4, 3, 4, 6, -1, 45, 9, 90, 10, 2, 2, 3, 3};
    flag2(arr2_2, 13, target2);
    for (int i: arr2_2) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}