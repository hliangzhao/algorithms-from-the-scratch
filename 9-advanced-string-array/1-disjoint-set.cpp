//
// Created by hliangzhao on 26/1/23.
//
#include <iostream>
#include <map>
#include <vector>
#include <stack>

using namespace std;

/**
 * 要点：由一个简单的 DFS 题目引出并查集。
 * */

/**
 * 问题一：01 岛问题。给定一个 01 二维数组，上下左右连成一片的 1 代表一个岛，返回岛的个数。
 * 解：每找到一个 1，就 DFS 找到临近的全部 1，并将这些 1 全部改为 2（或者别的数字），DFS 结束返回就意味着找到了一个岛。
 * 之所以要修改其值是为了放置无限递归（剪枝）——已经探索过的位置不应当再探索。
 *
 * 虽然还有递归，但是该方法的时间复杂度为 O (MN)，因为每个位置最多被调用 5 次。
 * */

void dfs(int **arr, int line_num, int col_num, int i, int j);

int island(int **arr, int line_num, int col_num);

/**
 * dfs 中，一个位置最多被调用 4 次
 * */
void dfs(int **arr, int line_num, int col_num, int i, int j) {
    arr[i][j] = 2;
    if (i > 0 && arr[i - 1][j] == 1) {
        dfs(arr, line_num, col_num, i - 1, j);
    }
    if (i < line_num - 1 && arr[i + 1][j] == 1) {
        dfs(arr, line_num, col_num, i + 1, j);
    }
    if (j > 0 && arr[i][j - 1] == 1) {
        dfs(arr, line_num, col_num, i, j - 1);
    }
    if (j < col_num - 1 && arr[i][j + 1] == 1) {
        dfs(arr, line_num, col_num, i, j + 1);
    }
}

/**
 * 把判断的操作放在最前，从而简化代码
 * */
void dfs2(int **arr, int line_num, int col_num, int i, int j) {
    /// 注意，根据短路原则，arr[i][j] != 1 应当放在最后，否则访问非法空间
    if (i < 0 || i >= line_num || j < 0 || j >= col_num || arr[i][j] != 1) {
        return;
    }
    arr[i][j] = 2;
    dfs2(arr, line_num, col_num, i - 1, j);
    dfs2(arr, line_num, col_num, i + 1, j);
    dfs2(arr, line_num, col_num, i, j - 1);
    dfs2(arr, line_num, col_num, i, j + 1);
}

int island(int **arr, int line_num, int col_num) {
    int res = 0;
    for (int i = 0; i < line_num; i++) {
        for (int j = 0; j < col_num; j++) {
            if (arr[i][j] == 1) {
                dfs(arr, line_num, col_num, i, j);
                res++;
            }
        }
    }
    return res;
}

/**
 * 并查集：对于一个集合，我们需要提供两个方法：is_same_set() 和 union()。
 * 如果集合采用链表来实现，则 union 操作复杂度为 O(1)，但 is_same_set 不是；
 * 如果采用哈希表来实现，则 is_same_set 操作复杂度为 O(1)，但 union 不是。
 *
 * 并查集的底层数据结构是树，可以让二者都很快。首先，每个元素放到一个集合中，该集合仅包含该元素本身。
 * is_same_set 通过判断两个集合是不是一个爹生的来实现（判断父指针指向节点是否相同）。
 * 注意，找到之后把让沿途节点的指针全部指向最顶端祖先节点，从而解决链过长的问题；
 * union 通过将小集合的父指针指向另一个集合来实现。
 *
 * 下面给出并查集的实现。
 *
 * 设一共有 n 个样本，则 find_head() 的调用次数达到 O(n) 时，单次 find_head() 的时间复杂度为 O(1)。
 * 调用次数越多，开下越低（得益于扁平化操作）。
 * */

template<typename elem_type>
struct Node {
    elem_type value;
};

template<typename elem_type>
Node<elem_type> *new_node(elem_type element) {
    Node<elem_type> node = new Node<elem_type>;
    node.value = element;
    return node;
}

