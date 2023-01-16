//
// Created by hliangzhao on 14/1/23.
//
#include <iostream>
#include <stack>
#include <map>

using namespace std;

/**
 * 要点：
 * 1、单链表的基本操作：创建、销毁、CRUD 以及翻转；
 * 2、打印有序链表的公共部分；
 * 3、面试时，尽量找到空间最省的方，否则考察不到算法。技巧：额外数据结构记录（哈希表等）、快慢指针；
 * 4、判断单链表是否为回文链表的三个方法；
 * 5、将单链表划分为 <=、==、>= 三个区间的两个方法；
 * 6、拷贝含有随机指针的链表。
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

void print_common_part(LinkedNode *l1, LinkedNode *l2);

bool palindrome(LinkedNode *l);

bool palindrome2(LinkedNode *l);

bool palindrome3(LinkedNode *l);

void divide_region1(LinkedNode *l, int pivot);

void divide_region2(LinkedNode *l, int pivot);

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

/**
 * 打印两个有序链表的公共部分
 * */
void print_common_part(LinkedNode *l1, LinkedNode *l2) {
    if (l1->next == nullptr || l2->next == nullptr) {
        return;
    }
    bool first = true;
    LinkedNode *p1 = l1->next, *p2 = l2->next;
    while (p1 && p2) {
        if (p1->value == p2->value) {
            if (first) {
                cout << p1->value;
                first = false;
            } else {
                cout << " " << p1->value;
            }
            p1 = p1->next;
            p2 = p2->next;
        } else if (p1->value < p2->value) {
            p1 = p1->next;
        } else {
            p2 = p2->next;
        }
    }
}

/**
 * 判断给定链表是否为回文链表。
 * 基本方法：借助助栈手动构造一个链表逆序，然后依次对比即可。
 * 额外空间复杂度：O(n)
 * */
bool palindrome(LinkedNode *l) {
    LinkedNode *p = l->next;
    if (!p) {
        return false;
    }
    stack<int> s;
    while (p) {
        s.push(p->value);
        p = p->next;
    }
    p = l->next;
    while (p) {
        if (s.top() == p->value) {
            s.pop();
            p = p->next;
        } else {
            return false;
        }
    }
    return true;
}

/**
 * 判断给定链表是否为回文链表。
 * 方法二：使用快慢指针使得所需栈空间减半。
 * 额外空间复杂度：O(n)
 * */
bool palindrome2(LinkedNode *l) {
    LinkedNode *p1 = l, *p2 = l;
    while (p1 && p1->next) {
        /// 这里的重点是循环终止的边界条件
        p1 = p1->next;
        p1 = p1->next;
        p2 = p2->next;
    }
    // 奇数和偶数的情况
    // -> h -> 1 -> 2 -> 3 -> 4 -> 5 -> null
    //                   p2              p1
    // -> h -> 1 -> 2 -> 3 -> 4 -> null
    //              p2        p1
    stack<int> s;
    p2 = p2->next;
    while (p2) {
        s.push(p2->value);
        p2 = p2->next;
    }
    p1 = l->next;
    while (!s.empty()) {
        if (s.top() != p1->value) {
            return false;
        } else {
            s.pop();
            p1 = p1->next;
        }
    }
    return true;
}

/**
 * 判断给定链表是否为回文链表。
 * 方法三：先用快慢指针找到中间位置，然后把后半部分逆序。
 * 额外空间复杂度：O(1)
 * */
bool palindrome3(LinkedNode *l) {
    LinkedNode *p1 = l, *p2 = l;
    if (!p1->next->next) {
        // 只有一个元素
        return true;
    }

    int size = 0;
    while (p1 && p1->next) {
        p1 = p1->next;
        p1 = p1->next;
        p2 = p2->next;
        size += 2;
    }

    LinkedNode *q = p2, *p = q->next, *m = p->next;
    while (true) {
        if (!m) {
            p->next = q;
            break;
        }
        p->next = q;
        q = p;
        p = m;
        m = m->next;
    }

    // 此时 p 指向最后一个元素，p2 指向中间元素（偶数个则是前一个）
    p1 = l->next;
    while (p != p2) {
        if (p1->value == p->value) {
            p1 = p1->next;
            p = p->next;
        } else {
            return false;
        }
    }
    /// 当然，最后返回之前应当把链表翻转回去，否则就修改链表了。这部分代码这里没有给出。
    return true;
}

/**
 * 将单链表按照 <=、==、>= 给定值 pivot 进行划分。
 * 笔试方法：引入节点数组，基于荷兰国旗问题的解法来实现。
 * 额外空间复杂度为 O(n)
 * */
const int MAX_SIZE = 200;

