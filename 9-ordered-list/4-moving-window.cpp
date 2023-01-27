//
// Created by hliangzhao on 27/1/23.
//
#include <iostream>
#include <deque>

using namespace std;

/**
 * 给定一个整型数组 arr 和一个大小为 w 的窗口。
 * 该窗口从数组的最左边滑动到最右边，每次滑动一个位置，将每个窗口内的最大值组织称数组并返回。
 * */
int *moving_window(const int *arr, int size, int w) {
    if (w > size) {
        return nullptr;
    }
    int *res = new int[size - w + 1];
    for (int j = w - 1; j < size; j++) {
        int max_of_j = INT_MIN;
        for (int i = j + 1 - w; i <= j; i++) {
            if (arr[i] > max_of_j) {
                max_of_j = arr[i];
            }
        }
        res[j - w + 1] = max_of_j;
    }
    return res;
}

/**
 * 方法二：使用双端队列。队列头部永远存放最大值。
 * 维护左右两个指针 l 和 r。当 r 往右移动时，永远保证队首元素最大
 * （依次比较队尾元素，队尾元素 <= arr[r] 则弹出，因为 arr[r] 作为更靠后的元素，其值还更大（或相等），因此队尾元素再也没有机会成为窗口中的最大值了）。
 * 当 l 往右移动时，若 arr[l] 在队列中（若在，只可能在队首），则弹出；否则什么也不做。
 *
 * 双端队列维护的是如下信息：在 r 固定住时，若 l 向右移动，队首元素将依次成为最大值。
 *
 * 因为每个元素最多进队列一次，出队列一次，因此总代价为 O(n)，这显然好于上面的方法。
 *
 * 注意，双端队列中存放的是元素的 index。
 * */
int *use_deque(const int *arr, int size, int w) {
    if (w > size) {
        return nullptr;
    }
    deque<int> q;
    int *res = new int[size - w + 1];
    int l = -1, r = 0;
    while (r < size) {
        while (!q.empty() && arr[r] >= arr[q.back()]) {
            q.pop_back();
        }
        q.push_back(r);
        if (r - l < w) {
            // l 不右移
        } else {
            // l 右移
            if (l == q.front()) {
                q.pop_front();
            } else {
                // do nothing
            }
            l++;
            res[l] = arr[q.front()];
        }
        r++;
    }
    return res;
}


int main() {
    int arr[] = {4, 3, 5, 4, 3, 3, 6, 9, 7};
    int size = 9, w = 3;
    int *res = moving_window(arr, size, w);
    for (int i = 0; i < size - w + 1; i++) {
        cout << res[i] << " ";
    }
    cout << endl;
    int *res2 = moving_window(arr, size, w);
    for (int i = 0; i < size - w + 1; i++) {
        cout << res2[i] << " ";
    }
    cout << endl;
    return 0;
}