template<typename elem_type>
struct DisjointSet {
    map<elem_type, Node<elem_type> *> element2node;         /// 元素到 Node 的映射
    map<Node<elem_type> *, Node<elem_type> *> node2father;  /// Node 到父节点的映射
    map<Node<elem_type> *, int> node2size;                  /// Node（作为集合/代表元素）中所包含的元素个数
};

template<typename elem_type>
DisjointSet<elem_type> *new_disjoint_set(const vector<elem_type> &vec) {
    auto *set = new DisjointSet<elem_type>;
    for (auto &element: vec) {
        Node<elem_type> *node = new_node(element);
        set->element2node.insert({element, node});
        set->node2father.insert({node, node});
        set->node2size.insert({node, 1});
    }
}

/**
 * 往上找的同时进行扁平化操作：把所有最顶父指针相同的节点的父指针直接置为父指针
 * */
template<typename elem_type>
Node<elem_type> *find_head(DisjointSet<elem_type> *set, Node<elem_type> *node) {
    stack<Node<elem_type> *> stk;
    while (node != set->node2father[node]) {
        stk.push(node);
        node = set->node2father[node];
    }
    while (!stk.empty()) {
        Node<elem_type> *cur = stk.top();
        set->node2father[cur] = node;
        stk.pop();
    }
    return node;
}

template<typename elem_type>
bool is_same_set(DisjointSet<elem_type> *set, elem_type a, elem_type b) {
    if (set->element2node.find(a) != set->element2node.end() && set->element2node.find(b) != set->element2node.end()) {
        return find_head(set, set->element2node[a]) == find_head(set->element2node[b]);
    }
    return false;
}

template<typename elem_type>
bool make_union(DisjointSet<elem_type> *set, elem_type a, elem_type b) {
    if (set->element2node.find(a) != set->element2node.end() && set->element2node.find(b) != set->element2node.end()) {
        Node<elem_type> *a_father_node = find_head(set, set->element2node[a]);
        Node<elem_type> *b_father_node = find_head(set, set->element2node[b]);
        if (a_father_node != b_father_node) {
            Node<elem_type> big =
                    set->node2size[a_father_node] >= set->node2size[b_father_node] ? a_father_node : b_father_node;
            Node<elem_type> small = big == a_father_node ? b_father_node : a_father_node;
            /// 把小的挂在大的后面
            set->node2father[small] = big;
            set->node2size[big] = set->node2size[a_father_node] + set->node2size[b_father_node];
            set->node2size.erase(small);
        }
    }
}

/**
 * 基于并查集设计并行算法解决 01 岛问题：借助并查集进行 merge。
 * 不妨设有两个进程，他们各自处理该 01 矩阵的左右半边。中间切一刀的位置使得左半部分最右列和右半部分最左列成为了边界。
 * 首先，分治。各个进程分别基于 dfs 找到各自负责区域中岛的个数，记录每个边界上值为 1 的点的源头 —— 它是被谁「感染」的。
 * 其次，合并。显然，每一个岛都会对应一个感染点，将这些感染点各自建立集合（new_disjoint_set）。
 * 依次遍历每一对边界点，如果二者都是 1，首先观察对应的两个感染点是否在同一个集合中，若是，则什么也不做；
 * 否则则岛个数--，调用 make_union 将二者所关联的感染点集合合并。
 *
 * 最后将返回真正的岛个数，且每个岛对应一个集合，该集合存放了不同感染点（如果没有采用并行算法，那么这些感染点将退化为一个感染点）。
 *
 * 如果分割为多个区域，各个区域和相邻的区域合并即可。
 * */

int main() {
    int arr[4][5] = {
            {0, 1, 0, 0, 1},
            {1, 1, 0, 0, 1},
            {0, 0, 1, 0, 0},
            {1, 0, 0, 0, 1},
    };
    int **arr2 = new int *[4];
    for (int i = 0; i < 4; i++) {
        arr2[i] = new int[5];
        for (int j = 0; j < 5; j++) {
            arr2[i][j] = arr[i][j];
        }
    }
    cout << island(arr2, 4, 5) << endl;
    return 0;
}
