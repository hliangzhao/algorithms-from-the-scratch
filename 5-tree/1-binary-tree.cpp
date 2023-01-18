//
// Created by hliangzhao on 17/1/23.
//
#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 要点：
 * 1、根据数组创建二叉树（下标满足父子关系的数组、LOT 数组、LOT 输入）
 *
 * 2、树上的基本操作
 *
 * 3、二叉树的递归思想以及先序遍历、中序遍历、后续遍历
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
 * 4、先序、中序、后序的非递归实现
 * 注意，任何递归函数都可以改写为非递归函数。没有系统帮我们压栈（函数调用栈），我们就手动压栈。
 * 我们需要手动创建栈，节点入栈的顺序决定了是先序、中序、还是后序。
 *
 * 5、二叉树的宽度优先遍历（层序遍历）
 * 注意，二叉树的深度优先遍历（依次沿着左右子树找到最深的地方）就是先序遍历。
 * 基于 BFS 的问题：求给定二叉树的最大宽度。
 *
 * 6、判断给定二叉树是否为搜索二叉树（该树的每一棵子树的根都满足：左孩子们都比它小，右孩子们都比它大）
 *
 * 7、判断一棵二叉树是否为完全二叉树
 *
 * 8、判断一棵二叉树是否为满二叉树
 *
 * 9、判断一棵二叉树是否为平衡二叉树（任意左右子树高度差不超过1）
 *
 * 10、6～9 可以总结出二叉树的递归框架，彻底掌握之！该框架可以解决一切「树型 DP」问题。
 *
 * 11、找最低公共祖先节点
 *
 * TODO:
 *  根据先序、中序、后序、层序遍历中的两个还原树
 * */

template<typename T>
struct Node {
    T data;
    Node<T> *left;
    Node<T> *right;
};

typedef Node<int> *Tree;

struct ResDataOfBBT {
    bool balanced;
    int depth;
};

struct ResDataOfBST {
    bool bst;
    int min;
    int max;
};

struct ResDataOfFBT {
    int depth;
    int num;
};

Node<int> *new_node(int value);

// 从下标关系满足 (i, 2i+1, 2i+2) 的数组中创建二叉树

Tree create_from_arr(const int *arr, int size);

Tree create_from_arr_recur(const int *arr, int size);

Node<int> *create_proc(const int *arr, int pos, int size);

// 从层次遍历结果数组中创建二叉树

Tree create_from_lot(const int *arr, int size);

// 根据层序遍历（level order traversal）的输入创建二叉树
Tree create_from_lot_input();

// 二叉树上的基本操作：删除树、获得叶子节点个数、获得节点个数、获得第 k 层的节点个数、获取树的深度

void delete_tree(Tree root);

int get_leaf_num(Tree root);

int get_node_num(Tree root);

int get_level_node_num(Tree root, int k);

int depth(Tree root);

// 前序、中序、后序遍历的递归和非递归写法

void pre_order_recur(Tree root);

void pre_order_traverse(Tree root);

void in_order_recur(Tree root);

void in_order_traverse(Tree root);

void in_order_traverse_compact(Tree root);

void post_order_recur(Tree root);

void post_order_traverse(Tree root);

// 将二叉树可视化打印（辅助函数，不要求掌握）

void write2arr(Tree root, int row_idx, int col_idx, string **res, int depth);

void print_tree(Tree root);

// 二叉树的广度优先遍历

void bfs(Tree root);

// 计算给定二叉树的最大宽度

int get_max_width(Tree root);

int get_max_width_better(Tree root);

// 判断二叉树是否为 BST

bool bst_basic(Tree root);

void bst_basic_proc(Tree root, vector<int> &v);

bool bst_recur(Tree root);

bool bst_recur_proc(Tree root, long &prev_val);

bool bst_recur_better(Tree root);

ResDataOfBST bst_recur_better_proc(Tree root);

bool bst_traverse(Tree root);

bool cbt(Tree root);

bool fbt(Tree root);

ResDataOfFBT fbt_proc(Tree root);

bool bbt(Tree root);

bool bbt_better(Tree root);

ResDataOfBBT bbt_better_proc(Tree root);

Node<int> *find_lowest_common_parent(Node<int> *node1, Node<int> *node2);

Node<int> *new_node(int value) {
    Node<int> *node = new Node<int>;
    node->data = value;
    node->left = node->right = nullptr;
    return node;
}

