//
// Created by hliangzhao on 26/2/23.
//
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <functional>

using namespace std;

/**
 * 1、容器包含顺序性容器（vector、list、queue、deque 等）和关联性容器（map、set）。
 * 对于前者，其上的迭代操作很方便；对于后者，我们可以快速查找容器中的元素值。
 * */

/**
 * 2、以 vector 为例，抽象出容器中指针的算数运算。
 * */
const int *my_find(const vector<int> &vec, int val) {
    for (auto &v: vec) {
        if (v == val) {
            return &v;
        }
    }
    return nullptr;
}

template<typename elem_type>
const elem_type *my_find(const elem_type *arr, int size, const elem_type &val) {
    if (!arr || size < 1) {
        return nullptr;
    }
    for (auto &v: arr) {
        if (v == val) {
            return &v;
        }
    }
    return nullptr;
}

template<typename elem_type>
const elem_type *my_find(const elem_type *first, const elem_type *last, const elem_type &val) {
    if (!first || !last) {
        return nullptr;
    }
    for (; first != last; first++) {
        if (*first == val) {
            return first;
        }
    }
    return nullptr;
}

template<typename elem_type>
inline const elem_type *my_begin(const vector<elem_type> &vec) {
    return vec.empty() ? nullptr : &vec[0];
}

template<typename elem_type>
inline const elem_type *my_end(const vector<elem_type> &vec) {
    return vec.empty() ? nullptr : &vec[vec.size()];
}

/// STL 帮我们实现了 vector<int>::iterator 和 list<int>::iterator
/// 任何顺序性容器都可以调用本方法来实现对容器的遍历
template<typename iter_type, typename elemType>
iter_type my_find2(iter_type first, iter_type last, const elemType &value) {
    for (; first != last; first++) {
        if (value == *first) return first;
    }
    return last;
}

/**
 * 3、所有容器类以及string类都具备如下通用操作：
 *      == 和 !=
 *      赋值 =
 *      判空 empty()
 *      大小 size()
 *      清空 clear()
 *      迭代器 begin() 和 end()
 *      插入与删除 insert(pos, value) 和 erase(pos)
 * */

/**
 * 4、使用顺序性容器
 *
 * 最主要的顺序性容器是 vector 和 list。
 * 对 vector 的内容进行随机访问（random access）效率很高，但是在任意位置进行插入和删除的效率很低；
 * list 本质上是一个双向链表。对 list 的任意位置进行元素的插入和删除操作效率都很高，
 * 只需设定正确的 back 指针和 front 指针即可，但是对 list 进行随机访问效率很低。
 *
 * 上述特征表示，如果我们需要反复随机访问元素，应当使用 vector；如果需要反复增删改元素，应该使用 list。
 *
 * 另一种常用的顺序性容器是 deque。
 * 在 STL 中，deque 以连续内存空间存储元素，对于首位元素的插入和删除操作效率很高。STL 中的 queue 以 deque 作为底部存储元素。
 *
 * 顺序性容器共有的操作：在尾部进行插入和删除的操作 —— push_back() 和 pop_back()。
 * 此外，list 和 deque 还提供了在首部进行插入和删除的操作 push_front() 和 pop_front()（因为它们首尾都有指针）。
 * */
void use_sequence_container() {
    // 产生空容器
    list<int> l;
    vector<int> v;
    deque<int> d;

    // 产生特定大小的容器，不手动设定初值，则每个元素以元素类型的默认值作为初值（int：0）
    list<int> l1(1024);                // 不设定初值
    vector<int> v1(1024, -1);       // 设定初值
    deque<int> d1(1024);

    // 通过一对iterator产生容器
    int arr[4] = {1, 2, 3, 4};
    list<int> l2(arr, arr + 4);
    vector<int> v2(arr, arr + 4);
    deque<int> d2(arr, arr + 4);

    // 根据已有容器产生新容器（整个复制），这里调用了copy constructor，后面会讲到
    list<int> l3(l2);

    l3.push_back(12);
    l3.insert(l3.end(), 100);
    for (auto &val: l3) {
        cout << val << " ";
    }
    cout << endl;

    auto it = v2.erase(v2.begin());
    cout << *it << endl;
}

/**
 * 5、使用关联性容器
 * */

void use_map() {
    map<string, int> words;
    words["abc"] = 2;
    words["at"] = 10;
    words["make"] = 1;

    // 查询操作1
    auto it = words.find("abc");
    if (it != words.end()) cout << it->second << endl;

    // 查询操作2
    int count = 0;
    if (words.count("abc")) count = words["bac"];

    // 不推荐的查询操作：这样会导致("xyz": 0)被添加进 words
    count = 0;
    if (!(count = words["xyz"])) {
        cout << words["xyz"] << endl;
    }
}

void use_set() {
    string word;
    vector<string> text;

    while (cin >> word) text.push_back(word);
    sort(text.begin(), text.end());

    for (auto &token: text) {
        cout << token << " ";
    }
    cout << endl;
}

