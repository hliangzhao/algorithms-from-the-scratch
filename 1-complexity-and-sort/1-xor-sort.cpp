//
// Created by hliangzhao on 11/1/23.
//
#include <iostream>

using namespace std;

/*
 * 异或运算：是一种不进位相加。
 * 特征：0 ^ n = n; n ^ n = 0; 满足交换律和结合律。异或运算在执行时是以二进制位的方式进行上的。
 * 由此：
 * a \leftarrow a^b;
 * b \leftarrow a^b = (a^b)^b = a^(b^b) = a^0 = a;
 * a \leftarrow a^b = (a^b)^a = b^(a^a) = b^0 = b;
 * */
void swap(int *arr, int i, int j) {
    // 注意，i \neq j，否则 arr[i] 会变成0
    if (i == j) {
        return;
    }
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
}

/*
 * 异或运算题目：给定一个数组，其中有且仅有一个数出现了奇数次，找到这个数。
 * 要求：时间复杂度为 O(n)，额外空间复杂度为 O(1)。
 * */
int find_one_num(const int *arr, int size) {
    if (arr == nullptr) {
        std::exit(1);
    }
    int res = 0;
    for (int i = 0; i < size; i++) {
        res = res ^ (arr[i]);
    }
    return res;
}

/*
 * 异或运算题目：给定一个数组，其中有且仅有两个数出现了奇数次，找到这两个数。
 * 要求：时间复杂度为 O(n)，额外空间复杂度为 O(1)。
 * 下面这个方法时间复杂度是 O(n^2)，不满足要求。
 * */
void find_two_nums(const int *arr, int size) {
    if (arr == nullptr) {
        std::exit(1);
    }
    int res = 0;
    for (int i = 0; i < size; i++) {
        res = res ^ (arr[i]);
    }
    // 至此，res 变为 a^b
    for (int i = 0; i < size; i++) {
        // a^b^a 或者 a^b^b 必然存在在数组中，遍历找到它们即可
        int tmp = res;
        tmp = tmp ^ arr[i];
        int found = false;
        for (int j = 0; j < size; j++) {
            if (tmp == arr[j]) {
                found = true;
                break;
            }
        }
        if (found) {
            cout << tmp << " ";
        }
    }
    cout << endl;
}

/*
 * 异或运算题目：给定一个数组，其中有且仅有两个数出现了奇数次，找到这两个数。
 * 要求：时间复杂度为 O(n)，额外空间复杂度为 O(1)。
 * 下面这个方法时间复杂度是 O(n)，满足要求。
 * */
void find_two_nums_faster(const int *arr, int size) {
    if (arr == nullptr) {
        std::exit(1);
    }
    int a_xor_b = 0;
    for (int i = 0; i < size; i++) {
        a_xor_b = a_xor_b ^ (arr[i]);
    }
    // 至此，a_xor_b 变为 a^b，且 a_xor_b 必然不为 0（否则 a = b，a 出现了偶数次）
    // 所以，a_xor_b 必然有一位是 1。a_xor_b 取反加 1，即得对应的补码。
    // 一个数和它的补码的和必然是 "111...1"，因此，最右的 1 可以通过该数和它的补码的与运算得到，因为对应位置必然都是 1。
    // 最终，right_one 是一个对应位置为 1，其余位置皆为 0 的变量。
    // a 和 b，在这一位上必然是不同的。
    int right_one = a_xor_b & (~a_xor_b + 1);
    int res1 = 0;
    for (int i = 0; i < size; i++) {
        // 只有那些该位不为 1 的数，才会参与 xor，参与运算的数个数必然 2k + 1，这个落单的，不是 a，就是 b
        // 然后我们就找到了 a（或b）。进一步，我们就能找到另一个数。
        if ((arr[i] & right_one) == 0) {
            res1 ^= arr[i];
        }
    }
    int res2 = a_xor_b ^ res1;
    cout << res1 << " " << res2 << endl;
}


/*
 * 选择排序：从 [i, n) 中找到最小的元素，放在第 i 个位置上。i 从 0 开始遍历。
 * O(n^2)
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
        // swap
//                int tmp = arr[i];
//                arr[i] = arr[min_idx];
//                arr[min_idx] = tmp;
        swap(arr, i, min_idx);
    }
}

/*
 * 冒泡排序：第 i 个位置的元素和第 i+1 个位置的元素比较，大的放后面 —— 由此最大的元素像气泡那样一到了队尾。
 * O(n^2)
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
                // swap
//                int tmp = arr[i];
//                arr[i] = arr[i+1];
//                arr[i+1] = tmp;
                swap(arr, i, i + 1);
            }
        }
    }
}

/*
 * 插入排序：把 arr[i] 插入到 [0,i] 范围内比它大的数前面，i 从 0 开始遍历。
 * */
void insert_sort(int *arr, int size) {
    // do not forget the bound condition
    if (arr == nullptr || size < 2) {
        return;
    }
    for (int i = 1; i < size; i++) {
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

        // 也可以边比较边移位
        for (int j = i - 1; j >= 0 && arr[j] > arr[j+1]; j--) {
            swap(arr, j, j+1);
        }
    }
}

int main() {
    int arr[8] = {4, 10, 5, 6, 89, 10, -9, -10};
    select_sort(arr, 8);
//    bubble_sort(arr, 8);
//    insert_sort(arr, 8);
    for (int i: arr) {
        cout << i << " ";
    }
//
//    int arr2[5] = {1, -123, 15, -123, 15};
//    cout << find_one_num(arr2, 5);
//
//    int arr3[12] = {1, -123, 15, -123, 15, 78,
//                    -123, -123, 15, 15, 15, 15};
//    find_two_nums(arr3, 12);
//    find_two_nums_faster(arr3, 12);

    return 0;
}