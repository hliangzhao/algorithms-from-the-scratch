//
// Created by hliangzhao on 30/1/23.
//
#include <iostream>

using namespace std;

/**
 * 有序表：在哈希表的基础上，按照 key 有序组织。
 * 有序表的操作大多为 O(log n) 级别。
 *
 * 可以用于实现有序表的数据结构有：红黑树、AVL 树、SB 树、跳表等。前三者均可归类为平衡二叉搜索树 BST。
 * */

struct Node {
    int val;
    Node *left, *right, *p;
};

typedef Node *BinarySearchTree;

Node *new_node(int val);

/**
 * 一、实现 BST 的新增、查询、删除节点等操作。
 * 默认情况下，BST 不包含重复元素值节点。如果有重复元素值，可以将 val 从 int 改为 map<val, vector<some struct>>。
 * */

void in_order(BinarySearchTree root);

Node *query_recur(BinarySearchTree root, int key);

Node *query_iter(BinarySearchTree root, int key);

Node *minimum(BinarySearchTree root);

Node *maximum(BinarySearchTree root);

Node *successor(Node *node);

Node *predecessor(Node *node);

Node *insert(BinarySearchTree root, int key);

BinarySearchTree transplant(BinarySearchTree root, Node *u, Node *v);

Node *del(BinarySearchTree root, Node *z);

Node *new_node(int val) {
    Node *node = new Node;
    node->val = val;
    node->left = node->right = node->p = nullptr;
    return node;
}

/**
 * 中序遍历的结果是一个递增序列
 * */
void in_order(BinarySearchTree root) {
    if (root != nullptr) {
        in_order(root->left);
        cout << root->val << " ";
        in_order(root->right);
    }
}

/**
 * 查询：时间复杂度为 O(h)，h 为树的高度
 * */
Node *query_recur(BinarySearchTree root, int key) {
    if (root == nullptr || root->val == key) {
        return root;
    }
    if (key < root->val) {
        return query_recur(root->left, key);
    }
    return query_recur(root->right, key);
}

/**
 * 查询的非递归写法
 * */
