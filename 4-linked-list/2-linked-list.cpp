//
// Created by hliangzhao on 14/1/23.
//
#include <iostream>
#include <stack>
#include <map>
#include <set>

using namespace std;

/**
 * 要点：
 * 1、单链表的基本操作：创建、销毁、CRUD 以及翻转；
 * 2、打印有序链表的公共部分；
 * 3、面试时，尽量找到空间最省的方，否则考察不到算法。技巧：额外数据结构记录（哈希表等）、快慢指针；
 * 4、判断单链表是否为回文链表的三个方法；
 * 5、将单链表划分为 <、==、> 三个区间的两个方法；
 * 6、拷贝含有随机指针的链表的三个方法；
 * 7、判断链表是否有环的两个方法；
 * 8、单链表相交问题。
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

void divide_region(LinkedNode *l, int pivot);

void divide_region_better(LinkedNode *l, int pivot);

LinkedNode *has_circle(LinkedNode *l);

LinkedNode *has_circle_better(LinkedNode *l);

LinkedNode *intersect(LinkedNode *l1, LinkedNode *l2);

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
 * 基本方法：借助栈手动构造一个链表逆序，然后依次对比即可。
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

void divide_region(LinkedNode *l, int pivot) {
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
void divide_region_better(LinkedNode *l, int pivot) {
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

//////////////////////////////////////////////////////////////////////////////
/// 下面是进阶题目
//////////////////////////////////////////////////////////////////////////////

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

NodeWithRandPtr *copy_rand_list_best(NodeWithRandPtr *l);

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
    return new_l;
}

/**
 * 方法（better）：将每个节点对应的新节点放在本节点后面
 * 1 --> 1' --> 2 --> 2' --> 3 --> 3' --> null
 * 额空为 O(1)
 * 和上面的方法相比没有显著区别，但是这是保持原链表的结构不变的写法。
 * */
NodeWithRandPtr *copy_rand_list_best(NodeWithRandPtr *l) {
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

    return new_l;
}

/**
 * 判断链表是否有环。如果有，返回第一个入环的节点。
 * 方法一：借助 set，额外 O(n)
 * */
LinkedNode *has_circle(LinkedNode *l) {
    set<LinkedNode *> s;
    LinkedNode *p = l->next;
    while (p) {
        set<LinkedNode *>::iterator it = s.find(p);
        if (it != s.end()) {
            return p;
        } else {
            s.insert(p);
        }
        p = p->next;
    }
    return nullptr;
}

/**
 * 判断链表是否有环。如果有，返回第一个入环的节点。
 * 方法二：O(1)
 * 核心思想：入环的这个节点是走不出来的，一个类似于"6"或者"9"的结构。
 * 因此，借助快慢指针，只要有环，二者一定会在某个节点相遇；若无环，则快节点必然抵达 null。
 * */
LinkedNode *has_circle_better(LinkedNode *l) {
    LinkedNode *f = l->next, *s = l->next;
    if (!f->next) {
        // 单节点链表必然无环
        return nullptr;
    }
    while (true) {
        /// 只要是快慢指针，基本就是采用 "f && f->next" 判断是否到尾部
        if (f && f->next) {
            f = f->next->next;
            s = s->next;
        } else {
            return nullptr;
        }
        if (f == s) {
            /// 注意，只要有环，则最多两圈，二者就可以相遇
            /// 重置快指针，二者一定在第一个入环节点处相遇
            f = l->next;
            while (f != s) {
                f = f->next;
                s = s->next;
            }
            break;
        }
    }
    return f;
}

/**
 * 单链表相交问题：给定两个可能有环的单链表的头节点。如果两个链表相交，返回相交的第一个节点；如果不相交，返回 null。
 * 要求：时 O(n)，额空 O(1)
 *
 * 首先需要依次判断两个链表是否有环路。
 * （1）二者均无环：遍历二者，记录二者的长度。如果二者尾节点地址不同，则不相交；
 *      若相交，则长的先走 gap 步，然后二者一起走，第一个地址相同的节点就是我们要的。
 * （2）若一个有环，一个无环：则二者必然不相交。
 * （3）若两个都有环，记录两个入环节点的位置。
 *      若位置相同，则相交。此时和第一步中的子步骤一样 ——
 *          将入环节点作为二者的新终点，统计二者的长度，长的先走 gap 步，然后二者一起走，第一个地址相同的节点就是我们要的。
 *      若位置不同，则必然不相交。
 * */
LinkedNode *intersect(LinkedNode *l1, LinkedNode *l2) {
    // TODO：构造实例测试本函数
    if (!l1->next || !l2->next) {
        // 至少有一个是空链表，必然无交点
        return nullptr;
    }

    LinkedNode *c1 = has_circle_better(l1), *c2 = has_circle_better(l2);

    if (!c1 && !c2) { /// 二者均无环
        LinkedNode *p = l1->next, *q = l2->next;
        int gap = 0;
        while (p->next) {
            gap++;
            p = p->next;
        }
        while (q->next) {
            gap--;
            q = q->next;
        }

        if (p != q) { /// 终点不同，则二者不相交
            return nullptr;
        } else { /// 终点相同，则二者相交，找到第一个交点
            // p 指向长链表的首节点，q 指向另一个的
            p = gap > 0 ? l1->next : l2->next;
            q = p == l1->next ? l2->next : l1->next;
            gap = ::abs(gap);
            // 长链表先走 gap 步
            while (gap != 0) {
                gap--;
                p = p->next;
            }
            // 现在二者一起走，必然会在一个地方抵达同一个点，这个第一个相同点就是我们要的
            while (p != q) {
                p = p->next;
                q = q->next;
            }
            return p;
        }
    } else if (c1 && c2) { /// 二者均有环
        if (c1 == c2) { /// 入环节点相同，二者必然相交。将入环节点作为二者的新终点，重复上面的步骤。
            LinkedNode *p = l1->next, *q = l2->next;
            int gap = 0;
            while (p != c1) {
                gap++;
                p = p->next;
            }
            while (q != c2) {
                gap--;
                q = q->next;
            }
            p = gap > 0 ? l1->next : l2->next;
            q = p == l1->next ? l2->next : l1->next;
            gap = ::abs(gap);
            while (gap != 0) {
                gap--;
                p = p->next;
            }
            while (p != q) {
                p = p->next;
                q = q->next;
            }
            return p;
        } else { /// 入环节点不同，二者可能相交，也可能不相交
            /// 相交：  \ /
            ///         O
            /// 不相交：_O _O
            /// 如何区分？此时，让其中一个链条的入环节点继续往下走，在回到本节点之前，若能遇到第二个链表的入环节点，则二者相交。
            /// 此时任意一个相交的节点均满足要求，可以返回。
            /// 否则二者不相交。
            LinkedNode *p = c1->next;
            while (p != c1) {
                if (p == c2) {
                    return c1;
                }
                p = p->next;
            }
            return nullptr;
        }
    } else { /// 一个有环，一个无环，二者必然不相交
        return nullptr;
    }
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

    divide_region(linked_list, 10);
    divide_region_better(linked_list, 10);
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
    NodeWithRandPtr *new_list = copy_rand_list_best(n0);
    print(new_list);
    print(n0);

    int arr4[] = {1, 2, 3, 4, 5};
    LinkedNode *l4 = create_llist(arr4, 5);
    l4->next->next->next->next->next->next = l4->next->next->next;
    LinkedNode *res = has_circle_better(l4);
    if (res) {
        cout << res->value << endl;
    } else {
        cout << "nullptr" << endl;
    }
}