void divide_region1(LinkedNode *l, int pivot) {
    LinkedNode *p = l->next;
    if (!p->next) {
        return;
    }

    LinkedNode *node_ptrs[MAX_SIZE];
    int size = 0;
    while (p) {
        node_ptrs[size++] = p;
        p = p->next;
    }

    int i = -1, j = size, k = 0;
    while (k < j) {
        if (node_ptrs[k]->value < pivot) {
            LinkedNode *tmp = node_ptrs[k];
            node_ptrs[k] = node_ptrs[i + 1];
            node_ptrs[i + 1] = tmp;
            i++;
        } else if (node_ptrs[k]->value > pivot) {
            LinkedNode *tmp = node_ptrs[k];
            node_ptrs[k] = node_ptrs[j - 1];
            node_ptrs[j - 1] = tmp;
            j--;
            continue;
        }
        k++;
    }

    // 将节点按照数组中存放的顺序重新组织
    for (i = 0; i < size - 1; i++) {
        p = node_ptrs[i];
        p->next = node_ptrs[i + 1];
    }
    node_ptrs[size - 1]->next = nullptr;
    l->next = node_ptrs[0];
}

/**
 * 将单链表按照 <=、==、>= 给定值 pivot 进行划分。
 * 更好的方法：额外空间复杂度为 O(1)，且能保持节点的相对次序不变
 * */
void divide_region2(LinkedNode *l, int pivot) {
    LinkedNode *lh = nullptr, *lt = nullptr, *eh = nullptr, *et = nullptr, *gh = nullptr, *gt = nullptr;
    LinkedNode *p = l->next;
    if (!p) {
        cerr << "Empty list!" << endl;
        return;
    }
    while (p) {
        if (p->value < pivot) {
            if (!lh && !lt) {
                lh = lt = p;
            } else {
                lt->next = p;
                lt = lt->next;
            }
        } else if (p->value == pivot) {
            if (!eh && !et) {
                eh = et = p;
            } else {
                et->next = p;
                et = et->next;
            }
        } else {
            if (!gh && !gt) {
                gh = gt = p;
            } else {
                gt->next = p;
                gt = gt->next;
            }
        }
        p = p->next;
    }
    /// 不同区域可能为空，需要依次讨论
    if (!lh) {
        if (!eh) {
            l->next = gh;
            gt->next = nullptr;
        } else {
            l->next = eh;
            if (!gh) {
                et->next = nullptr;
            } else {
                et->next = gh;
                gh->next = nullptr;
            }
        }
    } else {
        l->next = lh;
        if (!eh) {
            if (!gh) {
                lt->next = nullptr;
            } else {
                lt->next = gh;
                gt->next = nullptr;
            }
        } else {
            if (!gh) {
                lt->next = eh;
                et->next = nullptr;
            } else {
                lt->next = eh;
                et->next = gh;
                gt->next = nullptr;
            }
        }
    }
}

/**
 * 给定一个含有随机指针节点的链表的头节点，完成这个链表的复制并返回新链表的头节点。
 * 要求：时 O(n)，额空 O(1)
 * */
struct NodeWithRandPtr {
    int value;
    NodeWithRandPtr *next, *rand;
};

void print(NodeWithRandPtr *l);

NodeWithRandPtr *copy_rand_list(NodeWithRandPtr *l);

NodeWithRandPtr *copy_rand_list2(NodeWithRandPtr *l);

NodeWithRandPtr *copy_rand_list3(NodeWithRandPtr *l);

void print(NodeWithRandPtr *l) {
    NodeWithRandPtr *p = l->next;
    if (!p) {
        cout << "Empty list!" << endl;
        return;
    }
    while (p) {
        if (!p->next) {
            cout << "(" << p->value << ", " << p->rand->value << ")";
        } else {
            cout << "(" << p->value << ", " << p->rand->value << ") --> ";
        }
        p = p->next;
    }
    cout << endl;
}

/**
 * 方法一：使用额外空间 O(n)。借助 map 来实现。map 的 k-v 是一对旧-新节点
 * */
NodeWithRandPtr *copy_rand_list(NodeWithRandPtr *l) {
    map<NodeWithRandPtr *, NodeWithRandPtr *> m;
    NodeWithRandPtr *p = l, *q;
    while (p) {
        q = new NodeWithRandPtr;
        q->value = p->value;
        m.insert({p, q});
        p = p->next;
    }
    p = l;
    while (p) {
        m[p]->next = m[p->next];
        m[p]->rand = m[p->rand];
        p = p->next;
    }
    return m[l];
}

/**
 * 方法二：将每个节点对应的新节点放在本节点后面
 * 1 --> 1' --> 2 --> 2' --> 3 --> 3' --> null
 * 下面的写法会修改原链表！
 * 额空为 O(1)
 * */
