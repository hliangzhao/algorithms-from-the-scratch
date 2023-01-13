//
// Created by hliangzhao on 13/1/23.
//
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/**
 * 要点：使用函数指针。
 * */

bool legal_size(int size);

const vector<double> *fib_seq(int size);

const vector<double> *square_seq(int size);

const vector<double> *circle_seq(int size);

/// find_element() 第二个参数为 const vector<double> *func(int)，这是一个函数签名。
/// 它和上述三个函数的签名一致。我们用 (*seq_ptr)，一个指针，来作为对应函数的名字，从而指向对应的函数。
/// 这种写法叫做「函数指针」，seq_ptr 就是该变量的名字。
bool find_element(int pos, double &elem, const vector<double> * (*seq_ptr)(int));

enum seq_types {type_fib, type_square, type_circle};

bool legal_size(int size) {
    const int max_size = 1024;
    if (size <= 0 || size > max_size) {
        cout << "invalid size\n";
        return false;
    }
    return true;
}

const vector<double> *fib_seq(int size) {
    static vector<double> elems;
    if (!legal_size(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        if (i == 0 || i == 1) {
            elems.push_back(1);
        } else {
            elems.push_back(elems[i-1] + elems[i-2]);
        }
    }
    return &elems;
}

const vector<double> *square_seq(int size) {
    static vector<double> elems;
    if (!legal_size(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        elems.push_back(M_PI * i * i);
    }
    return &elems;
}

const vector<double> *circle_seq(int size) {
    static vector<double> elems;
    if (!legal_size(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        elems.push_back(i * i);
    }
    return &elems;
}

bool find_element(int pos, double &elem, const vector<double> * (*seq_ptr)(int)) {
    if (seq_ptr == nullptr) {
        elem = 0.0;
        return false;
    }
    /// seq_ptr(pos) 得到一个具体的函数，即 fib_seq()、square_seq()、circle_seq() 三者其一。
    /// 调用这个函数，拿到返回的 seq，因为这三个函数返回的是静态局部变量，因此 seq 需要用 const 修饰。
    const vector<double> *seq = seq_ptr(pos);
    elem = (*seq)[pos - 1];
    return true;
}

int main() {
    /// 定义一个由函数指针组成的数组，这种数组的声明方式和上面无异，唯一区别是加上了[]表示这是一个数组
    /// 该数组内三个元素分别是指向三个生成 seq 方法的函数的指针
    const vector<double> * (*seq_arr[])(int) = {fib_seq, square_seq, circle_seq};
    // 依次输出每一种数列前三个元素
    for (auto & ptr : seq_arr) {
        double elem;
        for (int j = 1; j < 4; j++) {
            if (find_element(j, elem, ptr)) {
                cout << elem << " ";
            }
        }
        cout << endl;
    }

    double elem;
    for (int j = 1; j < 4; j++) {
        // 只关心斐波那契数列的输出，用 enum 变量直接获得对应的函数指针
        if (find_element(j, elem, seq_arr[type_fib])) {
            cout << elem << " ";
        }
    }

    return 0;
}