/**
 * 6、设计一个泛型算法
 *
 * 问题：对于一个给定的 vector，如何返回一个由该 vector 中小于给定数值的元素组成的新的 vector？
 * */
vector<int> filter(const vector<int> &vec, int val) {
    vector<int> res;
    for (auto &v: vec) {
        if (v < val) {
            res.push_back(v);
        }
    }
    return res;
}

/**
 * 如果允许用户指定不同的比较操作（不限于小于），如何操作？—— 传入函数指针。
 * */
vector<int> filter(const vector<int> &vec, int val, bool (*predicate)(int, int)) {
    vector<int> res;
    for (auto &v: vec) {
        if (predicate(v, val)) {
            res.push_back(v);
        }
    }
    return res;
}

bool less_than(int v1, int v2) {
    return v1 <= v2;
}

bool greater_than(int v1, int v2) {
    return v1 >= v2;
}

/**
 * 通过函数指针来调用传入的比较方法仍然有不可忽视的代价，还有没有别的方法？—— 使用函数对象（function object）。
 * 函数对象是类的实例对象，这些类对函数调用运算符做了重载，使得函数对象可以被当作一般函数来使用。
 * */
void use_func_obj() {
    int arr[] = {1, 4, 3, 56, 7, 89, 90, -8, 91, 100};
    vector<int> v(arr, arr + 10);
    for (int &it: v) cout << it << " ";
    cout << endl;

    /// 类是 greater<int>，通过调用构造函数 greater<int>() 产生该类的一个实例对象传入
    sort(v.begin(), v.end(), greater<int>());
    for (int &it: v) cout << it << " ";
    cout << endl;
}

/**
 * 然而，诸如 greater<>() 或 less<>() 等函数对象传入的参数是两个（二元判断式），
 * 而在我们的问题中，第二个参数需要被指定为 filter_value，我们可以使用绑定适配器（binder adapter）来对第二个参数进行绑定，
 * 从而将二元函数对象转变为一元函数对象。
 *
 * lt 是我们传入的函数对象实例。显然，应当通过引用来调用
 * */
vector<int> filter(const vector<int> &vec, int val, less<int> lt) {
    vector<int> res;
    auto it = vec.begin();
    /// 对于传入的函数对象 lt（是类 less<int> 的一个实例），使用 bind 操作将该函数对象的第二个参数绑定 val
    /// 这里使用 find_if()，因为 find_if() 第三个参数为 Predicate，它在区间 [first,end) 中搜寻使一元判断式 pred 为 true 的第一个元素
    /// 而 find() 第三个参数为待寻找的元素值
    while ((it = find_if(it, vec.end(), bind2nd(lt, val))) != vec.end()) {
        res.push_back(*it);
        it++;
    }
    return res;
}

/**
 * 如果一开始给定不是 vector，而是别的容器，要如何更改 filter() 使其泛化？—— 使用函数模版就可以了。
 * 本函数的写法需要掌握！
 *
 * in_iter：输入容器类型
 * out_iter：输出容器类型
 * elem_type：元素类型
 * comp：函数对象类型
 * */
template<typename in_iter, typename out_iter, typename elem_type, typename comp>
out_iter filter(in_iter first, in_iter last, out_iter at, const elem_type &val, comp pred) {
    while ((first = find_if(first, last, bind2nd(pred, val))) != last) {
        *at++ = *first++;       // 存放满足要求的元素
    }
}

int main() {
    int arr0[] = {1, 9, 8, 0, 4};
    int val = arr0[2];
    const int *ptr = my_find(arr0, arr0 + 5, val);
    cout << ptr << ": " << *ptr << endl;
    vector<int> vec0(arr0, arr0 + 5);
    const int *ptr2 = my_find(my_begin(vec0), my_end(vec0), val);
    cout << ptr2 << ": " << *ptr2 << endl;

    int arr1[] = {1, 9, 8, 0, 4};
    vector<int> vec1(arr1, arr1 + 5);
    list<int> lst(arr1, arr1 + 5);
    int *p = my_find2(arr1, arr1 + 5, arr1[2]);
    cout << p << ": " << *p << endl;
    vector<int>::iterator it1;
    it1 = my_find2(vec1.begin(), vec1.end(), vec1[2]);
    if (it1 != vec1.end()) {
        cout << *it1 << endl;
    }
    list<int>::iterator it2;
    it2 = my_find2(lst.begin(), lst.end(), arr1[2]);
    if (it2 != lst.end()) {
        cout << *it2 << endl;
    }

    use_sequence_container();

    use_map();

    int arr2[] = {1, 4, 3, 56, 7, 89, 90, -8, 91, 100};
    vector<int> v(arr2, arr2 + 10);
    for (int &it: v) cout << it << " ";
    cout << endl;

    // 声明一个函数对象并传入
    less<int> lt;
    vector<int> res = filter(v, 50, lt);

    for (int &re: res) cout << re << " ";
    cout << endl;
}

