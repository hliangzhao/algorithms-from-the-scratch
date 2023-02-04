//
// Created by hliangzhao on 11/1/23.
//
#include <iostream>

using namespace std;

/**
 * 异或运算：是一种不进位相加。
 * 特征：0 ^ n = n; n ^ n = 0; 满足交换律和结合律。异或运算在执行时是以二进制位的方式进行上的。
 * 由此：
 * a \leftarrow a^b;
 * b \leftarrow a^b = (a^b)^b = a^(b^b) = a^0 = a;
 * a \leftarrow a^b = (a^b)^a = b^(a^a) = b^0 = b;
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
 * 题目一：给定一个数组，其中有且仅有一个数出现了奇数次，找到这个数。
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

/**
 * 题目二：给定一个数组，其中有且仅有两个数出现了奇数次，找到这两个数。
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
        /// TODO：此处有问题，a^b^c 也仍然有可能在数组中！
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

/**
 * 题目二的正确解法 —— 这个方法时间复杂度是 O(n)，满足要求。
 * */
void find_two_nums2(const int *arr, int size) {
    if (arr == nullptr) {
        std::exit(1);
    }
    int a_xor_b = 0;
    for (int i = 0; i < size; i++) {
        a_xor_b = a_xor_b ^ (arr[i]);
    }
    /// 至此，a_xor_b 变为 a^b，且 a_xor_b 必然不为 0（否则 a = b，a 出现了偶数次）
    /// 所以，a_xor_b 必然有一位是 1。a_xor_b 取反加 1，即得对应的补码。
    /// 一个数和它的补码的和必然是 "10...0"（1 是进位），因此，最右的 1 可以通过该数和它的补码的与运算得到，因为对应位置必然都是 1。
    /// 否则无法进位。
    /// 最终，right_one 是一个对应位置为 1，其余位置皆为 0 的变量。
    /// a 和 b，在这一位上必然是不同的。
    int right_one = a_xor_b & (~a_xor_b + 1);
    int res1 = 0;
    for (int i = 0; i < size; i++) {
        /// 只有那些该位不为 1 的数，才会参与 xor，参与运算的数个数必然 2k + 1，这个落单的，不是 a，就是 b
        /// 然后我们就找到了 a（或b）。进一步，我们就能找到另一个数。
        if ((arr[i] & right_one) == 0) {
            res1 ^= arr[i];
        }
    }
    int res2 = a_xor_b ^ res1;
    cout << res1 << " " << res2 << endl;
}

/**
 * 题目三：给定两个有符号整数 a、b，返回较大的。不允许使用比较运算符。
 * */

/**
 * 将 1 翻转为 0，将 0 翻转为 1
 * */
int flip(int n) {
    if (n != 0 && n != 1) {
        ::exit(1);
    }
    return n ^ 1;
}

/**
 * 若 n 是非负数，则返回 1，否则返回 0
 * */
int non_neg(int n) {
    return flip((n >> 31) & 1);
}

/**
 * 方法一：可以适用于大多数情况。然后，若 a-b 溢出，则本方法失效。
 * */
int find_max(int a, int b) {
    int c = a - b;
    int sc1 = non_neg(c);
    int sc2 = flip(sc1);
    return a * sc1 + b * sc2;       /// 互斥条件相加
}

/**
 * 方法二：考虑溢出的情形。
 * 两数相减，何时会溢出？当 a 和 b 符号相同时，a-b 必然不会溢出。
 * 何时应当返回 a？情况一：a 和 b 符号相同，且 a-b 非负；情况二：a 和 b 符号不同，但 a 非负。
 * */
int find_max2(int a, int b) {
    int c = a - b;
    int sc = non_neg(c);
    int sa = non_neg(a);
    int sb = non_neg(b);
    int diff = sa ^ sb;         /// a 和 b 符号不同则 diff 为 1
    int same = flip(diff);   /// 和 diff 互斥
    int ret_a = diff * sa + same * sc;     /// 返回 a 的两个条件被编码进 ret_，任意条件满足，ret_a 为 1
    int ret_b = flip(ret_a);            /// 和 ret_a 互斥，编码了返回 b 的可能
    return ret_a * a + ret_b * b;
}

/**
 * 题目四：判断一个 unsigned int 是否是 2 的幂。2 的幂：对应的二进制位模式组多只有一个 1。
 *
 * 方法一：
 * 只要拿到该数最右侧的 1，将这个新数和原数进行与运算，若结果和原数相同，则返回 true。
 * */