Node *query_iter(BinarySearchTree root, int key) {
    while (root != nullptr && root->val != key) {
        if (key < root->val) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}

/**
 * 获取全局最小值
 * */
Node *minimum(BinarySearchTree root) {
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}

/**
 * 获取全局最大值
 * */
Node *maximum(BinarySearchTree root) {
    while (root->right != nullptr) {
        root = root->right;
    }
    return root;
}

/**
 * 返回给定节点的后继。
 * 如果 node 有右孩子，则后继为以右孩子为子树的最小节点；
 * 如果 node 没有右孩子，则后继为 node 的满足特定要求的「最低」祖先 y，该要求为：y 的「左」孩子也是 x 的祖先节点。
 * */
Node *successor(Node *node) {
    if (node->right != nullptr) {
        return minimum(node->right);
    }
    Node *y = node->p;
    /// 利用全局 root 的 p 为空来实现「最末节点的后继返回空」这个目标
    while (y != nullptr && node == y->right) {
        node = y;
        y = node->p;
    }
    return y;
}

/**
 * 返回给定节点的前驱。和后继的代码正好左右逆转。
 * */
Node *predecessor(Node *node) {
    if (node->left != nullptr) {
        return maximum(node->left);
    }
    Node *y = node->p;
    while (y != nullptr && node == y->left) {
        node = y;
        y = node->p;
    }
    return y;
}

/**
 * 插入
 * */
Node *insert(BinarySearchTree root, int key) {
    Node *y = nullptr, *x = root;
    while (x != nullptr) {
        y = x;
        if (key < x->val) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    Node *z = new_node(key);
    z->p = y;
    if (y == nullptr) {
        root = z;
    } else if (z->val < y->val) {
        y->left = z;
    } else {
        y->right = z;
    }
    return z;
}

/**
 * 删除（CLRS, p319）。需要分类讨论：
 * 1、待删除节点 node 没有孩子。直接删除即可；
 * 2、待删除节点 node 只有一个孩子。则让该孩子 c 代替 node 的地位（node.p.left or node.p.right = c）；
 * 3、待删除节点 node 有两个孩子。此时较为麻烦。我们可以让以 node 的左孩子为子树根的树的最右节点（前驱）、或者以 node
 * 的右孩子为子树根的最左节点（后继）来代替 node 的地位。默认情况下，我们选择后者。不妨将该节点（也就是 node 的后继）记为 y。
 *      (1) 如果 y 不是待删除节点 node 的右孩子，让 y 的右孩子代替 y，然后让 y 代替 node（第一步，让 y 成为 node 的右孩子的新父亲；
 *          第二步，让 y 成为 node 的左孩子的新父亲）。
 *      (2) 如果 y 是待删除节点 node 的右孩子，直接让 y 代替 node 即可，只需要让 y 成为 node 的左孩子的新父亲。
 * */

/**
 * 子程序：将以 v 为根的子树完全代替以 u 为根的子树。
 * 返回新的全局根。
 * */
BinarySearchTree transplant(BinarySearchTree root, Node *u, Node *v) {
    if (u->p == nullptr) {
        /// u 是全局根，直接把全局根置为 v
        root = v;
    } else if (u == u->p->left) {
        /// u 是父亲的左孩子，则把 v 置为父亲的新左孩子
        u->p->left = v;
    } else {
        /// u 是父亲的右孩子，则把 v 置为父亲的新右孩子
        u->p->right = v;
    }
    if (v != nullptr) {
        /// 如果 v 不为空，则将 v 的父亲更新为 u 的父亲
        v->p = u->p;
    }
    return root;
}

/**
 * 删除的实现：返回新的全局根。
 * */
Node *del(BinarySearchTree root, Node *z) {
    BinarySearchTree new_root;
    /// 前两个 if 包含了三种情况：z 的左右孩子都为空、z 的左孩子为空但右孩子不为空、z 的右孩子为空但左孩子不为空
    /// 不论如何，我们只需要将不为空的孩子全然代替 z 的地位即可
    if (z->left == nullptr) {
        new_root = transplant(root, z, z->right);
    } else if (z->right == nullptr) {
        new_root = transplant(root, z, z->left);
    } else {
        /// z 的左右孩子都存在。我们需要根据 y 是否是 z 的直接右孩子进行分类讨论。
        Node *y = minimum(z->right);
        if (y->p != z) {
            /// y 不是 z 的直接右孩子，此时相对麻烦，我们首先需要让 y 的右孩子代替 y；
            /// 其次需要让 y 成为 z 的直接右孩子的新父亲（仍然作为新父亲的右孩子）。
            new_root = transplant(root, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        /// 不论 y 是不是 z 的直接右孩子，我们都需要让 y 取代 z。
        /// 两步走，第一步：将 z 开头的子树全部由 y 开头的新子树代替；第二步：让 z 的左孩子认 y 为新父亲。
        new_root = transplant(root, z, y);
        y->left = z->left;
        z->left->p = y;
    }
    return new_root;
}

/**
 * 接下来的内容不要求撕代码。
 *
 * 二、如果一棵 BST 形如棒状结构，则各种操作的代价将更接近于 O(n) 而非 O(log n)。
 * 因此，通过「左旋」和「右旋」调整保证 BST 的平衡性是很重要的。
 *
 * AVL 树是一棵严格的平衡 BST。其任意节点，左右子树的高度差不超过 1。
 * 红黑树和 SB 树也是平衡 BST。但是对于「平衡」，它们有自己的定义。
 *
 * 三、左旋：当前树根 A 往左边倒，则其右孩子 C 作为新的根。C 的左孩子作为 A 的新的右孩子。
 *
 *          A                               C
 *        /  \                            /  \
 *       B   C       --- 左旋 -->        A    G
 *     / \  / \                         / \
 *    D  E F  G                        B  F
 *                                    /\
 *                                   D E
 *
 * 四、右旋：当前树根 A 往右边倒，则其左孩子 B 作为新的根。
 *
 *          A                               B
 *        /  \                            /  \
 *       B   C       --- 右旋 -->        D    A
 *     / \  / \                             /  \
 *    D  E F  G                            E   C
 *                                            / \
 *                                           F  G
 *
 * 对于不平衡的 BST，左旋和右旋可以让树更平衡一些。
 *
 * 四、现在问题来了，如果判定一棵树是否平衡？AVL 树是这样做的 ——
 * 首先，不平衡只可能是在插入和删除节点时发生。
 *
 * AVL 树在完成节点插入之后，自该节点开始，从下往上检查以该节点为根的子树是否平衡（左右高度 > 1），
 * 如果不平衡，则调用左旋或右旋调整。直到根节点。
 *
 * AVL 树在完成节删除之后，自该节点开始，从下往上检查以该节点为根的子树是否平衡。具体地，如果被删除节点 A 左右孩子均有，
 * 则首先需要调用 transplant 操作保证其仍然为 BST。具体地，假设 F 是 A 的右孩子子树的最左节点，则我们需要用 F 取代 A，
 * 并从 F 的父节点开始往上判断是否平衡并通过左旋/右旋调整。
 *
 * 五、AVL 树不平衡的类型：LL、RR、LR、RL。
 *
 * LL：一次右旋即可。
 *
 *        Z
 *       / \
 *      Y  ^
 *    /  \
 *   X   ^
 *  / \
 * ^  ^
 *
 * RR：一次左旋即可。
 *
 *       Z
 *      / \
 *     ^  Y
 *       / \
 *      ^  X
 *        / \
 *       ^  ^
 *
 * LR：先左旋后右旋。
 *
 *      Z                                      Z
 *    /  \                                   /  \
 *   Y   T4                                 X   T4
 *  / \          -- 左旋 Y 子树 -->        / \           此时和 LL 一样，只需要对 Z 子树右旋即可。
 * T1 X                                  Y  T3
 *   / \                                / \
 *  T2 T3                              T1 T2
 *
 * 同理 RL，此处不在给出。
 *
 * 六、SB 树（https://en.wikipedia.org/wiki/B-tree）
 * 1、平衡标准：每棵子树的大小，不小于以其侄子为根的子树的大小。这里的大小指节点总个数。
 * SB 树的调整需要左旋/右旋结合递归使用。子树大小发生变化的根，需要进一步递归调整。
 *
 * 七、红黑树（https://en.wikipedia.org/wiki/Red–black_tree
 * 定义：
 * （1）每个节点，不是红节点就是黑节点。
 * （2）头节点和最底层的空节点必须是黑节点。
 * （3）任何两个红节点不能相邻；
 * （4）对任意节点，从该节点出发，到叶节点的每一条路径，黑节点数量一样。
 *
 * 因为红黑节点交替，所以第四条可以保证每个子树路径长度差不会超过两倍及以上。
 * */

/**
 * 八、跳表（https://en.wikipedia.org/wiki/Skip_list）
 * 1、跳表的结构如下：
 *
 * struct SkipListNode<K, V> {
 *      K key;  // K is a comparable type
 *      V val;
 *      vector<SkipListNode<K, V> *> nextNodes;
 * }
 *
 * 利用高度跨过很多不必要的节点，从而实现节点插入的加速。
 * */

int main() {
    BinarySearchTree root = new_node(15);
    root->left = new_node(6);
    root->left->p = root;
    root->left->left = new_node(3);
    root->left->left->p = root->left;
    root->left->left->left = new_node(2);
    root->left->left->left->p = root->left->left;
    root->left->left->right = new_node(4);
    root->left->left->right->p = root->left->left;
    root->left->right = new_node(7);
    root->left->right->p = root->left;
    root->left->right->right = new_node(13);
    root->left->right->right->p = root->left->right;
    root->left->right->right->left = new_node(9);
    root->left->right->right->left->p = root->left->right->right;
    root->right = new_node(18);
    root->right->p = root;
    root->right->left = new_node(17);
    root->right->left->p = root->right;
    root->right->right = new_node(20);
    root->right->right->p = root->right;

    in_order(root);
    cout << endl;

    cout << query_recur(root, 20) << " " << query_iter(root, 20) << " " << root->right->right << endl;
    cout << minimum(root)->val << " " << maximum(root)->val << endl;

    cout << successor(root->left->right->right)->val << endl;
    cout << predecessor(root->right->left)->val << endl;

    Node *z1 = insert(root, 12);
    cout << predecessor(z1)->val << " " << successor(z1)->val << endl;

    Node *z2 = insert(nullptr, 1);
    cout << predecessor(z2) << " " << successor(z2) << endl;

    Node *new_root = del(root, root);
    in_order(new_root);
    cout << endl;

    return 0;
}
