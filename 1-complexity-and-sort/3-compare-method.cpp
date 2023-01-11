//
// Created by hliangzhao on 11/1/23.
//
#include <iostream>
#include <random>
#include <cstring>

using namespace std;

/*
 * 编写所谓的「对数器」代码，对算法进行测试和检验。
 * */

// 使用 C11 风格生成随机数
int *generate_arr(int size, int max_val) {
    // Without "mt" as the seed, the output of the for-loop in compare() will be the same!
    ::random_device rd;
    ::mt19937 mt(rd());
    int *arr = new int[size];
    uniform_int_distribution<int> dist(0, max_val);
    for (int i = 0; i < size; i++) {
        arr[i] = dist(mt);
    }
    return arr;
}

// select_sort 是待测试函数
void select_sort(int *arr, int size) {
    if (arr == nullptr || size < 2) {
        return;
    }
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < size; j++) {
            min_idx = arr[j] < arr[min_idx] ? j : min_idx;
        }
        // swap
        int tmp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = tmp;
    }
}

const int TEST_NUM = 10;

void compare() {
    for (int t = 0; t < TEST_NUM; t++) {
        int size = 10, max_val = 100;
        int *arr1 = generate_arr(size, max_val);
        int *arr2 = new int[size];
        // copy arr1 to a newly allocated array arr2
        ::memcpy(arr2, arr1, size * sizeof(int));

        select_sort(arr1, size);
        // use self-contained func
        ::sort(arr2, arr2 + size);

//        for (int i = 0; i < size; i++) {
//            cout << arr1[i] << " ";
//        }
//        cout << endl;
//        for (int i = 0; i < size; i++) {
//            cout << arr2[i] << " ";
//        }
//        cout << endl;

        // now we test
        for (int i = 0; i < size; i++) {
            if (arr1[i] != arr2[i]) {
                cout << "target failed" << endl;
                ::exit(1);
            }
        }
    }
    cout << "success" << endl;
}

int main() {
    compare();
    return 0;
}