NodeWithRandPtr *copy_rand_list2(NodeWithRandPtr *l) {
    NodeWithRandPtr *p = l->next, *q;
    while (p) {
        q = new NodeWithRandPtr;
        q->value = p->value;
        // 将 q 设置为 p 的下一个节点
        q->next = p->next;
        p->next = q;

        // 更新 p
        p = q->next;
    }
    p = l->next, q = p->next;
    while (true) {
        q->rand = p->rand->next;
        p = p->next->next;
        if (!p) {
            break;
        }
        q = q->next->next;
    }

    NodeWithRandPtr *new_l = new NodeWithRandPtr;
    new_l->next = l->next->next;
    // 移除旧节点
    q = l->next->next;
    p = l->next->next->next;
    while (p) {
        q->next = p->next;
        q = p->next;
        p = p->next->next;
    }
    return  new_l;
}

/**
 * 方法（better）：将每个节点对应的新节点放在本节点后面
 * 1 --> 1' --> 2 --> 2' --> 3 --> 3' --> null
 * 额空为 O(1)
 * 保持原链表的结构不变的写法。
 * */
NodeWithRandPtr *copy_rand_list3(NodeWithRandPtr *l) {
    NodeWithRandPtr *p = l->next, *q;
    while (p) {
        q = new NodeWithRandPtr;
        q->value = p->value;
        // 将 q 设置为 p 的下一个节点
        q->next = p->next;
        p->next = q;

        // 更新 p
        p = q->next;
    }
    p = l->next, q = p->next;
    while (true) {
        q->rand = p->rand->next;
        p = p->next->next;
        if (!p) {
            break;
        }
        q = q->next->next;
    }

    NodeWithRandPtr *new_l = new NodeWithRandPtr;
    new_l->next = l->next->next;
    // 移除旧节点
    p = l->next;
    q = l->next->next;
    NodeWithRandPtr *m = l->next->next->next;
    while (m) {
        q->next = m->next;
        p->next = m;

        p = m;
        q = m->next;
        m = m->next->next;
    }
    p->next = m;

    return  new_l;
}

int main() {
    int arr[] = {13, 13, 13};
    LinkedNode *linked_list = create_llist(arr, 3);
    print(linked_list);

    cout << get_llist_node(linked_list, 0) << endl;
    cout << get_llist_node(linked_list, 9) << endl;
    cout << get_llist_node(linked_list, -1) << endl;
    cout << get_llist_node(linked_list, 10) << endl;

    update_llist(linked_list, -1, 1000);
    update_llist(linked_list, 0, 1000);
    update_llist(linked_list, 9, 1000);
    update_llist(linked_list, 10, 1000);
    print(linked_list);

    add_to_llist(linked_list, -1, 1000);
    add_to_llist(linked_list, 0, 1000);
    add_to_llist(linked_list, 9, 1000);
    add_to_llist(linked_list, 10, 1000);
    add_to_llist(linked_list, 11, 1000);
    print(linked_list);

    delete_from_llist(linked_list, -1);
    delete_from_llist(linked_list, 0);
    delete_from_llist(linked_list, 9);
    delete_from_llist(linked_list, 10);
    print(linked_list);

    delete_llist(linked_list);

    LinkedNode *ll = reverse_llist(linked_list);
    print(ll);

    int arr1[] = {1, 2, 4, 8, 9, 10, 12, 14, 18, 100};
    int arr2[] = {2, 3, 8, 10, 14, 15, 18, 76, 100, 101, 200};
    LinkedNode *l1 = create_llist(arr1, 10);
    LinkedNode *l2 = create_llist(arr2, 11);
    print_common_part(l1, l2);

    int arr3[] = {1, 2, 2, 2, 4};
    LinkedNode *l3 = create_llist(arr3, 5);
    cout << palindrome(l3) << endl;
    cout << palindrome2(l3) << endl;
    cout << palindrome3(l3) << endl;

    divide_region1(linked_list, 10);
    divide_region2(linked_list, 10);
    print(linked_list);

    NodeWithRandPtr *n0 = new NodeWithRandPtr;
    NodeWithRandPtr *n1 = new NodeWithRandPtr;
    n1->value = 1;
    NodeWithRandPtr *n2 = new NodeWithRandPtr;
    n2->value = 2;
    NodeWithRandPtr *n3 = new NodeWithRandPtr;
    n3->value = 3;
    NodeWithRandPtr *n4 = new NodeWithRandPtr;
    n4->value = 4;
    NodeWithRandPtr *n5 = new NodeWithRandPtr;
    n5->value = 5;
    n0->next = n1;
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = nullptr;
    n1->rand = n2;
    n2->rand = n2;
    n3->rand = n1;
    n4->rand = n3;
    n5->rand = n4;
    print(n0);
    NodeWithRandPtr *new_list = copy_rand_list3(n0);
    print(new_list);
    print(n0);
}
