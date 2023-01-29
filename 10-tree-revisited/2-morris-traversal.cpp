//
// Created by hliangzhao on 28/1/23.
//
#include <iostream>

using namespace std;

/**
 * Morris 遍历：一种遍历二叉树的方式，时间复杂度为 O(n)，但额外空间复杂度为 O(1)。
 *
 * 流程：
 * 开始时，当前节点 cur 来到头节点的位置；
 * 如果 cur 没有左孩子，cur 向右移动（cur = cur.right）；
 * 如果 cur 有左孩子，找到左子树最右的节点 most_right；
 *      (1) 如果 most_right 的右指针指向空，让其指向 cur，然后 cur 向左移动（cur = cur.left）；
 *      (2) 如果 most_right 的右指针指向 cur，让其指向 null，然后 cur 向右移动（cur = cur.right）；
 * cur 为空时遍历结束。
 *
 * 本质上，Morris 遍历利用叶子结点下的空指针来进行跳转（让这些空指针先指向 cur 下一个要跳转的位置，然后将它们的值赋给 cur，
 * 从而让 cur 跳转到该跳转的位置）。这种思想引出了「线索二叉树」。
 *
 * 如果一个节点没有左树，则该节点只会被 cur 指向一次，否则两次，并且我们可以知道 cur 是第几次被指向。
 * */

struct Node {
    int val;
    Node *left, *right;
};

Node *new_node(int v) {
    auto *node = new Node;
    node->val = v;
    node->left = node->right = nullptr;
    return node;
}

/**
 * 传统的递归遍历算法借助系统提供的函数调用栈来实现 cur 的跳转。
 * */
void proc(Node *root) {
    if (root == nullptr) {
        return;
    }
    // process... (pre-order)
    proc(root->left);
    // process... (in-order)
    proc(root->right);
    // process... (post-order)
}

/**
 * Morris 遍历通过设置叶子结点的右孩子来实现 cur 的跳转。
 * 因为找左子树右边界的时候，遍历的路径是不重复的，因此整个过程时间复杂度就是 O(n)。
 * */
void morris(Node *root) {
    if (root == nullptr) {
        return;
    }
    Node *cur = root, *most_right;
    while (cur != nullptr) {
        most_right = cur->left;
        if (most_right != nullptr) {    /// cur 有左孩子
            while (most_right->right != nullptr && most_right->right != cur) {
                most_right = most_right->right;
            }
            /// 此时 most_right 是 cur 左树上的最右节点
            if (most_right->right == nullptr) {    /// 情况 (1)
                /// 这是第一次遍历到 cur，因为 most_right 的右孩子还没有设置过
                /// 我们设置 most_right 的右孩子，然后让 cur 左移
                most_right->right = cur;
                cout << cur->val << " ";
                cur = cur->left;
                continue;
            } else {                               /// 情况 (2)
                most_right->right = nullptr;
            }
        }
        /// cur 没有左孩子，因此直接让 cur 右移
        cout << cur->val << " ";
        cur = cur->right;
    }
}

/**
 * 基于 Morris 遍历的先序遍历。
 * 对于只能访问一次的节点（这些节点无左子树），遍历到即打印；
 * 对于能访问到两次的节点（这些节点有左子树），第一次遍历到时打印，第二次什么也不做。
 * */
void morris_pre(Node *root) {
    if (root == nullptr) {
        return;
    }
    Node *cur = root, *most_right;
    while (cur != nullptr) {
        most_right = cur->left;
        if (most_right != nullptr) {
            while (most_right->right != nullptr && most_right->right != cur) {
                most_right = most_right->right;
            }
            if (most_right->right == nullptr) {
                most_right->right = cur;
                /// 第一次遍历到 cur 时打印
                cout << cur->val << " ";
                cur = cur->left;
                continue;
            } else {
                most_right->right = nullptr;
            }
        } else {
            /// cur 没有左孩子，只会被遍历到一次，直接打印
            cout << cur->val << " ";
        }
        cur = cur->right;
    }
}

