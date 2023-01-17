//
// Created by hliangzhao on 17/1/23.
//
#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

/**
 * 要点：
 * 1、根据层序遍历的结果数组创建二叉树（空节点有占位）
 * （1）利用节点在数组中的 index。有递归和非递归的实现方案；
 * （2）利用队列。
 *
 * 2、二叉树的递归思想以及先序遍历、中序遍历、后续遍历
 * ---------------------------
 * void f(Node n) {
 *     if (n == nullptr) {
 *         return;
 *     }
 *     process... // position 1
 *     f(n->left);
 *     process... // position 2
 *     f(n->right);
 *     process... // position 3
 * }
 * ---------------------------
 * 在 position 1-3 中的一个或多个插入代码，就形成了不同的递归序。
 *
 * 案例：
 *         1
 *       /  \
 *      2   3
 *     /\  / \
 *    4 5 6  7
 *
 * 递归序：1 2 4 4 4 2 5 5 5 2 1 3 6 6 6 3 7 7 7 3 1
 *
 * 先序遍历：每个节点第一次被访问时，打印出来；后续被访问到时，什么也不做；
 * 1 2 4 4 4 2 5 5 5 2 1 3 6 6 6 3 7 7 7 3 1
 * p p p       p         p p     p p
 *
 * 中序遍历：每个节点第二次被访问时，打印出来；其余被访问到时，什么也不做；
 * 1 2 4 4 4 2 5 5 5 2 1 3 6 6 6 3 7 7 7 3 1
 *       p   p   p     p     p   p   p
 *
 * 后序遍历：每个节点第三次被访问时，打印出来；其余被访问到时，什么也不做。
 * 1 2 4 4 4 2 5 5 5 2 1 3 6 6 6 3 7 7 7 3 1
 *         p       p p         p       p p p
 *
 * 3、先序、中序、后序的非递归实现
 * 注意，任何递归函数都可以改写为非递归函数。没有系统帮我们压栈（函数调用栈），我们就手动压栈。
 * 我们需要手动创建栈，节点入栈的顺序决定了是先序、中序、还是后序。
 *
 * 4、获取二叉树的深度（一行代码的递归实现）
 *
 * 5、二叉树的宽度优先遍历（层序遍历）
 * 注意，二叉树的深度优先遍历（依次沿着左右子树找到最深的地方）就是先序遍历。
 * 基于 BFS 的问题：求给定二叉树的最大宽度。
 *
 * TODO:
 *  根据先序、中序、后序遍历中的两个还原树
 *  二叉树的递归销毁
 * */

template<typename T>
struct Node {
    T data;
    Node<T> *left;
    Node<T> *right;
};

typedef Node<int> *Tree;

Node<int> *new_node(int value);

Tree create_from_level_order_traversal_recur(const int *arr, int size);

Node<int> *create_from_level_order_traversal_recur_proc(const int *arr, int pos, int size);

Tree create_from_level_order_traversal(const int *arr, int size);

void delete_tree(Tree root);

int get_leaf_num(Tree root);

int get_node_num(Tree root);

int get_level_node_num(Tree root, int k);

void pre_order_recur(Tree root);

void pre_order_traverse(Tree root);

void in_order_recur(Tree root);

void in_order_traverse(Tree root);

void in_order_traverse_2(Tree root);

void post_order_recur(Tree root);

void post_order_traverse(Tree root);

int depth(Tree root);

void write2arr(Tree root, int row_idx, int col_idx, string **res, int depth);

void print_tree(Tree root);

void bfs(Tree root);

int get_max_width(Tree root);

int get_max_width_better(Tree root);

Node<int> *new_node(int value) {
    Node<int> *node = new Node<int>;
    node->data = value;
    node->left = node->right = nullptr;
    return node;
}

/**
 * 根据层序遍历的结果数组创建二叉树。
 * e.g.: {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1}，简单起见，null 用 0 表示
 * 需要利用节点在数组中的下标的关系。
 *
 * 方法一：递归实现
 * */
Tree create_from_level_order_traversal_recur(const int *arr, int size) {
    if (arr == nullptr || size <= 0) {
        return nullptr;
    }
    return create_from_level_order_traversal_recur_proc(arr, 0, size);
}

Node<int> *create_from_level_order_traversal_recur_proc(const int *arr, int pos, int size) {
    if (arr[pos] == 0 || pos >= size) {
        return nullptr;
    }
    Node<int> *node = new_node(arr[pos]);
    node->left = create_from_level_order_traversal_recur_proc(arr, 2 * pos + 1, size);
    node->right = create_from_level_order_traversal_recur_proc(arr, 2 * pos + 2, size);
    return node;
}

