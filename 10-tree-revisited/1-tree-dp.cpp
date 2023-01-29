//
// Created by hliangzhao on 28/1/23.
//
#include <iostream>
#include <vector>

using namespace std;

/**
 * 树型 DP 的使用：如果题目求解目标是 S 规则。则求解流程为每个节点为头节点的子树在 S 规则下从左右子树要信息，得到一个答案。
 * 最终答案一定在其中。
 * */

struct Node {
    int val;
    Node *left;
    Node *right;
};

Node *new_node(int val) {
    auto *node = new Node;
    node->val = val;
    node->left = node->right = nullptr;
    return node;
}

/**
 * 问题一：求二叉树节点间的最大距离。
 * 从二叉树的节点 A 出发，可以向上走或者向下走，但沿途的节点只能经过一次，到达节点 b 时路径上的节点个数叫做 a 到 b 的距离。
 * 求整棵树上的最大距离。
 *
 * 按照头节点是否参与进行分类：
 * 对于以 root 为根的子树，它的最大距离按照 root 是否参与可以划分为两类：
 * （1）root 参与。则其最大距离为：左子树的深度 + 右子树的深度 + 1（从左边最深的叶子一路网上走到 root，再一路往下走到右边最深的位置）。
 * （2）root 不参与。则其最大距离为：左子树的最大距离或者右子树的最大距离。
 * 因此，一共有三种可能性。并且，每棵树向其左右子树所要的信息为最大距离和最大深度。
 * */

int max_dis(Node *root, int &depth) {
    if (root == nullptr) {
        depth = 0;
        return 0;
    }
    int depth_left, depth_right;
    int res;
    if (root->left == nullptr && root->right == nullptr) {
        depth = 1;
        return 1;
    }
    if (root->left != nullptr && root->right == nullptr) {
        res = max_dis(root->left, depth_left);
        res = max(res, depth_left + 1);
        depth_right = 0;
    }
    if (root->left == nullptr && root->right != nullptr) {
        res = max_dis(root->right, depth_right);
        res = max(res, depth_right + 1);
        depth_left = 0;
    }
    if (root->left != nullptr && root->right != nullptr) {
        res = max(max_dis(root->left, depth_left), max_dis(root->right, depth_right));
        res = max(res, depth_left + depth_right + 1);
    }
    depth = max(depth_left, depth_right) + 1;
    return res;
}

/**
 * 问题一更加规范简洁的处理方式
 * */

struct Info {
    int max_dis;
    int height;
};

Info *new_info(int d, int h) {
    auto *info = new Info;
    info->max_dis = d;
    info->height = h;
    return info;
}

Info *max_dis2(Node *root) {
    if (root == nullptr) {
        return new_info(0, 0);
    }
    Info *left_info = max_dis2(root->left);
    Info *right_info = max_dis2(root->right);
    int p1 = left_info->max_dis, p2 = right_info->max_dis;
    int p3 = left_info->height + right_info->height + 1;
    int max_dis = max(p3, max(p1, p2));
    int height = max(left_info->height, right_info->height) + 1;
    return new_info(max_dis, height);
}

/**
 * 问题二：给定一颗多叉树，我们试图从中选择一些节点，使得这些节点的 value 和最大。
 * 做选择时，如果一个节点被选中，则它的所有孩子节点都不能被选中。
 * 返回最大 value 和。
 *
 * f(n) = max(
 *      n.val + \sum_{m=n.child} \sum_{p=m.child} f(p),
 *      \sum_{m=n.child} f(m)
 * )
 * */

struct Node2 {
    int value{};
    vector<Node2 *> children;
};

Node2 *new_node2(int v) {
    auto *node = new Node2;
    node->value = v;
    return node;
}

int max_val_sum(Node2 *root) {
    if (root == nullptr) {
        return 0;
    }
    /// root 被选中了，此时 root 的孩子一定不能被选中，但是它的孙子可以
    int max1 = root->value;
    Node2 *node = root;
    for (Node2 *node_child: node->children) {
        for (Node2 *node_grand_child: node_child->children) {
            max1 += max_val_sum(node_grand_child);
        }
    }
    /// root 没有被选中，此时 root 的孩子可以被选中
    int max2 = 0;
    for (Node2 *node_child: node->children) {
        max2 += max_val_sum(node_child);
    }
    return max(max1, max2);
}

/**
 * 问题二的另一种处理方式，仍然将「向子树索要的结果」打包成结构体
 * */

struct Info2 {
    int chosen;
    int non_chosen;
};

Info2 *new_info2(int c, int n) {
    auto *info = new Info2;
    info->chosen = c;
    info->non_chosen = n;
    return info;
}

Info2 *max_val_sum2(Node2 *root) {
    if (root->children.empty()) {
        return new_info2(root->value, 0);
    }
    int chosen = root->value;
    int non_chosen = 0;
    for (auto *child: root->children) {
        auto info2 = max_val_sum2(child);
        /// root 被选中，则只能加上孩子不能被选中的结果
        chosen += info2->non_chosen;
        /// root 没有被选中，则孩子可以被选中，也可以不被选中。我们要较大的
        non_chosen += max(info2->chosen, info2->non_chosen);
    }
    return new_info2(chosen, non_chosen);
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

    int depth;
    cout << max_dis(tree1, depth) - 1 << endl;
    cout << max_dis2(tree1)->max_dis - 1 << endl;

    Node2 *tree2 = new_node2(10);
    tree2->children.push_back(new_node2(3));
    tree2->children.push_back(new_node2(20));
    tree2->children.push_back(new_node2(40));
    tree2->children[0]->children.push_back(new_node2(60));
    tree2->children[0]->children.push_back(new_node2(3));
    tree2->children[2]->children.push_back(new_node2(5));
    tree2->children[2]->children.push_back(new_node2(6));
    cout << max_val_sum(tree2) << endl;
    auto *info2 = max_val_sum2(tree2);
    cout << max(info2->chosen, info2->non_chosen) << endl;

    return 0;
}
