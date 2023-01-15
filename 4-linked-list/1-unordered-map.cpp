//
// Created by hliangzhao on 14/1/23.
//
#include <iostream>
#include <map> // map 就是有序表，同理 set
#include <unordered_map> // #include <unordered_map> 可以使用 unordered_set，区别是是否有伴随数据（value），底层数据结构一致。
#include <utility>

using namespace std;

/**
 * C++ 中的 map 和 unordered map
 * 1、对比
 * （1）map： map 内部实现了一个红黑树，该结构具有自动排序的功能，因此 map 内部的所有元素都是有序的。
 *      对于 map 进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行这样的操作，故红黑树的效率决定了 map 的效率。
 *      优点：有序；
 *      缺点：空间占用高。
 * （2）unordered_map: unordered_map 内部实现了一个哈希表，因此其元素的排列顺序是杂乱的，无序的。
 *      优点：查找速度快；
 *      缺点：建立哈希表的过程费时。
 *
 * 2、哈希表
 *  哈希表最大的优点，就是把数据的存储和查找消耗的时间大大降低，几乎可以看成是 O(1)；
 *  而代价仅仅是消耗比较多的内存（对于基础数据类型，内部保存的是 key 整体，而非其指针；非基础类型存放的则是指针）。编码比较容易也是它的特点之一。
 *
 *  哈希表的基本原理：使用一个下标范围比较大的数组来存储元素。
 *  可以设计一个函数（哈希函数，也叫做散列函数），使得每个元素的关键字都与一个函数值（即数组下标，hash值）相对应，
 *  于是用这个数组单元来存储这个元素；
 *  也可以简单的理解为，按照关键字为每一个元素“分类”，然后将这个元素存储在相应“类”所对应的地方，称为桶。
 *
 *  不能够保证每个元素的关键字与函数值是一一对应的，因此极有可能出现对于不同的元素，却计算出了相同的函数值，
 *  这样就产生了“冲突”，换句话说，就是把不同的元素分在了相同的“类”之中。 总的来说，“直接定址”与“解决冲突”是哈希表的两大特点。
 *
    哈希表的实现：首先分配一大片内存，形成许多桶。是利用 hash 函数，对 key 进行映射到不同区域（桶）进行保存。其插入过程是：
        1、得到 key
        2、通过 hash 函数得到 hash 值
        3、得到桶号(一般都为 hash 值对桶数求模)
        4、存放 key 和 value 在桶内。

    其取值过程是：
        1、得到 key
        2、通过 hash 函数得到 hash 值
        3、得到桶号(一般都为 hash 值对桶数求模)
        4、比较桶的内部元素是否与 key 相等，若都不相等，则没有找到。
        5、取出相等的记录的 value。
 *
 * ash_map 中直接地址用 hash 函数生成；解决冲突，用比较函数解决。
 * 如果每个桶内部只有一个元素，那么查找的时候只有一次比较。当许多桶内没有值时，许多查询就会更快了(指查不到的时候)。
 * */

typedef unordered_map<string, string> str_hash_map;

/*
 * 将 m1 和 m2 融合为一个 unordered map
 * */
str_hash_map merge(str_hash_map m1, str_hash_map m2) {
    str_hash_map res(std::move(m1));
    res.insert(m2.begin(), m2.end());
    return res;
}

void use_unordered_map() {
    /// 定义与初始化
    unordered_map<string, int> m1 = {{"Mars",    3000},
                                     {"Saturn",  6000},
                                     {"Jupiter", 7000}};
    /// 使用 at() 定位
    m1.at("Mars") = 3396;
    m1.at("Saturn") += 210;
    // 报错：key not found
//    m1.at("New") = 1;

//    for (unordered_map<string, int>::iterator it = m1.begin(); it != m1.end(); it++) {
//        cout << it->first << ": " << it->second << endl;
//    }
    /// 遍历
    for (auto & it : m1) {
        cout << it.first << ": " << it.second << endl;
    }
    /// 使用 size() 和 bucket_count()
    cout << "Size of m1: " << m1.size() << endl;
    cout << "Bucket count of m1: " << m1.bucket_count() << endl;
    cout << endl;

    /// 遍历每一个桶
    cout << "Elements in each bucket:" << endl;
    for (int i = 0; i < m1.bucket_count(); i++) {
        cout << "   bucket #" << i << ": ";
        for (auto it = m1.begin(i); it != m1.end(i); it++) {
            cout << it->first << ": " << it->second << " | ";
        }
        cout << endl;
    }
    cout << endl;

    /// 使用 count() 统计 key 是否在其中
    string tmp[] = {"Mars", "Earth", "Jupiter"};
    for (auto &s: tmp) {
        if (m1.count(s)) {
            cout << s << " is in m1" << endl;
        } else {
            cout << s << " is not in m1" << endl;
        }
    }
    cout << endl;

    /// 插入与删除
    m1.insert({"China", 100});
    m1.insert({"USA", 100});
    m1.insert({"UK", 99});
    pair<string, int> japan("Japan", 50);
    m1.insert(japan);
    m1.insert(make_pair<string, int>("France", 78));
    m1.erase("Mars");
    /// 插入 key 已存在元素会被忽略，value 不会被覆盖（这与 java 不同）
    m1.insert({{"China", 20}, {"USA", 10}});
    for (auto & it : m1) {
        cout << it.first << ": " << it.second << endl;
    }
    cout << endl;
    /// 删除从 给定 key 所在位置到末尾的元素，用 find 找到 key 所在位置，返回一个 iterator
    m1.erase(m1.find("China"), m1.end());
    /// 范围插入
    unordered_map<string, int> m2 = {{"Mexico", 30}, {"Korea", 0}};
    m1.insert(m2.begin(), m2.end());
    for (auto & it : m1) {
        cout << it.first << ": " << it.second << endl;
    }
    cout << endl;

    str_hash_map m3 = {{"AP", "Apple"}, {"MS", "Microsoft"}};
    str_hash_map m4 = {{"GG", "Google"}, {"OC", "Oracle"}};
    str_hash_map m5 = merge(m3, m4);
    /// 虽然 m3 和 m5 包含相同的元素，但是这些元素在二者内部排列方式不一样
    m3 = m5;
    cout << "----- m5 -----" << endl;
    for (auto & it : m5) {
        cout << it.first << ": " << it.second << endl;
    }
    cout << "----- m3 -----" << endl;
    for (auto & it : m3) {
        cout << it.first << ": " << it.second << endl;
    }
    cout << endl;

    /// 更新元素值
    m1.insert({{"China", 100}, {"USA", 100}});
    // 方式一
    auto pos = m1.find("China");
    if (pos != m1.end()) {
        pos->second = 20;
        cout << "China: " << pos->second << endl;
    }
    // 方式二
    if (pos != m1.end()) {
        m1["China"] = 15;
        cout << "China: " << m1["China"] << endl;
    }

}

/**
 * 对于有序表，放入其中的东西如果不是基础类型，则按值传递；否则必须提供将比较器，内部按引用传递。
 * 有序表可以提供固定的 CRUD 复杂度，不论背后的实现方式是什么；
 * 有序表可以通过红黑树、AVL 树、Size-Balanced Tree 等来实现。
 * */

void use_ordered_map() {
    map<string, int> m;
    m["Narcissus"] = 100;
    m["Julia"] = 90;
    m["Angel"] = 97;

    for (const auto& x : m) {
        cout << x.first << ":" << x.second << endl;
    }
}

int main() {
    use_unordered_map();
    use_ordered_map();

    return 0;
}