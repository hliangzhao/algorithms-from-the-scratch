//
// Created by hliangzhao on 13/1/23.
//
#include <iostream>
#include <queue>

using namespace std;

/**
 * 需要掌握的关于堆的内容：
 * 1、堆是用数值实现的完全二叉树结构，可分为大根堆和小根堆；
 * 2、掌握堆的 heap insert 和 heapify 操作；
 * 3、掌握堆结构的增大和减小操作'
 * 4、优先队列，在内部即采用堆来实现。
 * */

void swap(int *, int, int);

void heap_insert(int *, int);

void heapify(int *, int, int);

/**
 * 题目一：已知一个几乎有序的数组，这里几乎有序是指：如果把数组排好序的化，每个元素移动的距离不超过 k（k << size）。
 * 选择一个合适的方法对这个数组进行排序。
 *
 * 选择前 k+1 个元素构造小根堆。则全局最小必然是该小根堆的根。移除它，将数组下一个元素放到根的位置并调整，使其仍然为小根堆。
 * 此时次最小的元素必然在根。反复进行，直到排列完毕。该方法复杂度为 O(n*log(k))。
 * 因为额外使用了一个数组，因此额外空间复杂度为 O(n)。
 * */
int *solve1(int *arr, int size, int k) {
    if (arr == nullptr || size < 2) {
        return arr;
    }
    int *res = new int[size];
    int pos = 0;

    // 用前 k+1 个元素构造小根堆（此处默认 k<size）
    int heap_size = k + 1, cur = heap_size;
    for (int i = 0; i < heap_size; i++) {
        heap_insert(arr, i);
    }
    res[pos++] = arr[0];
    // 在抵达数组尾部之前，heap_size 是不会变小的
    while (cur < size) {
        swap(arr, 0, cur++);
        heapify(arr, 0, heap_size);
        res[pos++] = arr[0];
    }
    // 堆开始减小。
    // 若条件是 heap_size > 0，则 res[size] 会被赋值，操作非法区域
    while (heap_size > 1) {
        swap(arr, 0, --heap_size);
        heapify(arr, 0, heap_size);
        res[pos++] = arr[0];
    }
    return res;
}

/**
 * 题目一方法二：直接借助优先队列来实现。
 * 同样地，额外的空间复杂度仍然为 O(n)。
 * */
void solve2(int *arr, int size, int k) {
    if (arr == nullptr || size < 2) {
        return;
    }
    /// 注意，默认是大顶堆！
//    priority_queue<int> heap;
    /// 使其是小顶堆，需要使用 greater 比较函数（greater \to 后面的比前面的大 \to 升序排列 \to 小顶堆）！
    priority_queue<int, vector<int>, greater<int>> heap;
    int idx = 0;
    // 用前 k+1 个元素构造小根堆
    for (; idx < min(size, k); idx++) {
        heap.push(arr[idx]);
    }
    int i = 0;
    for (; idx < size; i++, idx++) {
        // 添加一个元素，内部自动 heapify
        heap.push(arr[idx]);
        // 此时队首是 heap 中的最小值，取出
        arr[i] = heap.top();
        heap.pop();
    }
    // 堆开始减小，反复弹出队首即可
    while (!heap.empty()) {
        arr[i++] = heap.top();
        heap.pop();
    }
}

/**
 * 优先队列采用成倍扩容。因此扩容次数为 O(log n)。
 * */
void test_pq() {
    /// 内部以大根堆的方式组织
    priority_queue<int> q1;
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    for (int i = 0; i < 8; i++) {
        q1.push(arr1[i]);
    }
    while (!q1.empty()) {
        cout << q1.top() << " ";
        // 每一次 pop 元素之后，内部都会重新被组织成大根堆
        q1.pop();
    }
    cout << endl;

    /// 内部以小根堆的方式组织
    priority_queue<int, vector<int>, greater<int>> q2;
    int arr2[] = {8, 7, 6, 5, 4, 3, 2, 1};
    for (int i = 0; i < 8; i++) {
        q2.push(arr2[i]);
    }
    while (!q2.empty()) {
        cout << q2.top() << " ";
        // 每一次 pop 元素之后，内部都会重新被组织成小根堆
        q2.pop();
    }
    cout << endl;
}

/**
 * 将 arr[idx] 插入到小根堆的合适位置上。
 * */
void heap_insert(int *arr, int idx) {
    while (arr[idx] < arr[(idx - 1) / 2]) {
        swap(arr, idx, (idx - 1) / 2);
        idx = (idx - 1) / 2;
    }
}

/**
 * 将 arr[idx] 自顶向下调整到小根堆合适的位置上。
 * */
void heapify(int *arr, int idx, int heap_size) {
    int left = 2 * idx + 1;
    while (left < heap_size) {
        int smallest = (left + 1 < heap_size && arr[left + 1] < arr[left]) ? left + 1 : left;
        smallest = arr[smallest] < arr[idx] ? smallest : idx;
        if (smallest == idx) {
            break;
        }
        swap(arr, smallest, idx);
        idx = smallest;
        left = 2 * idx + 1;
    }
}

void swap(int *arr, int i, int j) {
    /// 不要忘了判断 i \neq j！否则会出错！
    if (i == j) {
        return;
    }
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
}

int main() {
    int arr1[] = {0, -1, 3, 1, 2, 4, 6, 5, 8, 9, 7};
    int *res = solve1(arr1, 11, 4);
    for (int i = 0; i < 11; i++) {
        cout << res[i] << " ";
    }
    cout << endl;

    int arr2[] = {0, -1, 3, 1, 2, 4, 6, 5, 8, 9, 7};
    solve2(arr2, 11, 4);
    for (int v : arr2) {
        cout << v << " ";
    }
    cout << endl;

    test_pq();

    return 0;
}
