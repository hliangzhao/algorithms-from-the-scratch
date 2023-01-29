//
// Created by hliangzhao on 25/1/23.
//
#include <iostream>
#include <map>
#include <bitset>

using namespace std;

/**
 * 1、布隆过滤器：只允许元素加入集合，该数据结构能判定一个元素：可能在该集合；或一定不在该集合。
 * 案例：一个包含了100 亿个 URL 的数据表，每个 URL 占用 64 字节的空间，任意给定一个 URL，判定该 URL 是否在该数据表中。
 * 显然，只使用哈希表，无法在较短的时间范围内解决这个问题。使用布隆过滤器，可以在相对小得多的内存占用下进行判定（虽然会有一定错误）。
 *
 * 布隆过滤器的原理是，给定一个长度为 m 的 bitmap（m 很大），当一个元素被加入集合时，
 * 通过 K 个散列函数将这个元素映射成一个 bitmap 中的 K 个点，把它们置为 1：f1(x)%m, f2(x)%m, ..., f_K(x)%m。可能存在一些点位置相同。
 * 检索时，我们只要看看这些点是不是都是 1 就（大约）知道集合中有没有它了：
 * 如果这些点有任何一个 0，则被检元素一定不在；如果都是 1，则被检元素很可能在。
 * 这就是布隆过滤器的基本思想。
 *
 * 失误率随着 m 的增大而减小，随着 K 的增大，失误率先减小后增大。
 *
 * 相比于其它的数据结构，布隆过滤器在空间和时间方面都有巨大的优势。布隆过滤器存储空间和插入/查询时间都是常数 O(K)。
 * 另外，散列函数相互之间没有关系，方便由硬件并行实现。布隆过滤器不需要存储元素本身，在某些对保密要求非常严格的场合有优势。
 * 布隆过滤器可以表示全集，其它任何数据结构都不能。
 *
 * 布隆过滤器参数的确定共识（对应论文中，根据一系列期望运算所得）：
 * 设 n 为样本量，p 为期望失误率，则 m = - (n * ln p) / (ln 2)^2，k = ln 2 * (m / n)
 *
 * 总结：布隆过滤器可以用于集合的建立和元素的查询，可以以很低的空间代价确定某个元素是否一定不在集合中，或可能在集合中。
 * */

/**
 * bitmap 的使用案例。
 * */
void use_bitmap() {
    /// 8 * 10 = 80 bit
    char *arr = new char[4];
    arr[0] = 'a';
    arr[1] = 'B';
    arr[2] = 'x';
    arr[3] = '.';
    for (int k = 0; k < 4; k++) {
        cout << bitset<sizeof(arr[k]) * 8>(arr[k]) << endl;
    }
    cout << endl;

    /// 定位第 15 个 bit
    int i = 15;
    int size_width = 8;
    int num_idx = i / size_width, bit_idx = i % size_width;

    /// 取出
    /// arr[num_idx] >> bit_idx 将该位的数移动至最右位
    /// 和 1 进行与运算
    /// 该位是多少，s的值就是多少
    int s = (arr[num_idx] >> (size_width - bit_idx)) & 1;
    cout << s << endl << endl;

    /// 将该位置为 1（用 'A' - 64 表示 8 位下的 '0000 0001'，从而用于移位运算）
//    cout << bitset<8>('A' - 64) << endl << endl;
//    arr[num_idx] = arr[num_idx] | (('A' - 64) << (size_width - bit_idx));
//
//    for (int k = 0; k < 4; k++) {
//        cout << bitset<sizeof(arr[k]) * 8>(arr[k]) << endl;
//    }
//    cout << endl;

    /// 将该位置为 0
    cout << bitset<8>('A' - 64) << endl;
    cout << bitset<8>(('A' - 64) << (size_width - bit_idx)) << endl;
    cout << bitset<8>(~(('A' - 64) << (size_width - bit_idx))) << endl << endl;
    arr[num_idx] = arr[num_idx] & (~(('A' - 64) << (size_width - bit_idx)));
    for (int k = 0; k < 4; k++) {
        cout << bitset<sizeof(arr[k]) * 8>(arr[k]) << endl;
    }
    cout << endl;
}

int main() {
    use_bitmap();
    return 0;
}