/**
 * 根据层序遍历的结果数组创建二叉树。
 * e.g.: {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1}，简单起见，null 用 0 表示
 * 需要利用节点在数组中的下标的关系。
 *
 * 方法二：非递归实现。下面的写法会浪费 <size 个节点空间。且额外申请了大小为 size 的指针数组。
 * */
Tree create_from_level_order_traversal(const int *arr, int size) {
    Node<int> **nodes = new Node<int> *[size];
    for (int i = 0; i < size; i++) {
        nodes[i] = new_node(arr[i]);
    }
    for (int i = 0; i < size / 2; i++) {
        if (nodes[i]->data != 0) {
            if (2 * i + 1 < size && nodes[2 * i + 1]->data != 0) {
                nodes[i]->left = nodes[2 * i + 1];
            }
            if (2 * i + 2 < size && nodes[2 * i + 2]->data != 0) {
                nodes[i]->right = nodes[2 * i + 2];
            }
        }
    }
    return nodes[0];
}

/**
 * 销毁二叉树
 * */
void delete_tree(Tree root) {
    if (root) {
        delete_tree(root->left);
        delete_tree(root->right);
        delete root;
    }
}

/**
 * 计算叶子结点的个数
 * */
int get_leaf_num(Tree root) {
    if (!root) {
        return 0;
    }
    if (!root->left && !root->right) {
        return 1;
    }
    return get_leaf_num(root->left) + get_leaf_num(root->right);
}

/**
 * 计算总节点个数
 * */
int get_node_num(Tree root) {
    if (!root) {
        return 0;
    }
    return get_node_num(root->left) + get_node_num(root->right) + 1;
}

/**
 * 求二叉树第K层的节点个数
 * */
int get_level_node_num(Tree root, int k) {
    if (!root) {
        return 0;
    }
    if (k == 1) {
        return 1;
    }
    return get_level_node_num(root->left, k - 1) + get_level_node_num(root->right, k - 1);
}

/**
 * 根据层序遍历的结果数组创建二叉树。
 * e.g.: {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1}，简单起见，null 用 0 表示
 * 需要利用节点在数组中的下标的关系。
 *
 * 方法三：基于队列的实现。
 * */
Tree create_from_level_order_traversal_with_queue(const int *arr, int size) {
    Node<int> *root = new_node(arr[0]), *cur;
    bool is_left = true;
    queue<Node<int> *> q;
    q.push(root);
    for (int i = 1; i < size; i++) {
        Node<int> *node;
        if (arr[i] != 0) {
            node = new_node(arr[i]);
            q.push(node);
        }
        /// cur 作为队首节点，是每次需要设定左孩子或右孩子的节点
        /// node 则是新创建的节点，它作为 cur 的左孩子或者右孩子，需要根据 flag 决定是左还是右（交替进行）
        if (is_left) {
            cur = q.front();
            q.pop();
            cur->left = node;
            is_left = false;
        } else {
            cur->right = node;
            is_left = true;
        }
    }
    return root;
}

/**
 * 前序遍历的递归实现
 * */
void pre_order_recur(Tree root) {
    if (!root) {
        return;
    }
    cout << root->data << " ";  // position 1
    pre_order_recur(root->left);
    pre_order_recur(root->right);
}

/**
 * 前序遍历的非递归实现：自主压栈
 * */
void pre_order_traverse(Tree root) {
    if (!root) {
        return;
    }
    stack<Node<int> *> s;
    s.push(root); /// 根压栈
    while (!s.empty()) {
        /// 弹出一个节点并处理
        Node<int> *cur_node = s.top();
        cout << cur_node->data << " ";
        s.pop();
        /// 先压右节点，后压左节点
        if (cur_node->right) {
            s.push(cur_node->right);
        }
        if (cur_node->left) {
            s.push(cur_node->left);
        }
    }
}

/**
 * 中序遍历的递归实现
 * */
void in_order_recur(Tree root) {
    if (!root) {
        return;
    }
    in_order_recur(root->left);
    cout << root->data << " ";  // position 2
    in_order_recur(root->right);
}

