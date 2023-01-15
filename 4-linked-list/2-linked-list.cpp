//
// Created by hliangzhao on 14/1/23.
//
#include <iostream>

using namespace std;

/**
 * 要点：单链表的基本操作：创建、销毁、CRUD 以及翻转；
 * */

struct LinkedNode {
    int value;
    LinkedNode *next;
};

LinkedNode *create_llist(const int *arr, int size);
void print(LinkedNode *linked_list);
int get_llist_node(LinkedNode *linked_list, int index);
void update_llist(LinkedNode *linked_list, int index, int value);
void add_to_llist(LinkedNode *linked_list, int index, int value);
void delete_from_llist(LinkedNode *linked_list, int index);
void delete_llist(LinkedNode *linked_list);
LinkedNode *reverse_llist(LinkedNode *linked_list);

/**
 * 链表的创建
 * */
LinkedNode *create_llist(const int *arr, int size) {
    LinkedNode *p = new LinkedNode, *linked_list = p; // 有头节点
    for (int i = 0; i < size; i++) {
        LinkedNode *q = new LinkedNode;
        q->value = arr[i];
        p->next = q;
        p = p->next;
    }
    return linked_list;
}

/**
 * 链表的打印
 * */
void print(LinkedNode *linked_list) {
    LinkedNode *p = linked_list->next;
    if (!p) {
        cout << "Empty list!" << endl;
        return;
    }
    while (p) {
        if (!p->next) {
            cout << p->value;
        } else {
            cout << p->value << " --> ";
        }
        p = p->next;
    }
    cout << endl;
}

/**
 * 获取链表指定位置的元素
 * */
int get_llist_node(LinkedNode *linked_list, int index) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        ::exit(-1);
    }
    LinkedNode *p = linked_list->next;
    int i = 0;
    while (i < index) {
        p = p->next;
        i++;
        if (!p) {
            cerr << "Invalid index!" << endl;
            ::exit(-1);
        }
    }
    return p->value;
}

/**
 * 将链中特定位置的元素值修改（链表中的节点从零开始编号）
 * */
void update_llist(LinkedNode *linked_list, int index, int value) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        return;
    }
    LinkedNode *p = linked_list->next;
    int i = 0;
    while (i < index) {
        p = p->next;
        i++;
        if (!p) {
            cerr << "Invalid index!" << endl;
            return;
        }
    }
    p->value = value;
}

/**
 * 往链表中特定位置添加元素
 * */
void add_to_llist(LinkedNode *linked_list, int index, int value) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        return;
    } else if (index == 0) {
        LinkedNode *p = linked_list;
        LinkedNode *q = new LinkedNode;
        q->value = value;
        q->next = p->next;
        p->next = q;
        return;
    }
    LinkedNode *p = linked_list->next;
    int i = 1;
    while (i < index) {
        p = p->next;
        i++;
        if (!p) {
            cerr << "Invalid index!" << endl;
            return;
        }
    }
    LinkedNode *q = new LinkedNode;
    q->value = value;
    q->next = p->next;
    p->next = q;
}

/**
 * 将链表中特定位置的元素删除
 * */
void delete_from_llist(LinkedNode *linked_list, int index) {
    if (index < 0) {
        cerr << "Invalid index!" << endl;
        return;
    }
    if (index == 0) {
        LinkedNode *p = linked_list;
        LinkedNode *q = p->next;
        p->next = q->next;
        delete q;
        return;
    }

    LinkedNode *p = linked_list->next;
    int i = 1;
    while (i < index) {
        p = p->next;
        i++;
        if (!p->next) {
            cerr << "Invalid index!" << endl;
            return;
        }
    }
    LinkedNode *q = p->next;
    p->next = q->next;
    delete q;
}

/**
 * 链表的销毁：从后往前删除元素
 * */
void delete_llist(LinkedNode *linked_list) {
    LinkedNode *p = linked_list->next;
    int size = 0;
    while (p->next) {
        p = p->next;
        size++;
    }
    for (int i = size; i >= 0; i--) {
        delete_from_llist(linked_list, i);
        print(linked_list);
    }
}

/**
 * 链表的翻转
 * */
LinkedNode *reverse_llist(LinkedNode *linked_list) {
    LinkedNode *q = linked_list, *p = q->next, *m = p->next;
    int size = 0;
    while (true) {
        if (!m) {
            p->next = q;
            size++;
            break;
        }
        p->next = q;
        size++;
        q = p;
        p = m;
        m = m->next;
    }
    // 构造新头节点
    LinkedNode *k = new LinkedNode;
    k->next = p;

    // 移除旧头节点（在末尾）
    p = k;
    int i = 0;
    while (i <= size) {
        q = p;
        p = p->next;
        i++;
    }
    delete p;
    q->next = nullptr;

    return k;
}

int main() {
    int arr[] = {4, 2, 100, 89, 10, 70, 23, 3, 2, 67};
    LinkedNode *linked_list = create_llist(arr, 10);
    print(linked_list);

//    cout << get_llist_node(linked_list, 0) << endl;
//    cout << get_llist_node(linked_list, 9) << endl;
//    cout << get_llist_node(linked_list, -1) << endl;
//    cout << get_llist_node(linked_list, 10) << endl;
//
//    update_llist(linked_list, -1, 1000);
//    update_llist(linked_list, 0, 1000);
//    update_llist(linked_list, 9, 1000);
//    update_llist(linked_list, 10, 1000);
//    print(linked_list);
//
//    add_to_llist(linked_list, -1, 1000);
//    add_to_llist(linked_list, 0, 1000);
//    add_to_llist(linked_list, 9, 1000);
//    add_to_llist(linked_list, 10, 1000);
//    add_to_llist(linked_list, 11, 1000);
//    print(linked_list);
//
//    delete_from_llist(linked_list, -1);
//    delete_from_llist(linked_list, 0);
//    delete_from_llist(linked_list, 9);
//    delete_from_llist(linked_list, 10);
//    print(linked_list);

    delete_llist(linked_list);

    LinkedNode *new_list = reverse_llist(linked_list);
    print(new_list);
}