/**
 * 基于 Morris 遍历的中序遍历。
 * 对于只能访问一次的节点（这些节点无左子树），遍历到即打印；
 * 对于能访问到两次的节点（这些节点有左子树），第二次遍历到时打印，第一次什么也不做。
 * */
void morris_in(Node *root) {
    if (root == nullptr) {
        return;
    }
    Node *cur = root, *most_right;
    while (cur != nullptr) {
        most_right = cur->left;
        if (most_right != nullptr) {
            while (most_right->right != nullptr && most_right->right != cur) {
                most_right = most_right->right;
            }
            if (most_right->right == nullptr) {
                most_right->right = cur;
                cur = cur->left;
                continue;
            } else {
                most_right->right = nullptr;
            }
        }
        /// 只需要在这里插入打印行为即可
        cout << cur->val << " ";
        cur = cur->right;
    }
}

/**
 * 基于 Morris 遍历的后序遍历。
 * 对于能访问到两次的节点（这些节点有左子树），第二次遍历到时，逆序打印自己左树的右边界。
 * 最后，当 cur == null 时，逆序打印整棵树的右边界。
 *
 * 对于「逆序打印左树的右边界」这个子步骤，我们可以通过逆序链表的方式来实现：先逆序左树右边界，遍历打印，然后在将链表逆转回来。
 * */

/*
 * 将以 from 开头的子树沿着右边界逆序
 * */
Node *reverse_edge(Node *from) {
    Node *pre = nullptr, *next = nullptr;
    while (from != nullptr) {
        next = from->right;
        from->right = pre;
        pre = from;
        from = next;
    }
    return pre;
}

/*
 * 先逆序，后打印，再逆序回来
 * */
void print_right_edge(Node *root) {
    Node *tail = reverse_edge(root);
    Node *cur = tail;
    while (cur != nullptr) {
        cout << cur->val << " ";
        cur = cur->right;
    }
    reverse_edge(tail);
}

void morris_post(Node *root) {
    if (root == nullptr) {
        return;
    }
    Node *cur = root, *most_right;
    while (cur != nullptr) {
        most_right = cur->left;
        if (most_right != nullptr) {
            while (most_right->right != nullptr && most_right->right != cur) {
                most_right = most_right->right;
            }
            if (most_right->right == nullptr) {
                most_right->right = cur;
                cur = cur->left;
                continue;
            } else {
                most_right->right = nullptr;
                /// 第二次抵达 cur 时，逆序打印左树右边界
                print_right_edge(cur->left);
            }
        }
        cur = cur->right;
    }
    print_right_edge(root);
}

/**
 * 基于 Morris 遍历判断一棵二叉树是否为搜索二叉树（中序遍历节点值递增）。
 * */
bool is_bst(Node *root) {
    if (root == nullptr) {
        return true;
    }
    Node *cur = root, *most_right;
    int pre_max = INT_MIN;
    while (cur != nullptr) {
        most_right = cur->left;
        if (most_right != nullptr) {
            while (most_right->right != nullptr && most_right->right != cur) {
                most_right = most_right->right;
            }
            if (most_right->right == nullptr) {
                most_right->right = cur;
                cur = cur->left;
                continue;
            } else {
                most_right->right = nullptr;
            }
        }
        /// 只需要在这里判断节点值是否总是递增即可
        if (cur->val <= pre_max) {
            return false;
        }
        pre_max = cur->val;
        cur = cur->right;
    }
    return true;
}

int main() {
    Node *tree1 = new_node(1);
    tree1->left = new_node(2);
    tree1->right = new_node(5);
    tree1->left->left = new_node(3);
    tree1->right->left = new_node(6);
    tree1->right->right = new_node(9);
    tree1->right->left->left = new_node(7);
    tree1->right->left->right = new_node(8);
    tree1->right->left->right->right = new_node(4);

    morris(tree1);
    cout << endl;
    morris_pre(tree1);
    cout << endl;
    morris_in(tree1);
    cout << endl;
    morris_post(tree1);
    cout << endl;

    cout << is_bst(tree1) << endl;

    return 0;
}