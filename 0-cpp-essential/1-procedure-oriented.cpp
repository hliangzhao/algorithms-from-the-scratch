//
// Created by hliangzhao on 13/1/23.
//
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

/**
 * 要点：
 * 1、传递参数 or 参数的引用 or 指针。
 * 2、使用 const 修饰常量、参数、静态变量和函数返回值。
 * 3、使用静态局部变量。
 * 4、使用 new 运算符在堆上分配内存。
 * 5、函数重载。
 * 6、使用默认参数。
 * 7、使用函数模版。
 * */

void display(const vector<int> &, ostream & = cout);

void display(const int *, int);

void print2stream(vector<int> &, ostream * = nullptr);

void swap(int &, int &);

void swap(int *, int *);

void swap(int *, int, int);

void bubble_sort(vector<int> &);

void select_sort(int *, int);

void insert_sort(int *, int);

int *copy_from_array(const int *, int);

const int *copy_from_vector(const vector<int> &);

const vector<int> *fib(int);

template <typename elemType> void prints(const string &str, const vector<elemType> &vec);

/**
 * 将参数申明为 const type &，表明该参数不会被本函数修改，并且由于是传引用所以无需在本函数调用栈上复制。
 * 这是「非内部数据类型」的参数（内部数据类型有 int、float、unsigned 等）的最佳写法。效率最高且安全。
 * 内部数据类型直接使用 inner_type x 来传入 x 即可，例如 void func(int x) {}。
 * */
void display(const vector<int> &vec, ostream &os) {
    for (auto v: vec) {
        os << v << " ";
    }
    os << endl;
}

/**
 * 传递的参数为指针类型。这可以起到和引用类似的效果。不会在本函数调用栈上复制。
 * */
void display(const int *arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

/**
 * 输出到流中。流由参数 ofs 指定。使用之前需要判定其是否为空指针。
 * */
void print2stream(vector<int> &vec, ostream *ofs) {
    for (auto v: vec) {
        if (ofs) {
            (*ofs) << v << " ";
        }
    }
    if (ofs) {
        (*ofs) << endl;
    }
}

/**
 * 参数以引用的形式传递。此时，本函数所在调用栈无需将这两个参数拷贝到自身。
 * 既节约了空间，也保证了入参真的可以被本函数修改。
 * */
void swap(int &v1, int &v2) {
    int tmp = v1;
    v1 = v2;
    v2 = tmp;
}

/**
 * 传递的参数为指针类型。这可以起到和引用类似的效果。
 * */
void swap(int *v1, int *v2) {
    int tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

void swap(int *arr, int i, int j) {
    if (i == j) {
        return;
    }
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
}

void bubble_sort(vector<int> &vec) {
    for (unsigned j = vec.size() - 1; j > 0; j--) {
        for (unsigned i = 0; i < j; i++) {
            if (vec[i] > vec[i + 1]) {
                swap(vec[i], vec[i + 1]);
            }
        }
    }
}

void select_sort(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < size; j++) {
            min_idx = arr[j] < arr[min_idx] ? j : min_idx;
        }
        swap(&arr[min_idx], &arr[i]);
    }
}

void insert_sort(int *arr, int size) {
    for (int i = 1; i < size; i++) {
        for (int j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--) {
            swap(arr, j, j + 1);
        }
    }
}

/**
 * 使用 new 运算符在堆上分配内存。
 * 这样，即使本调用栈被释放，dst 仍然存在。
 * */
int *copy_from_array(const int *src, int size) {
    int *dst = new int[size];
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
    return dst;
}

/// 使用 const inner_type x 定义常量 x。
const int SIZE = 14;

/**
 * 使用静态局部变量案例。
 * 此时，dst 的生命周期为程序运行时。即使本函数调用栈被释放，dst 仍然存在 —— 直到本程序结束为止。
 * 如果一个函数返回静态局部变量，那么函数在声明和定义时，返回类型必须标注为 const type，并且函数输出只能赋给 const type 对象。
 * */
const int *copy_from_vector(const vector<int> &vec) {
    /// 静态数组的长度必须是 const 常量
    static int dst[SIZE];
    for (int i = 0; i < SIZE; i++) {
        dst[i] = vec[i];
    }
    return dst;
}


const int MAX_SIZE = 1024;

/**
 * 使用静态局部变量案例。
 * 在函数内创建的静态局部变量，应当返回其指针，而非其本身。从而节省堆空间的占用。
 * */
const vector<int> *fib(int size) {
    static vector<int> seq;
    if (size <= 0 || size > MAX_SIZE) {
        cout << "Invalid size" << endl;
        return nullptr;
    }
    for (unsigned i = seq.size(); i < size; i++) {
        if (i == 0 || i == 1) {
            seq.push_back(1);
        } else {
            seq.push_back(seq[i - 1] + seq[i - 2]);
        }
    }
    /// 需要返回的，是静态局部变量的（首）地址
    return &seq;
}

template <typename elemType> void prints(const string &str, const vector<elemType> &vec) {
    cout << "string: " << str << endl;
    cout << "vec: ";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

int main() {
    vector<int> vec = {1, 3, 4, 45, 67, 23, 45, 2, 6, 232, 45, -12, -89, 100};
    display(vec);
    bubble_sort(vec);
    display(vec);

    int arr0[] = {1, 3, 4, 45, 67, 23, 45, 2, 6, 232, 45, -12, -89, 100};
    int arr0_1[14];
    ::memcpy(arr0_1, arr0, sizeof(int) * 14);
    select_sort(arr0, 14);
    display(arr0, 14);
    insert_sort(arr0_1, 14);
    display(arr0_1, 14);

    int arr1[5] = {1, 2, 3, 4, 5};
    int *arr2 = copy_from_array(arr1, 5);
    display(arr2, 5);

    /// copy_from_vector() 返回静态局部变量，只能赋给 const 对象。
    const int *arr3 = copy_from_vector(vec);
    display(arr3, SIZE);

    /// 使用 append 模式创建一个输出流
    ofstream ofs("data.txt", ios_base::app);
    if (!ofs) {
        /// 使用 cerr 输出错误到标准输出
        cerr << "Error create output file stream" << endl;
        /// 使用 exit 终止程序
        ::exit(-1);
    }
    print2stream(vec, &ofs);

    /// fib() 返回静态局部变量，只能赋给 const 对象。
    const vector<int> *seq = fib(10);
    cout << "Address: " << seq << endl;
    display(*seq);

    prints("Please recall me", vec);

    return 0;
}
