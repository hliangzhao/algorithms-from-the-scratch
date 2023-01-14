//
// Created by hliangzhao on 13/1/23.
//
#include <iostream>
#include <queue>
#include <set>

using namespace std;

/**
 * 使用比较器：
 * 1、本质就是重载比较运算符；
 * 2、适用于特殊标准下的排序；
 * 3、适用于特殊数据结构下的排序。
 * */

/**
 * 重载运算符
 * 方式一：直接修改模版类的比较函数
 * */
void change_comparator() {
    /// 把默认的 _Compare 改为 greater，从而让 set 内部元素降序排列
    set<int, greater<int>> s;
    set<int, greater<int>>::iterator it;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    for (it = s.begin(); it != s.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

/**
 * 重载运算符
 * 方式二：重载 < 运算符
 * */
class ReverseInt {
    int data;
public:
    explicit ReverseInt(int d = 3) {
        data = d;
    }

    /// 比较运算符只能被重载为「常成员函数」，因此函数签名末尾有一个 const
    /// const 之所以被放在这里，是因为放在前面（const bool）会造成歧义，这是告知编译器这是一个常成员函数的最好位置。
    bool operator<(const ReverseInt r) const {
        /// 当 this.data > r.data 时，认为自己比 r 小
        return data > r.data;
    }

    /// 传入的流需要被返回，且返回的是同一个流。因此函数签名的输出为 ostream &（注意这里的 &，去掉报错）。
    friend ostream &operator<<(ostream &out, ReverseInt r) {
        out << r.data;
        return out;
    }
};

void test_reverse_int() {
    set<ReverseInt> s;
    set<ReverseInt>::iterator it;
    ReverseInt r1(1), r2(3);
    s.insert(r1);
    s.insert(r2);
    for (it = s.begin(); it != s.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

/**
 * 重载运算符
 * 方式三：自定义比较函数
 * */
struct MyCmpForInt{
    bool operator() (const int &a, const int &b) {
        // 小顶堆（后面的 b 比前面的 a 小）
        return b < a;
    }
};

void test_self_defined_comparator() {
    priority_queue<int, vector<int>, MyCmpForInt> q;
    q.push(1);
    q.push(2);
    q.push(3);
    for (int i = 0; i < 3; i++) {
        cout << q.top() << " ";
        q.pop();
    }
    cout << endl;
}

int main() {
    change_comparator();
    test_reverse_int();
    test_self_defined_comparator();

    return 0;
}