/**
 * 根据结果数组创建二叉树。
 * e.g.: {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1}，简单起见，null 用 0 表示。
 * 需要利用节点在数组中的下标的关系。
 *
 * 注意，这其实不是层序遍历！在层序遍历中，如果一个节点不存在，那么它的左右子树不会被遍历到！
 *
 * 方法一：递归实现
 * */
Tree create_from_arr_recur(const int *arr, int size) {
    if (arr == nullptr || size <= 0) {
        return nullptr;
    }
    return create_proc(arr, 0, size);
}

/**
 * 上方函数的递归自函数
 * */
Node<int> *create_proc(const int *arr, int pos, int size) {
    if (arr[pos] == 0 || pos >= size) {
        return nullptr;
    }
    Node<int> *node = new_node(arr[pos]);
    node->left = create_proc(arr, 2 * pos + 1, size);
    node->right = create_proc(arr, 2 * pos + 2, size);
    return node;
}

/**
 * 根据结果数组创建二叉树。
 * e.g.: {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1}，简单起见，null 用 0 表示。
 * 需要利用节点在数组中的下标的关系。
 *
 * 注意，这其实不是层序遍历！在层序遍历中，如果一个节点不存在，那么它的左右子树不会被遍历到！
 *
 * 方法二：非递归实现。额外申请了大小为 size 的指针数组，额空 O(n)。
 * */
Tree create_from_arr(const int *arr, int size) {
    if (!arr || size <= 0) {
        return nullptr;
    }
    Node<int> **nodes = new Node<int> *[size];
    for (int i = 0; i < size; i++) {
        if (arr[i] != 0) {
            nodes[i] = new_node(arr[i]);
        }
    }
    for (int i = 0; i < size / 2; i++) {
        if (nodes[i]) {
            if (2 * i + 1 < size && nodes[2 * i + 1]) {
                nodes[i]->left = nodes[2 * i + 1];
            }
            if (2 * i + 2 < size && nodes[2 * i + 2]) {
                nodes[i]->right = nodes[2 * i + 2];
            }
        }
    }
    return nodes[0];
}

/**
 * 根据层序遍历（level order traversal）的结果数组创建二叉树。
 * 本题输入和前两个不同，这里的数组元素下标不满足 (i, 2i+1, 2i+2) 的关系。
 * e.g.: {1, 5, 11, 12, 0, 3, 0, 0, 6, 0, 0, 0, 0}（0 元素的左右孩子不会再被遍历到）
 *
 * 此时，我们可以基于队列来实现。
 * */