/**
 * 中序遍历的非递归实现
 * 按照左子树的方向将节点依次入栈，然后弹出时：先打印，后将出节点的右子树入栈。
 *
 * 之所以可以这样做，是因为一棵二叉树，总可以被它的左子树完全分解。
 *
 * 案例：
 *         1
 *       /  \
 *      2   3
 *     /\  / \
 *    4 5 6  7
 *
 * 按左子树分解：
 *         x
 *       /  \
 *      x   z
 *     /\  / \
 *    x y z  a
 *
 * 可视化：
 * 左 头 右
 *       |
 *       左 头 右
 *             |
 *             左 头 右
 *                   ... ...
 * */
void in_order_traverse(Tree root) {
    if (!root) {
        return;
    }
    stack<Node<int> *> s;
    /// 首先，根及所有左孩子/孙子全部入栈
    while (root) {
        s.push(root);
        root = root->left;
    }
    while (!s.empty()) {
        /// 取出栈顶节点，并打印
        Node<int> *cur_node = s.top();
        cout << cur_node->data << " ";
        s.pop();
        /// 如果该节点有右子树，那么以右子树根为新的 root，将以新 root 为根的子树的左孩子/孙子全部入栈
        if (cur_node->right) {
            root = cur_node->right;
            while (root) {
                s.push(root);
                root = root->left;
            }
        }
    }
}

/**
 * 中序遍历的非递归实现，方式二。
 * 思路和上面的代码一样，但是更简洁
 * */
void in_order_traverse_2(Tree root) {
    if (!root) {
        return;
    }
    stack<Node<int> *> s;
    while (!s.empty() || root) {
        if (root) {
            s.push(root);
            root = root->left;
        } else {
            root = s.top();
            cout << root->data << " ";
            s.pop();
            root = root->right;
        }
    }
}

/**
 * 后序遍历的递归实现
 * */
void post_order_recur(Tree root) {
    if (!root) {
        return;
    }
    post_order_recur(root->left);
    post_order_recur(root->right);
    cout << root->data << " ";  // position 3
}

/**
 * 后续遍历的非递归实现
 * 申请两个栈，节点按照"根-右-左"的顺序弹出第一个栈中的元素，并依次将弹出的元素放入第二个栈。
 * 此时，第二个栈的节点弹出顺序天然就是"左-右-根"，即第一个栈元素的逆序，从而直接实现后序遍历
 * */
void post_order_traverse(Tree root) {
    if (!root) {
        return;
    }
    stack<Node<int> *> s, collect;
    s.push(root);
    while (!s.empty()) {
        /// 为了让第一个栈中元素的弹出顺序为"根-右-左"，我们需要将入第一个栈的顺序设置为先左后右
        Node<int> *cur_node = s.top();
        collect.push(cur_node);
        s.pop();

        if (cur_node->left) {
            s.push(cur_node->left);
        }
        if (cur_node->right) {
            s.push(cur_node->right);
        }
    }
    while (!collect.empty()) {
        cout << collect.top()->data << " ";
        collect.pop();
    }
}

/**
 * 返回二叉树的深度
 * */
int depth(Tree root) {
    return root == nullptr ?
           0 : (1 + max(depth(root->left), depth(root->right)));
}

/**
 * 直观打印二叉树的辅助函数：将以 root 作为根的子树写入二维字符串数组中
 * */
void write2arr(Tree root, int row_idx, int col_idx, string **res, int depth) {
    if (!root) {
        return;
    }
    res[row_idx][col_idx] = to_string(root->data);
    int cur_level = (row_idx + 1) / 2;
    if (cur_level == depth) return;
    int gap = depth - cur_level - 1;

    if (root->left) {
        res[row_idx + 1][col_idx - gap] = "/";
        write2arr(root->left, row_idx + 2, col_idx - gap * 2, res, depth);
    }

    if (root->right) {
        res[row_idx + 1][col_idx + gap] = "\\";
        write2arr(root->right, row_idx + 2, col_idx + gap * 2, res, depth);
    }
}

/**
 * 将二叉树可视化打印出来
 * */
void print_tree(Tree root) {
    if (!root) {
        return;
    }
    int dep = depth(root);
    int arr_height = dep * 2 - 1;
    int arr_width = (2 << (dep - 2)) * 3 + 1;

    /// 使用 new 分配二维数组的写法
    string **res;
    res = new string *[arr_height];
    for (int i = 0; i < arr_height; i++) {
        res[i] = new string[arr_width];
    }
    for (int i = 0; i < arr_height; i++) {
        for (int j = 0; j < arr_width; j++) {
            res[i][j] = " ";
        }
    }

    write2arr(root, 0, arr_width / 2, res, dep);

    for (int i = 0; i < arr_height; i++) {
        string str;
        for (int j = 0; j < arr_width; j++) {
            str += res[i][j];
            if (res[i][j].length() > 1 && j < res[i]->length() - 1) {
                j += res[i][j].length() > 4 ? 2 : (int) res[i][j].length() - 1;
            }
        }
        cout << str << endl;
    }
}

