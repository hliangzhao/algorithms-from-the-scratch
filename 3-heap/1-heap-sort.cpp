//
// Created by hliangzhao on 12/1/23.
//
#include <iostream>

using namespace std;

/**
 * 堆是一个完全二叉树。
 * 大根堆：每个子树的头节点数值是本子树中最大的。同理定义小根堆。
 *
 * 往大根堆中添加一个数，调整的复杂度为 O(log n)；
 * 移除最大值并使得结果仍然为大根堆，调整的复杂度也为 O(log n)。
 * */

void swap(int *, int, int);

/**
 * 在大根堆中将新插入的节点 arr[idx] 调整到合适的位置上。
 * */
void heap_insert(int *arr, int idx) {
    // 节点反复和其祖先节点比较，一路向上，直到祖先节点小于本节点
    // 注意，当 idx = 0 时，(idx - 1) / 2 = 0（向零取整），一个数不比自身大，因此若 idx 为零则循环终止。
    while (arr[idx] > arr[(idx - 1) / 2]) {
        swap(arr, idx, (idx - 1) / 2);
        idx = (idx - 1) / 2;
    }
}

/**
 * 给定一个大根堆，剔除最大节点，使得剩余节点仍然是一个大根堆。
 * 思路：去掉大根堆的根节点后，先把最后一个位置的元素放到根节点，
 * 然后在自顶向下调整树（从左右孩子中找到大的那个，如果它大于根，交换，直接无法交换为止），使得其仍然是一个大根堆。
 * 下面给出自将 arr[idx]（作为根）自顶向下被调整到树的合适位置过程。
 * */
void heapify(int *arr, int idx, int heap_size) {
    int left = idx * 2 + 1;
    while (left < heap_size) {
        // 找到左右孩子中大的那个
        int largest = (left + 1 < heap_size && arr[left + 1] > arr[left]) ? left + 1 : left;
        // 如果孩子比自己大，交换
        largest = arr[largest] > arr[idx] ? largest : idx;
        // 截止
        if (largest == idx) {
            break;
        }
        swap(arr, largest, idx);
        // 更新自己的 index
        idx = largest;
        left = idx * 2 + 1;
    }
}

/**
 * 堆排序：先根据给定序列构造大根堆，然后反复将根和最后一个节点交换（移除最大值），从而拿到节点元素值的降序排列。
 * 复杂度为 O(n log n)，额外空间复杂度为 O(1)。因为我们并没有实际上建树 —— 完全通过调整元素 index 来实现。
 * 和快排相比，堆排序的额外空间复杂度做到了极致，只需要 O(1)。
 * */

void get_big_root_heap_v1(int *, int);

void get_big_root_heap_v2(int *, int);

void heap_sort(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    // construct the big-root heap
//    get_big_root_heap_v1(arr, size); // O(n log n)
    get_big_root_heap_v2(arr, size); // o(n)
    int heap_size = size;
    // remove the largest value (move it to the end) until the heap is empty
    // then we get a sorted sequence in the ascending order of value
    swap(arr, 0, --heap_size);
    while (heap_size > 0) { // O(n)
        heapify(arr, 0, heap_size); // O(log n)
        swap(arr, 0, --heap_size); // O(1)
    }
}

/**
 * 该建立大根堆方法的复杂度为 O(n log n)
 * */
void get_big_root_heap_v1(int *arr, int size) {
    for (int i = 0; i < size; i++) { // O(n)
        heap_insert(arr, i); // O(log n)
    }
}

/**
 * 该建立大根堆方法的复杂度为 O(n)
 * T(n) = N/2*1 + N/4*2 + N/8*3 +...
 * 使用错位相减，可得 T(n) = O(n)
 * */
void get_big_root_heap_v2(int *arr, int size) {
    for (int i = size - 1; i >= 0; i--) {
        heapify(arr, i, size);
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
    int arr1[] = {1, 2, -2, 34, -9, 100, 87, 34, 2};
    heap_sort(arr1, 9);
    for (int v: arr1) {
        cout << v << " ";
    }
    cout << endl;
    return 0;
}