bool power_of_2(int n) {
    int right_most_1 = n & (~n + 1);
    return (right_most_1 & n) == n;
}

/**
 * 题目四方法二：(n & (n-1)) == 0
 * */
bool power_of_2_2(int n) {
    if (n == 1) {
        return true;
    }
    return (n & (n - 1)) == 0;
}

/**
 * 题目五：判断一个 unsigned int 是否是 4 的幂。
 *
 * 首先，它必须是 2 的幂，其次，任意奇数位不能为 1。
 * */
bool power_of_4(int n) {
    if (!power_of_2(n)) {
        return false;
    }
    if (n == 1) {
        return true;
    }
    /// 0b01010101010101010101010101010101 == 0x55555555
    return (n & 0b01010101010101010101010101010101) != 0;
}

/**
 * 题目六：给定两个 int 类型的整数 a 和 b，不使用算数运算符，如果实现二者的加减乘除？
 * */

/**
 * 加法：和系统保持一致，不解决溢出问题。
 *
 * 异或运算就是无进位加法，此外，与运算的结果左移一位记录的正是进位信息。
 * 因此我们只需要将二者「相加」即可。这个相加仍然借助上述操作实现，直到与运算的进位结果为 0。
 * */
int my_add(int a, int b) {
    int res;
    while (b != 0) {
        res = a ^ b;
        b = (a & b) << 1;
        a = res;
    }
    return res;
}

/**
 * 计算相反数：取补码加 1
 * */
int get_neg(int n) {
    return my_add(~n, 1);
}

/**
 * 减法：a 加上 b 的相反数。和系统保持一致，不解决溢出问题。
 * */
int my_minus(int a, int b) {
    return my_add(a, get_neg(b));
}

/**
 * 乘法：和系统保持一致，不解决溢出问题。
 * */
int my_multi(int a, int b) {
    int res = 0;
    while (b != 0) {
        if ((b & 1) != 0) {
            res = my_add(res, a);
        }
        a <<= 1;
        /// 引入 tmp 是为了实现逻辑右移
        unsigned tmp = ((unsigned) b) >> 1;
        b = (int) tmp;
//        b >>= 1;
    }
    return res;
}

/**
 * 除法：计算 y 除以 x。令 y=ax+b，我们想要找到 a。直接找到 a 很难，我们不妨依次找到 a 的各个位。
 * 令 y=2^n*x+b，2^n*x 由 x 左移 n 位得到。则 a 的第 n 位必然为 1。然后令 y'=y-2^n*x，重复上述步骤计算 y'/x。
 * 如此反复，直到 y'<x 为止。
 * */

bool is_neg(int n) {
    return n < 0;
}

int proc_of_my_divide(int a, int b) {
    int x = is_neg(a) ? get_neg(a) : a;
    int y = is_neg(b) ? get_neg(b) : b;
    int res = 0;
    for (int i = 31; i >= 0; i = my_minus(i, 1)) {
        /// 和注释中的区别是，左移 x 不安全（可能会溢出），因此这里选择右移 y 来进行判断
        if ((x >> i) >= y) {
            res |= (1 << i);
            x = my_minus(x, y << i);
        }
    }
    return is_neg(a) ^ is_neg(b) ? get_neg(res): res;
}

int my_divide(int a, int b) {
    if (b == 0) {
        ::exit(-1);
    }
    if (a == INT_MIN && b == INT_MIN) {
        return 1;
    } else if (b == INT_MIN) {
        return 0;
    } else if (a == INT_MIN) {
        // TODO: ?
        int res = proc_of_my_divide(my_add(a, 1), b);
        return my_add(res, proc_of_my_divide(my_minus(a, my_multi(res, b)), b));
    } else {
        return proc_of_my_divide(a, b);
    }
}

int main() {
    int arr2[5] = {1, -123, 15, -123, 15};
    cout << find_one_num(arr2, 5);

    int arr3[12] = {1, -123, 15, -123, 15, 78,
                    -123, -123, 15, 15, 15, 15};
    int arr4[12] = {1, -123, 15, -123, 15, 78,
                    -123, -123, 15, 15, 15, 15};
    find_two_nums(arr3, 12);
    find_two_nums2(arr4, 12);

    cout << 0b01010101010101010101010101010101 << " " << 0x55555555 << endl;

    cout << my_multi(7, 9) << endl;
    cout << my_multi(-7, 8) << endl;
    cout << proc_of_my_divide(-57, 6) << endl;

    return 0;
}