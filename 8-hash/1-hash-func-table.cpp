//
// Created by hliangzhao on 25/1/23.
//
#include <iostream>
#include <map>
#include <random>

using namespace std;

/**
 * 要点：理解哈希函数和哈希表的实现（用于面试）
 *
 * 1、哈希函数：y = f(x), x 可以取任何值，但是值域是一个有界的范围
 * （1）不含有随机成分，同一个输入必然得到同一个输出。
 * （2）不同的输入，可能会导致相同的输出（哈希碰撞），但概率极低。越低越好。
 * （3）哈希函数可以从期望上保证任意输入映射到值域上时，满足均匀分布，这意味着哈希函数内部时无规律的（离散性/均匀性）。
 *
 * 2、哈希表的实现
 * 申请一个大小为 SIZE 的指针数组，每个原始是一个指向链表的指针。
 * 对于每个传入的 {key, value}，带入一个内部的哈希函数计算得到 y，然后将该键值对链接到以 y % SIZE 作为 index 的元素所指向的链表末尾。
 * 键值对的查询也遵循类似的步骤。可以规定一个链表最大长度，当存在一个（或很多，看具体实现）链表长度都超过该最大长度时，
 * 申请扩容（SIZE 翻倍），然后 rehash。
 *
 * 基于上述设计，可以保证不扩容时，插入、查询键值对到哈希表中的复杂度都几乎为 O(1)。
 * 此外，扩容的次数为 O(log n)，单次扩容后，rehash 的代价是 O(n)。
 * 因此，整体上，查询/插入键值对的代价平均为 O(n * log n / n)。但是，如果哈希表一开始的 SIZE 开得较大，且其扩容由 runtime
 * 在后台用户无感式完成，则可以认为存取的速度为 O(1)。
 * */

/**
 * 题目一：给定一个包含 40 亿个无符号整数（范围 0 ～ 约 42 亿）的文件和 1G 内存，要求返回出现次数最多的数。
 *
 * 解：使用哈希函数。
 * 将每个数 x 传入某个哈希函数得到 y，再将 y % 100。根据该结果将 x 分发至编号为 y % 100 的文件中。
 * 根据哈希函数的随机性，这 100 个文件内包含的数的数量应当几乎相同，依次检查每个文件中出现次数最多的数（使用 map<int, int>），
 * 再将结果合并即可。处理每个文件仅需要 0.25 亿 * 8 Bytes = 2 亿 < 10 亿 ～ 1 GB。依次进行是可行的。
 *
 * 使用哈希函数既可以保证同一个数只会出现在一个文件中，这使得合并解的时候很方便；又能减少内存占用。
 * */

/**
 * 题目二：设计 RandomPool 结构，要求：
 * （1）insert(key)：做到不重复加入；
 * （2）delete(key)：删除；
 * （3）get_random()：等概率返回任意一个key。
 * 要求三个操作的时间复杂度都是 O(1)。
 *
 * 不是要你自己实现 C++ 内部的 hash se，借助它们实现即可。
 * */

/**
 * 维护两张哈希表，每次删除元素时，把最后一个元素填充到被删除元素的位置（修改对应的 index），从而保证随机时不出现空位。
 * */
template<typename key_type>
struct RandomPool {
    map<key_type, int> key2idx;
    map<int, key_type> idx2key;
    int size{0};
};

template<typename key_type>
void insert(RandomPool<key_type> *pool, key_type key) {
    if (pool->key2idx.find(key) == pool->key2idx.end()) {
        pool->key2idx.insert({key, pool->size});
        pool->idx2key.insert({pool->size++, key});
    }
}

template<typename key_type>
void del(RandomPool<key_type> *pool, key_type key) {
    if (pool->key2idx.find(key) != pool->key2idx.end()) {
        int del_idx = pool->key2idx[key];
        int last_idx = --pool->size;
        key_type last_key = pool->idx2key[last_idx];

        pool->key2idx[last_key] = del_idx;
        pool->idx2key[del_idx] = last_key;

        pool->key2idx.erase(key);
        pool->idx2key.erase(last_idx);
    }
}

template<typename key_type>
key_type get_random(RandomPool<key_type> *pool) {
    if (pool->size == 0) {
        return nullptr;
    }

    ::random_device rd;
    ::mt19937 mt(rd());
    int *arr = new int[pool->size];
    uniform_int_distribution<int> rand_distribution(0, pool->size);
    return pool->idx2key[rand_distribution(mt)];
}

int main() {
    return 0;
}