Tree create_from_lot(const int *arr, int size) {
    Node<int> *root = new_node(arr[0]);
    queue<Node<int> *> q;
    bool is_left = true;
    Node<int> *cur = nullptr; /// 初始化自定义指针时，一定要赋值为 NULL！否则极易出 bug！默认值不是 NULL！
    q.push(root);

    for (int i = 1; i < size; i++) {
        Node<int> *node = nullptr; /// 初始化自定义指针时，一定要赋值为 NULL！否则极易出 bug！默认值不是 NULL！
        if (arr[i] != 0) {
            node = new_node(arr[i]);
            q.push(node);
        }

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
 * 根据层序遍历（level order traversal）的输入创建二叉树。
 * 虽然仍然是层序遍历，但是输入数据和给定数组的处理方式不同。
 * e.g.: 1 5 11 12 0 3 0 0 6 0 0 0 0
 * */
Tree create_from_lot_input() {
    queue<Node<int> *> q;
    Node<int> *root;
    int x;
    cin >> x;
    if (x != 0) {
        root = new_node(x);
        q.push(root);
    }
    while (!q.empty()) {
        Node<int> *cur = q.front();

        cin >> x;
        if (x != 0) {
            cur->left = new_node(x);
            q.push(cur->left);
        }
        cin >> x;
        if (x != 0) {
            cur->right = new_node(x);
            q.push(cur->right);
        }
        q.pop();
    }
    return root;
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
 * 返回二叉树的深度
 * */
int depth(Tree root) {
    return root == nullptr ?
           0 : (1 + max(depth(root->left), depth(root->right)));
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
void in_order_traverse_compact(Tree root) {
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
    int cur_level = 1, nodes_in_cur_level = 0;
    int res = INT_MIN;
    while (!q.empty()) {
        Node<int> *cur_node = q.front();
        q.pop();

        int level_of_cur_node = m[cur_node];
        if (level_of_cur_node == cur_level) {
            nodes_in_cur_level++;
        } else {
            /// 弹出的节点的层数不是当前层，则对 nodes_in_cur_level 进行结算（因为已经进入下一层了）
            res = max(res, nodes_in_cur_level);
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
    return max(res, nodes_in_cur_level);
}

/**
 * 求二叉树的最大宽度
 * 方法二：不用哈希表表。
 * */
int get_max_width_better(Tree root) {
    if (!root) {
        return 0;
    }
    queue<Node<int> *> q;
    q.push(root);

    Node<int> *end_node_of_cur_lvl = root, *end_node_of_next_lvl = nullptr;
    int nodes_in_cur_lvl = 0, res = INT_MIN;

    while (!q.empty()) {
        Node<int> *cur_node = q.front();
        q.pop();
        nodes_in_cur_lvl++;

        /// 和 cur_node 相比，它的左右孩子一定是下一层的
        /// 依次把 end_node_of_next_lvl 设置为当前层各个节点的左右孩子，
        /// 这样到本层结束时（用 cur_node == end_node_of_cur_lvl 来判断），end_node_of_next_lvl 一定是下一层的最后一个元素
        /// 然后进行结算并更新变量

        if (cur_node->left) {
            q.push(cur_node->left);
            end_node_of_next_lvl = cur_node->left;
        }

        if (cur_node->right) {
            q.push(cur_node->right);
            end_node_of_next_lvl = cur_node->right;
        }

        if (cur_node == end_node_of_cur_lvl) {
            res = max(res, nodes_in_cur_lvl);

            end_node_of_cur_lvl = end_node_of_next_lvl;
            nodes_in_cur_lvl = 0;
            end_node_of_next_lvl = nullptr;
        }
    }

    return res;
}

/**
 * 判断一棵二叉树是否为 BST。
 * BST 的中序遍历必然是递增的，因此可以将中序遍历结果存入额外数组，然后检查该数组。
 * */
bool bst_basic(Tree root) {
    vector<int> v;
    bst_basic_proc(root, v);
    /// 直接采用 STL 库函数判断是不是升序
    if (is_sorted(v.begin(), v.end())) {
        return true;
    }
    return false;
}

void bst_basic_proc(Tree root, vector<int> &v) {
    if (!root) {
        return;
    }
    bst_basic_proc(root->left, v);
    v.push_back(root->data);
    bst_basic_proc(root->right, v);
}

/**
 * 判断一棵二叉树是否为 BST。
 * 借助递归中序遍历实现。
 *                 左  <  根  <  右
 *            左 < 根 < 右  左 < 根 < 右
 *                ...           ...
 * 递归会不断地将根展开。
 *
 * pre_val 必须以引用的方式传递，保证递归函数可以修改到它，并且传入的初值为 INT_MIN。
 * */
bool bst_recur(Tree root) {
    if (!root) {
        return false;
    }
    long pre_val = INT_MIN;
    return bst_recur_proc(root, pre_val);
}


bool bst_recur_proc(Tree root, long &prev_val) {
    /// 子树如果不存在，则直接成立。
    if (!root) {
        return true;
    }
    /// 判断左树是否是 BST，如果是，则将 prev_val 改成左树中最大的节点的值；
    bool left = bst_recur_proc(root->left, prev_val);
    if (!left) {
        return false;
    }
    /// 判断左树的根的值和自己（左树的父）相比，自己是否更大，若是，则将 prev_val 改成自身的值
    if (root->data >= prev_val) {
        prev_val = root->data;
    } else {
        return false;
    }

    /// 判断右树是否为 BST，如果是，那么整棵树就是 BST
    /// 至于右树是不是，它需要和传进来的 pre_val 按照上述步骤相比
    return bst_recur_proc(root->right, prev_val);
}

/**
 * 判断一棵二叉树是否为 BST。
 *
 * 按照递归框架求解。
 * 左树是 BST + 右树是 BST + 左 max <= cur_root + 右 min >= cur_root
 * 因此，每个子树需要返回三个信息：是否为 BST、max、min。
 * */
bool bst_recur_better(Tree root) {
    if (!root) {
        return false;
    }
    return bst_recur_better_proc(root).bst;
}

ResDataOfBST bst_recur_better_proc(Tree root) {
    if (!root) {
        return {false, INT_MAX, INT_MIN};
    }
    ResDataOfBST left = bst_recur_better_proc(root->left);
    ResDataOfBST right = bst_recur_better_proc(root->right);

    bool bst = true;
    int min_val = root->data, max_val = root->data;
    if (!(!left.bst && left.min == INT_MAX && left.max == INT_MIN)) {
        min_val = min(min_val, left.min);
//        max_val = max(max_val, left.max);
    }
    if (!(!right.bst && right.min == INT_MAX && right.max == INT_MIN)) {
//        min_val = min(min_val, right.min);
        max_val = max(max_val, right.max);
    }
    // 默认 bst 是 true 是因为若左右孩子为空（即 {false, INT_MAX, INT_MIN}），则必然是 true
    if (!(!left.bst && left.min == INT_MAX && left.max == INT_MIN)) {
        if (!left.bst || left.max > root->data) {
            bst = false;
        }
    }
    if (!(!right.bst && right.min == INT_MAX && right.max == INT_MIN)) {
        if (!right.bst || right.min < root->data) {
            bst = false;
        }
    }

    return {bst, min_val, max_val};
}

/**
 * 判断一颗二叉树是否为 BST。借助非递归中序遍历实现。
 * */
bool bst_traverse(Tree root) {
    if (!root) {
        return true;
    }

    stack<Node<int> *> s;
    long pre_val = INT_MIN;

    while (!s.empty() || root) {
        if (root) {
            s.push(root);
            root = root->left;
        } else {
            root = s.top();
            s.pop();

            if (root->data <= pre_val) {
                return false;
            } else {
                pre_val = root->data;
            }

            root = root->right;
        }
    }

    return true;
}

/**
 * 判断二叉树是否为完全二叉树。
 * 借助 BFS 实现。
 *
 * （1）遇到的任何一个节点，若有右孩子但没有左孩子，则返回 false。
 * （2）遇到的第一个只有左孩子但是没有右孩子的节点，后序节点必须都是叶子节点。
 * */
bool cbt(Tree root) {
    if (!root) {
        return false;
    }

    queue<Node<int> *> q;
    q.push(root);
    bool leaf = false;

    while (!q.empty()) {
        Node<int> *cur = q.front();
        q.pop();
//        if (!cur->left && cur->right) {
//            return false;
//        }
//        if (leaf) {
//            if (cur->left || cur->right) {
//                return false;
//            }
//        }
        if ((!cur->left && cur->right) || (leaf && (cur->left || cur->right))) {
            return false;
        }
        if (cur->left && !cur->right) {
            leaf = true;
        }
        if (cur->left) {
            q.push(cur->left);
        }
        if (cur->right) {
            q.push(cur->right);
        }
    }
    return true;
}

/**
 * 判断一棵二叉树是否为满二叉树。
 * 使用递归框架求解，每个子树应当返回高度和个数。
 * */
bool fbt(Tree root) {
    if (!root) {
        return false;
    }
    ResDataOfFBT res = fbt_proc(root);
    /// 用移位符号判断！
    return res.num == (1 << res.depth) - 1;
}

ResDataOfFBT fbt_proc(Tree root) {
    if (!root) {
        return {0, 0};
    }
    ResDataOfFBT left = fbt_proc(root->left);
    ResDataOfFBT right = fbt_proc(root->right);
    int depth = max(left.depth, right.depth) + 1;
    int num = left.num + right.num + 1;
    return {depth, num};
}

/**
 * 判断一棵二叉树是否为平衡二叉树：左树是 + 右树是 + 左右子树高度差 \leq 1
 * 下面这个写法没有充分利用递归减少高度的计算复杂度
 * */
bool bbt(Tree root) {
    if (!root) {
        return true;
    }
    return bbt(root->left) && bbt(root->right) && (abs(depth(root->left) - depth(root->right)) <= 1);
}

/**
 * 判断一棵二叉树是否为平衡二叉树。
 * 下面这个写法充分利用递归简化了高度的计算
 * */
bool bbt_better(Tree root) {
    ResDataOfBBT res = bbt_better_proc(root);
    return res.balanced;
}

ResDataOfBBT bbt_better_proc(Tree root) {
    if (!root) {
        return {true, 0};
    }
    ResDataOfBBT left = bbt_better_proc(root->left);
    ResDataOfBBT right = bbt_better_proc(root->right);
    int depth = max(left.depth, right.depth) + 1;
    bool balanced = left.balanced && right.balanced && abs(left.depth - right.depth) < 2;
    return {balanced, depth};
}

Node<int> *find_lowest_common_parent(Node<int> *node1, Node<int> *node2) {
    // TODO
}

int main() {
//    int arr1[] = {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1};
//    Tree tree1_1 = create_from_arr_recur(arr1, 13);
//    print_tree(tree1_1);
//
//    cout << get_leaf_num(tree1_1) << endl;
//    cout << get_node_num(tree1_1) << endl;
//    cout << get_level_node_num(tree1_1, 1) << " "
//         << get_level_node_num(tree1_1, 2) << " "
//         << get_level_node_num(tree1_1, 3) << " "
//         << get_level_node_num(tree1_1, 4) << " "
//         << get_level_node_num(tree1_1, 5) << " "
//         << get_level_node_num(tree1_1, 6) << endl;
//    delete_tree(tree1_1);
//
//    Tree tree1_2 = create_from_arr(arr1, 13);
//    print_tree(tree1_2);
//    delete_tree(tree1_2);
//
//    int arr2_1[] = {1, 5, 11, 12, 0, 3, 0, 0, 6, 0, 0, 0, 0};
//    Tree tree2_1 = create_from_lot(arr2_1, 13);
//    print_tree(tree2_1);
//    pre_order_recur(tree2_1);
//    cout << endl;
//
//    int arr2_2[] = {1, 2, 3, 0, 0, 4, 0, 0, 5};
//    Tree tree2_2 = create_from_lot(arr2_2, 9);
//    print_tree(tree2_2);
//    pre_order_recur(tree2_2);
//    cout << endl;
//
//    Node<int> * tree2_3 = create_from_lot_input();
//    print_tree(tree2_3);
//    pre_order_recur(tree2_3);
//    cout << endl;
//
//    Tree tree3 = new_node(1);
//    tree3->left = new_node(2);
//    tree3->right = new_node(5);
//    tree3->left->left = new_node(3);
//    tree3->right->left = new_node(6);
//    tree3->right->right = new_node(9);
//    tree3->right->left->left = new_node(7);
//    tree3->right->left->right = new_node(8);
//    tree3->right->left->right->right = new_node(4);
//    print_tree(tree3);
//    cout << get_max_width_better(tree3) << endl;
//
//    cout << "Pre: " << endl;
//    pre_order_recur(tree3);
//    cout << endl;
//    pre_order_traverse(tree3);
//    cout << endl;
//
//    cout << "In: " << endl;
//    in_order_recur(tree3);
//    cout << endl;
//    in_order_traverse(tree3);
//    cout << endl;
//    in_order_traverse_compact(tree3);
//    cout << endl;
//
//    cout << "Post: " << endl;
//    post_order_recur(tree3);
//    cout << endl;
//    post_order_traverse(tree3);
//    cout << endl;
//
//    print_tree(tree3);
//
//    bfs(tree3);
//    cout << endl;
//
//    int arr3[] = {5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 5, 1, 6, 20};
//    Tree tree4 = create_from_arr_recur(arr3, 15);
//    print_tree(tree4);
//    cout << cbt(tree4) << endl;
//    cout << bbt(tree4) << endl;
//    cout << bbt_better(tree4) << endl;
//    cout << get_max_width_better(tree4) << endl;
//    cout << get_max_width(tree4) << endl;

    int arr4[] = {5, 3, 7, 2, 4, 6, 8, 1, 0, 0, 0, 0, 0, 0, 0};
    Tree tree5 = create_from_lot(arr4, 15);
    print_tree(tree5);

    cout << bst_basic(tree5) << endl;
    cout << bst_recur(tree5) << endl;
    cout << bst_traverse(tree5) << endl;
    cout << bst_recur_better(tree5) << endl;
    cout << fbt(tree5) << endl;

//    int arr5[] = {5, 3, 7, 2, 4, 6, 8, 1, 0, 0, 0, 0, 0, 0, 0};
//    Tree tree6 = create_from_lot(arr5, 15);
//    print_tree(tree6);
//    cout << cbt(tree6) << endl;
//    cout << bbt(tree6) << endl;
//    cout << bbt_better(tree6) << endl;

    return 0;
}