/**
 * 二叉树的宽度优先遍历（层序遍历）。
 * 借助队列实现：按先左后右的方式入队，出队即处理（打印）
 * */
void bfs(Tree root) {
    if (!root) {
        return;
    }
    queue<Node<int> *> q;
    q.push(root);
    while (!q.empty()) {
        Node<int> *node = q.front();
        cout << node->data << " ";
        q.pop();
        if (node->left) {
            q.push(node->left);
        }
        if (node->right) {
            q.push(node->right);
        }
    }
}

/**
 * 求二叉树的最大宽度（每一层节点的个数的最大值）。
 * 方法一：引用哈希表，通过改造 BFS 来实现。
 * */
int get_max_width(Tree root) {
    if (!root) {
        return 0;
    }

    queue<Node<int> *> q;
    q.push(root);
    map<Node<int> *, int> m;
    m.insert({root, 1});
    int cur_level = 1, nodes_in_cur_level = 0, max_width = -1;
    while (!q.empty()) {
        Node<int> *cur_node = q.front();
        q.pop();

        int level_of_cur_node = m[cur_node];
        if (level_of_cur_node == cur_level) {
            nodes_in_cur_level++;
        } else {
            /// 弹出的节点的层数不是当前层，则对 nodes_in_cur_level 进行结算（因为已经进入下一层了）
            max_width = max(max_width, nodes_in_cur_level);
            /// 然后进入下一层（cur_level 和 nodes_in_cur_level 的初始化）
            cur_level++;
            nodes_in_cur_level = 1;
        }

        /// 显然，cur_node->left 和 cur_node->right 的层数是 cur_node 的层数 + 1
        if (cur_node->left) {
            q.push(cur_node->left);
            m.insert({cur_node->left, level_of_cur_node + 1});
        }
        if (cur_node->right) {
            q.push(cur_node->right);
            m.insert({cur_node->right, level_of_cur_node + 1});
        }
    }

    /// 最后一层的个数没有来得及和 max_width 比较，因为此时 q 空了，外部 while 循环退出了。
    /// 因此，这里需要再比一下
    return max(max_width, nodes_in_cur_level);
}

/**
 * 求二叉树的最大宽度
 * 方法二：不用哈希表表。
 * */
int get_max_width_better(Tree root) {
    if (!root) {
        return 0;
    }
    // TODO
}

int main() {
    int arr[] = {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1};
    Tree tree1_1 = create_from_level_order_traversal_recur(arr, 13);
    print_tree(tree1_1);

    cout << get_leaf_num(tree1_1) << endl;
    cout << get_node_num(tree1_1) << endl;
    cout << get_level_node_num(tree1_1, 1) << " "
         << get_level_node_num(tree1_1, 2) << " "
         << get_level_node_num(tree1_1, 3) << " "
         << get_level_node_num(tree1_1, 4) << " "
         << get_level_node_num(tree1_1, 5) << " "
         << get_level_node_num(tree1_1, 6) << endl;

    delete_tree(tree1_1);


    Tree tree1_2 = create_from_level_order_traversal(arr, 13);
    print_tree(tree1_2);

    Tree tree1_3 = create_from_level_order_traversal_with_queue(arr, 13);
    print_tree(tree1_3);

    Tree tree2 = new_node(1);
    tree2->left = new_node(2);
    tree2->right = new_node(5);
    tree2->left->left = new_node(3);
    tree2->right->left = new_node(6);
    tree2->right->right = new_node(9);
    tree2->right->left->left = new_node(7);
    tree2->right->left->right = new_node(8);
    tree2->right->left->right->right = new_node(4);

    cout << "Pre: " << endl;
    pre_order_recur(tree2);
    cout << endl;
    pre_order_traverse(tree2);
    cout << endl;

    cout << "In: " << endl;
    in_order_recur(tree2);
    cout << endl;
    in_order_traverse(tree2);
    cout << endl;
    in_order_traverse_2(tree2);
    cout << endl;

    cout << "Post: " << endl;
    post_order_recur(tree2);
    cout << endl;
    post_order_traverse(tree2);
    cout << endl;

    print_tree(tree2);

    bfs(tree2);
    cout << endl;

    cout << get_max_width(tree1_1) << endl;

    return 0;
}
