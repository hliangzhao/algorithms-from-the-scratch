//
// Created by hliangzhao on 15/1/23.
//
#include <iostream>

using namespace std;

struct DoublyLinkedNode {
    int value;
    DoublyLinkedNode *left, *right;
};

DoublyLinkedNode *create_dllist(const int *arr, int size);
void print(DoublyLinkedNode *dllist);
void reverse_print(DoublyLinkedNode *dllist);
int get_dllist_node(DoublyLinkedNode *dllist, int index);
void update_dllist(DoublyLinkedNode *dllist, int index, int value);
void add_to_dllist(DoublyLinkedNode *dllist, int index, int value);
void delete_from_dllist(DoublyLinkedNode *dllist, int index);
void delete_dllist(DoublyLinkedNode *dllist);
DoublyLinkedNode *reverse_dllist(DoublyLinkedNode *dllist);

/**
 * 双向链表的创建（有头节点）
 * */
DoublyLinkedNode *create_dllist(const int *arr, int size) {
    DoublyLinkedNode *p = new DoublyLinkedNode, *dllist = p;
    for (int i = 0; i < size; i++) {
        DoublyLinkedNode *q = new DoublyLinkedNode;
        q->value = arr[i];
        p->right = q;
        q->left = p;
        p = p->right;
    }
    return dllist;
}

/**
 * 双向链表的顺序打印
 * */
void print(DoublyLinkedNode *dllist) {
    DoublyLinkedNode *p = dllist->right;
    if (!p) {
        cout << "Empty list!" << endl;
        return;
    }
    while (p) {
        if (!p->right) {
            cout << p->value;
        } else {
            cout << p->value << " --> ";
        }
        p = p->right;
    }
    cout << endl;
}

/**
 * 双向链表的逆序打印
 * */
void reverse_print(DoublyLinkedNode *dllist) {
    DoublyLinkedNode *p = dllist->right;
    if (!p) {
        cout << "Empty list!" << endl;
        return;
    }
    // 到最后一个位置
    while (p->right) {
        p = p->right;
    }
    while (p != dllist) {
        if (p->left == dllist) {
            cout << p->value;
        } else {
            cout << p->value << " --> ";
        }
        p = p->left;
    }
    cout << endl;
}

/**
 * 获取双向链表指定位置的元素
 * */
int get_dllist_node(DoublyLinkedNode *dllist, int index) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        ::exit(-1);
    }
    DoublyLinkedNode *p = dllist->right;
    int i = 0;
    while (i < index) {
        p = p->right;
        i++;
        if (!p) {
            cerr << "Invalid index!" << endl;
            ::exit(-1);
        }
    }
    return p->value;
}

/**
 * 将双向链表中特定位置的元素值修改（链表中的节点从零开始编号）
 * */
void update_dllist(DoublyLinkedNode *dllist, int index, int value) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        return;
    }
    DoublyLinkedNode *p = dllist->right;
    int i = 0;
    while (i < index) {
        p = p->right;
        i++;
        if (!p) {
            cerr << "Invalid index!" << endl;
            return;
        }
    }
    p->value = value;
}

/**
 * 往双向链表中特定位置添加元素
 * */
void add_to_dllist(DoublyLinkedNode *dllist, int index, int value) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        return;
    } else if (index == 0) {
        DoublyLinkedNode *p = dllist;
        DoublyLinkedNode *q = new DoublyLinkedNode;
        q->value = value;
        q->right = p->right;
        q->left = p;
        p->right->left = q;
        p->right = q;
        return;
    }
    DoublyLinkedNode *p = dllist;
    int i = 0;
    while (i < index) {
        p = p->right;
        i++;
        if (!p->right) {
            cerr << "Invalid index!" << endl;
            return;
        }
    }
    DoublyLinkedNode *q = new DoublyLinkedNode;
    q->value = value;
    q->right = p->right;
    q->left = p;
    p->right->left = q;
    p->right = q;
}

/**
 * 将双向链表中特定位置的元素删除
 * */
void delete_from_dllist(DoublyLinkedNode *dllist, int index) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        return;
    }
    if (index == 0) {
        DoublyLinkedNode *p = dllist;
        DoublyLinkedNode *q = p->right;
        if (!q->right) {
            // 删除唯一一个节点
            delete q;
            p->right = nullptr;
            return;
        }
        q->right->left = p;
        p->right = q->right;
        delete q;
        return;
    }
    DoublyLinkedNode *p = dllist;
    DoublyLinkedNode *q = p->right;
    int i = 0;
    while (i < index) {
        p = p->right;
        q = q->right;
        i++;
        if (!q) {
            cerr << "Invalid index!" << endl;
            return;
        }
    }
    if (!q->right) {
        // q 是最后一个节点
        delete q;
        p->right = nullptr;
    } else {
        q->right->left = p;
        p->right = q->right;
        delete q;
    }
}

/**
 * 双向链表的销毁。这里从后往前删除元素
 * */
void delete_dllist(DoublyLinkedNode *dllist) {
    DoublyLinkedNode *p = dllist->right;
    int size = 0;
    while (p->right) {
        p = p->right;
        size++;
    }
    for (int i = size; i >= 0; i--) {
        delete_from_dllist(dllist, i);
        print(dllist);
    }
}

/**
 * 双向链表的翻转
 * 本方法：从后往前将节点插入到前面，然后删除本节点。
 * */
DoublyLinkedNode *reverse_dllist(DoublyLinkedNode *dllist) {
    if (!dllist->right) {
        cerr << "Empty list!" << endl;
        ::exit(-1);
    }
    if (!dllist->right->right) {
        // 单节点链表，直接返回
        return dllist;
    }
    DoublyLinkedNode *p = dllist->right;
    int size = 1;
    while (p->right) {
        p = p->right;
        size++;
    }
    int i = 0;
    while (i < size) {
        add_to_dllist(dllist, i, p->value);
//        print(dllist);
        DoublyLinkedNode *q = p->left;
        delete p;
        q->right = nullptr;
        p = q;
        i++;
    }
    return dllist;
}

int main() {
    int arr[] = {4, 2, 100, 89, 10, 70, 23, 3, 2, 67};
    DoublyLinkedNode *dllist = create_dllist(arr, 10);
    print(dllist);
    reverse_print(dllist);

    cout << get_dllist_node(dllist, 0) << endl;
    cout << get_dllist_node(dllist, 5) << endl;
    cout << get_dllist_node(dllist, 9) << endl;
    cout << get_dllist_node(dllist, -1) << endl;
    cout << get_dllist_node(dllist, 10) << endl;

    update_dllist(dllist, 0, -100);
    update_dllist(dllist, 9, 100);
    update_dllist(dllist, -1, -100);
    update_dllist(dllist, 10, 100);
    print(dllist);

    add_to_dllist(dllist, 0, 100);
    add_to_dllist(dllist, 9, 1000);
    add_to_dllist(dllist, 10, 10000);
    print(dllist);


    delete_from_dllist(dllist, 0);
    delete_from_dllist(dllist, -1);
    delete_from_dllist(dllist, 5);
    delete_from_dllist(dllist, 9);
    delete_from_dllist(dllist, 10);
    print(dllist);

    delete_dllist(dllist);

    reverse_dllist(dllist);
    print(dllist);

    return 0;
}