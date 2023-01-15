//
// Created by hliangzhao on 14/1/23.
//
#include <iostream>
#include <queue>

using namespace std;

/**
 * 到目前为止，所有的排序都属于「基于比较的排序」。
 * 接下来介绍桶排序。
 *
 * 两个典型方法：计数排序和基数排序。
 * 时间复杂度为 O(n)，额外空间复杂度为 O(n)。
 * 应用范围有限，需要根据样本的数据状况找到桶的划分。
 * */


/**
 * 排序算法梳理：
 * 1、排序算法的稳定性
 *    同样值的个体之间，如果不因为排序而改变相对次序，则这个排序是稳定的；否则不稳定。
 *
 *    不稳定的：选择排序、快速排序、堆排序
 *        快排：partition 的时候做交换（不论是否相同，只要小于 target 就换到后面）），引入了不稳定性；
 *        选择：只要比当前小，就已到前面，没有考虑比当前小的元素是否相同；
 *        堆排序：同样地，只要大于当前元素，就交换。
 *    稳定的：冒泡排序、插入排序、归并排序、一切桶排序思想下的排序
 *        冒泡和插入：相等的时候不交换即可；
 *        归并排序：merge 的时候保持先左后右即可（但小和问题不稳定）；
 *        桶排序：先入先出，整体维持了原有顺序。
 *
 *    目前尚未找到时间复杂度为 O (n * log n)，额外空间复杂度为 O(1)，且稳定的排序。
 *
 *    依据使用场景，有些场景必须要使用具有稳定性的排序算法。
 *
 *  2、整体梳理
 *  --------------------------------------------------
 *  算法      时间复杂度       额外空间复杂度     稳定性
 *  --------------------------------------------------
 *  选择排序    O(n^2)             O(1)            无
 *  冒泡排序    O(n^2)             O(1)            有
 *  插入排序    O(n^2)             O(1)            有
 *  归并排序    O(n log(n))        O(n)            有
 *  快速排序    O(n log(n))        O(log(n))       无
 *  堆排序      O(n log(n))        O(1)            无
 *  --------------------------------------------------
 *
 *  3、结论
 *  （1）基于比较的排序，时间复杂度不可能小于 O(n log(n))；
 *  （2）基于比较的排序，时间复杂度为 O(n log(n))，不存在空间复杂度在 O(n) 以下的稳定排序。
 *
 *  4、一些坑
 *  （1）归并排序的额外空间复杂度可以降到 O(1)（内部缓存法），但会丧失稳定性，不如直接使用堆排序；
 *  （2）「原地归并排序」额外空间复杂度可以降到 O(1)，但是复杂度会变成 O(n^2)；
 *  （3）快速排序可以做到稳定性（0-1 stable sort），但是额外空间复杂度升为 O(n)；
 *
 *  5、一道题：给定一个数组，要求奇数放左边，偶数放右边，且不改变数之间的相对次序。要求时 O(n)，额空 O(1)。
 *  类似于荷兰国旗问题和快速排序，可通过双指针来实现。然而不具有稳定性。只能用 0-1 stable sort 来实现，然而这很难。
 *  实际上，只要是 0-1 partition 问题，都做不到稳定性。
 *
 *  6、工程实现时，可结合数据量调整解法。例如，当 partition 的数据量小于 k 时，直接使用插入排序。但是整体上仍然是快排。
 *
 *  7、编译器自带的库函数 sort，当传入的序列是基础对象时，它采用快排，否则擦一用归并，这是在最低复杂度下出于稳定性的考量。
 * */

/*
 * 奇偶放置问题的解法。
 * */
void solve(int *arr, int size) {
    int i = 0, j = -1, k = size;
    while (i < k) {
        if (arr[i] % 2 == 1) {
            int tmp = arr[i];
            arr[i] = arr[j + 1];
            arr[j + 1] = tmp;
            j++;
        } else {
            int tmp = arr[i];
            arr[i] = arr[k - 1];
            arr[k - 1] = tmp;
            k--;
            continue;
        }
        i++;
    }
}

int maxbits(const int *, int);

int get_digit(int, int);

/**
 *基数排序的简单实现。本函数直接模拟桶排序的过程。
 * */
void basic_radix_sort(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    int radix = 10, digit = maxbits(arr, size);
    for (int d = 1; d <= digit; d++) {
        // 每一个桶是一个先进先出的队列
        queue<int> *queues = new queue<int>[radix];
        // 依次进桶
        for (int i = 0; i < size; i++) {
            int j = get_digit(arr[i], d);
            queues[j].push(arr[i]);
        }
        // 依次出桶
        int i = 0;
        while (i < size) {
            for (int j = 0; j < radix; j++)
                while (!queues[j].empty()) {
                    arr[i] = queues[j].front();
                    queues[j].pop();
                    i++;
                }
        }
        delete[]queues;
    }
}

void proc(int *, int, int, int);

/**
 * 基数排序的优化实现。不需要额外创建队列，借助一个前缀和数组直接实现「入桶+出桶」操作。
 * */
void radix_sort(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    proc(arr, 0, size - 1, maxbits(arr, size));
}

void proc(int *arr, int l, int r, int digit) {
    /**
     * 设 arr[] = {013, 021, 011, 052, 062}
     * */
    int radix = 10; // 以 10 为基
    int *bucket = new int[r - l + 1]; // 准备等规模的桶个数

    /// 最多有多少位，就进出几次
    for (int d = 1; d <= digit; d++) {
        /// count[i] 表示当前位（d）是 0 ～ i 的数的个数
//        int *count = new int[radix];
//        ::memset(count, 0, sizeof(int) * radix);
        int *count = new int[radix]{};  // {} 表示全部元素初始化为 0

        /// 首先，count[i] 记录的是当前位是 i 的数的个数，此时 count 是一个词频表（pdf）
        for (int i = l; i <= r; i++) {
            int j = get_digit(arr[i], d);
            count[j]++;
        }
        /**
         * d = 1 时，此时 count[] = {0, 2, 2, 1, 0, ..., 0}
         * */

        /// 然后，通过前后累加（前缀和），使得 count[i] 存放的是当前位 <=i 的数的个数（CDF）
        /// 前缀和给我们提供了一个划分好了的片区，第 d 位相同的，只会出现在这个片区内
        /// 属于每一个片区内的数，显然应当从后往前放，这样才可以不冲突地占满这个片区（因为我们只知道每个片区的右边界）
        /// 本质上，count[i] 存放的是第 i 个片区的右边界！
        //  _____ _________ ___ _________ _____________
        // |  0  |    1    | 2 | ... ... |      9      |
        for (int i = 1; i < radix; i++) {
            count[i] = count[i] + count[i - 1];
        }
        /**
         * d = 1 时，此时 count[] = {0, 2, 4, 5, 5, ..., 5}
         * */

        /// 注意，在桶排序中，对于每一个桶内的元素，是先进先出、后进后出的。
        /// 因为我们每个片区内是从后往前放，相当于先考虑后出的，那为了模拟这个过程，我们自然应该也应当先考虑后进的。
        /// 所以下面的循环从后往前遍历。
        for (int i = r; i >= l; i--) {
            /// 借助 count，bucket 里存放的直接就是按照出桶顺序存放的 —— 相当于一步完成了 入桶+出桶 的操作。
            int j = get_digit(arr[i], d);
            /**
             * 第一个数 062，个位数为 2，注意看 count 数组中，个位数 <=2 的，有 4 个，这 4 个数应当依次被放在 index 为 0、1、2、3 的位置上。
             * 062 作为最后一个进桶的数，应当被放在最后一个位置上，也就是 index = 3 的位置上。也就是 count[j] - 1 = 3 这个位置上。
             * 因为我们只知道右边界，所以从后往前放。而本边界最后一个位置的 index 就是 count[j] - 1。
             * */
            bucket[count[j] - 1] = arr[i];
            /// arr[i] 已出桶，因此对应的统计结果自减。这是在缩小对应的片区 —— 减小对应片区的右边界
            count[j]--;
        }
        /**
         * d = 1 时，此时 bucket[] = {021, 011, 052, 062, 013}，这就是按照个位数排好序的结果（入桶和出桶均完成）
         * */

        for (int i = l, j = 0; i <= r; i++, j++) {
            arr[i] = bucket[j];
        }
    }
}

/*
 * 返回最大元素的位的个数
 * */
int maxbits(const int *arr, int size) {
    int max_val = INT_MIN;
    for (int i = 0; i < size; i++) {
        max_val = max(max_val, arr[i]);
    }
    int res = 0;
    while (max_val != 0) {
        res++;
        max_val /= 10;
    }
    return res;
}

/*
 * 返回输入 num 第 d 位的数字
 * */
int get_digit(int num, int d) {
    int max_d = 0, tmp = num;
    while (tmp != 0) {
        max_d++;
        tmp /= 10;
    }
    if (max_d < d) {
        return 0;
    }

    int divide_num = 1;
    while (divide_num < d) {
        num /= 10;
        divide_num++;
    }
    return num - num / 10 * 10;
}

int main() {
    cout << get_digit(1235621, 12) << endl;

    int arr_0[] = {17, 13, 25, 100, 72, 6, 8, 90,
                   99, 23, 45, 12, 34, 5, 6, 23};
    basic_radix_sort(arr_0, 16);
    for (int i: arr_0) {
        cout << i << " ";
    }
    cout << endl;

    int arr_1[] = {1, 23, 100, 78, 23, 10, 56, 4, 11, 23, 100};
    radix_sort(arr_1, 11);
    for (int i: arr_1) {
        cout << i << " ";
    }
    cout << endl;

    int arr_2[] = {1, 23, 100, 78, 23, 10, 56, 4, 11, 23, 100};
    solve(arr_2, 11);
    for (int i: arr_2) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
