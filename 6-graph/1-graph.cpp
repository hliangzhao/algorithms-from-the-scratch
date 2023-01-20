//
// Created by hliangzhao on 19/1/23.
//
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

/**
 * 要点：
 * 1、图的表示（邻接矩阵、邻接表等）
 * 用本文件所示的 Graph 结构实现所有图的算法，遇到具体问题时，只需要写一个接口来适配图的表示即可。
 *
 * 2、图上的 BFS 和 DFS
 *
 * 3、拓扑排序
 *
 * 4、最小生成树（K 算法和 P 算法）
 *
 * 5、Dijkstra 算法求最短路径和
 *
 * TODO：
 *   补充其他图上的基本操作
 * */

struct Node;
struct Edge;
struct Graph;
struct KSets;
struct CmpEdgeByWeight;

Node *new_node(int value);

Edge *new_edge(int weight, Node *src, Node *dst);

void print(Graph *g);

// 适配器（将其他图的表示转换为 Graph 结构）

Graph *adaptor1_1(int **tuples, int size);

Graph *adaptor1_2(int (*tuples)[3], int size);

Graph *adaptor2(int **adj_mat, int size);

// BFS 和 DFS

void bfs(Graph *g, Node *start);

void dfs(Graph *g, Node *start);

// 拓扑排序

vector<Node *> topology_order(Graph *g);

// K 算法求最小生成树

KSets *set_ksets(const vector<Node *> &nodes);

bool is_same_set(KSets *ksets, Node *node1, Node *node2);

void make_union(KSets *ksets, Node *node1, Node *node2);

set<Edge *> kruskal(Graph *g);

// P 算法求最小生成树

set<Edge *> prim(Graph *g);

// Dijkstra 算法求最短路径和

Node *get_min_dis_and_unselected_node(const map<Node *, int>& node2min_dis, const set<Node *>& selected_nodes);

map<Node *, int> dijkstra(Graph *g, Node *start);

struct Node {
    /// 因为 nexts 和 edges 作为在堆上分配的变量，
    /// 我们为下方的内置数据类型添加一个 {}，就能让他们以默认值在堆上创建时被初始化
    int value{};
    int in_dg{};                // 入度
    int out_dg{};               // 出度
    vector<Node *> nexts;       // 从本节点发散出去的节点
    vector<Edge *> edges;       // 从本节点发散出去的边
};

struct Edge {
    int weight;                 // 权值
    Node *src;                  // src node
    Node *dst;                  // dst node
};

struct Graph {
    map<int, Node *> nodes;     // 从节点的值到节点自身的映射
    set<Edge *> edges;          // 边的集合
};

Node *new_node(int value) {
    Node *node = new Node;
    node->value = value;
    node->in_dg = node->out_dg = 0;
    return node;
}

Edge *new_edge(int weight, Node *src, Node *dst) {
    Edge *edge = new Edge;
    edge->weight = weight;
    edge->src = src;
    edge->dst = dst;
    return edge;
}

void print(Graph *g) {
    cout << "------------------ Graph ------------------" << endl;
    cout << "Nodes: " << endl;
    for (auto &node: g->nodes) {
        cout << "\tvalue: " << node.first <<
             ", in degree: " << node.second->in_dg <<
             ", out degree: " << node.second->out_dg << endl;
    }
    cout << "Edges: " << endl;
    for (auto &e: g->edges) {
        cout << "\t(" << e->src->value << ", " << e->dst->value << "), weight: " << e->weight << endl;
    }
    cout << "-------------------------------------------" << endl << endl;
}

/**
 * 将有向图的 [[src_node_value, dst_node_value, edge_weight], ...] 表示方式转换为图
 * */
Graph *adaptor1_1(int **tuples, int size) {
    Graph *g = new Graph;
    for (int i = 0; i < size; i++) {
        int src_val = tuples[i][0], dst_val = tuples[i][1], w = tuples[i][2];
        if (g->nodes.find(src_val) == g->nodes.end()) {
            g->nodes.insert({src_val, new_node(src_val)});
        }
        if (g->nodes.find(dst_val) == g->nodes.end()) {
            g->nodes.insert({dst_val, new_node(dst_val)});
        }
        Node *src_node = g->nodes[src_val];
        Node *dst_node = g->nodes[dst_val];
        // 如果是无向图，下面要多增加几行
        Edge *e = new_edge(w, src_node, dst_node);
        src_node->nexts.push_back(dst_node);
        src_node->out_dg++;
        dst_node->in_dg++;
        src_node->edges.push_back(e);
        g->edges.insert(e);
    }
    return g;
}

/**
 * 将有向图的 [[src_node_value, dst_node_value, edge_weight], ...] 表示方式转换为图
 *
 * 第一个参数还可以写成 int tuples[][3]。即第一维的大小可以省略，但第二维的不可。
 * */
Graph *adaptor1_2(int (*tuples)[3], int size) {
    Graph *g = new Graph;
    for (int i = 0; i < size; i++) {
        int src_val = tuples[i][0], dst_val = tuples[i][1], w = tuples[i][2];
        if (g->nodes.find(src_val) == g->nodes.end()) {
            g->nodes.insert({src_val, new_node(src_val)});
        }
        if (g->nodes.find(dst_val) == g->nodes.end()) {
            g->nodes.insert({dst_val, new_node(dst_val)});
        }
        Node *src_node = g->nodes[src_val];
        Node *dst_node = g->nodes[dst_val];
        Edge *e = new_edge(w, src_node, dst_node);
        src_node->nexts.push_back(dst_node);
        src_node->out_dg++;
        dst_node->in_dg++;
        src_node->edges.push_back(e);
        g->edges.insert(e);
    }
    return g;
}

/**
 * 将邻接矩阵转换为图
 * */
Graph *adaptor2(int **adj_mat, int size) {
    Graph *g = new Graph;
    Node *src, *dst;
    Edge *e;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i != j) {
                /// -1 表示二点之间无边
                if (adj_mat[i][j] == -1) {
                    continue;
                }
                if (g->nodes.find(i) == g->nodes.end()) {
                    src = new_node(i);
                    g->nodes.insert({i, src});
                } else {
                    src = g->nodes.find(i)->second;
                }

                if (g->nodes.find(j) == g->nodes.end()) {
                    dst = new_node(j);
                    g->nodes.insert({j, dst});
                } else {
                    dst = g->nodes.find(j)->second;
                }

                e = new_edge(adj_mat[i][j], src, dst);
                src->nexts.push_back(dst);
                src->out_dg++;
                dst->in_dg++;
                src->edges.push_back(e);
                g->edges.insert(e);
            }
        }
    }
    return g;
}

/**
 * 图上的 BFS：从某个节点出发，按照广度探索所有可以走到的节点并打印。
 * 借助队列实现。
 * 注意，只有没有进过队列的节点才会入队。故而 BFS 还可以用来判断图中是否有环。
 * */
void bfs(Graph *g, Node *start) {
    if (g == nullptr) {
        return;
    }
    queue<Node *> q;
    set<Node *> s;
    q.push(start);
    s.insert(start);
    while (!q.empty()) {
        Node *cur = q.front();
        q.pop();
        cout << cur->value << " ";
        for (auto &next: cur->nexts) {
            if (s.find(next) == s.end()) {
                q.push(next);
                s.insert(next);
            }
        }
    }
    cout << endl;
}

/**
 * 图上的 DFS：从某个节点出发，打印最深路径。
 * 借助栈来实现。进栈的时候处理。
 * */
void dfs(Graph *g, Node *start) {
    if (g == nullptr) {
        return;
    }
    stack<Node *> stk;
    set<Node *> st;
    stk.push(start);
    st.insert(start);
    /// 进栈的时候处理，这样栈中存放的就是从 start 出发最深的路径
    cout << start->value << " ";
    while (!stk.empty()) {
        Node *cur = stk.top();
        stk.pop();
        for (auto &next: cur->nexts) {
            if (st.find(next) == st.end()) {
                /// 先把父节点入栈，这样保证父节点所在路径是最深的
                stk.push(cur);
                stk.push(next);
                st.insert(next);
                cout << next->value << " ";
                /// 但凡找到了一个更深的节点，cur 的其他孩子就不在探索了。从而保证沿着一条路走到黑
                break;
            }
        }
    }
    cout << endl;
}

/**
 * 给定一个无环有向图，返回一条拓扑路径。
 * */
vector<Node *> topology_order(Graph *g) {
    map<Node *, int> remained_in_dg;
    queue<Node *> zero_dg_nodes;
    for (auto &node: g->nodes) {
        remained_in_dg.insert({node.second, node.second->in_dg});
        if (node.second->in_dg == 0) {
            zero_dg_nodes.push(node.second);
        }
    }
    vector<Node *> res;
    while (!zero_dg_nodes.empty()) {
        Node *cur = zero_dg_nodes.front();
        zero_dg_nodes.pop();
        res.push_back(cur);
        /// 下面的 for 循环体和上面的完全一致
        for (auto &next: cur->nexts) {
            /// 下面这行代码出错！因为 C++ 中，insert 时，如果发现重复的 key，则不会 update！
//            remained_in_dg.insert({next, remained_in_dg[next] - 1});
            /// 需要像下面这样手动修改
            remained_in_dg.find(next)->second--;
            if (remained_in_dg[next] == 0) {
                zero_dg_nodes.push(next);
            }
        }
    }
    return res;
}

/**
 * 给定无向连通图，实现 Kruskal 算法（最小生成树 —— 联通加权无向图中权值和最小的一棵树）。
 *
 * 每次加上一条最小的边，看是否成环。
 * 判断是否成环：并查集方法 —— 如果一条边的两个端点不在同一个集合中，则加上本边（不会成环），然后将这两个集合合并。
 * 这在本质上是判断两个子连通图合并之后，是否会成环。
 *
 * 下面用一种简单的方式直接实现这里的"集合"，暂不涉及并查集。
 * */

/**
 * 从每个节点到本节点所在集合的映射。我们用 vector 来模拟集合。
 * */
struct KSets {
    map<Node *, vector<Node *> *> node2set;
};

/**
 * 初始化 KSets。一开始，每个节点各自是一个集合。
 * */
KSets *set_ksets(const vector<Node *> &nodes) {
    auto *ksets = new KSets;
    for (auto &node: nodes) {
        auto *s = new vector<Node *>;
        s->push_back(node);
        ksets->node2set.insert({node, s});
    }
    return ksets;
}

/**
 * 判断 node1 和 node2 各自所属的 set 是否是同一个
 * 判断对应的 set 的指针是否相同
 * */
bool is_same_set(KSets *ksets, Node *node1, Node *node2) {
    vector<Node *> *set_of_node1 = ksets->node2set[node1];
    vector<Node *> *set_of_node2 = ksets->node2set[node2];
    return set_of_node1 == set_of_node2;
}

/**
 * 将 node1 和 node2 所在的集合合并，并更新从这些节点所映射到的集合地址。
 * */
void make_union(KSets *ksets, Node *node1, Node *node2) {
    vector<Node *> *set_of_node1 = ksets->node2set[node1];
    vector<Node *> *set_of_node2 = ksets->node2set[node2];
    for (auto &node: *set_of_node2) {
        set_of_node1->push_back(node);
        /// 将新加入的节点的 set 更新为 set_of_node1
        ksets->node2set.find(node)->second = set_of_node1;
    }
}

/**
 * 使用优先队列存放边。为了使其是小顶堆，自主实现按 weight 升序排列。
 * */
struct CmpEdgeByWeight {
    bool operator()(const Edge *e1, const Edge *e2) {
        return e2->weight < e1->weight;
    }
};

/**
 * 实现 Kruskal 算法返回最小生成树所在的边。
 * */
set<Edge *> kruskal(Graph *g) {
    /// 初始化 KSets
    vector<Node *> all_nodes;
    for (auto &n: g->nodes) {
        all_nodes.push_back(n.second);
    }
    KSets *ksets = set_ksets(all_nodes);

    /// 初始化小顶堆（把全体边扔进去）
    priority_queue<Edge *, vector<Edge *>, CmpEdgeByWeight> pq;
    for (auto &e: g->edges) {
        pq.push(e);
    }

    set<Edge *> res;
    while (!pq.empty()) {
        Edge *e = pq.top();
        pq.pop();
        if (!is_same_set(ksets, e->src, e->dst)) {
            res.insert(e);
            make_union(ksets, e->src, e->dst);
        }
    }

    return res;
}

/**
 * 实现 Prim 算法返回最小生成树所在的边。
 * Prim 算法以节点为中心，不断解锁相邻边并从中挑选不成环的（该边的 dst 端点不可以在节点集合中）、权值最小的边。
 * */
set<Edge *> prim(Graph *g) {
    /// 初始化小顶堆（存放所有解锁的边）
    priority_queue<Edge *, vector<Edge *>, CmpEdgeByWeight> pq;
    /// st 存放已经考察过的点
    set<Node *> st;
    set<Edge *> res;

    /// 如果输入的图由多个连通子图组成，并且这些连通子图之间互不连通，则这个 for 循环可以返回全部连通子图的最小生成树的边集。
    /// 如果输入的图就是一个单一的连通图，这个 for 循环可以去掉！
    for (auto &node: g->nodes) {
        Node *cur = node.second;
        if (st.find(cur) == st.end()) {
            /// 将当前节点加入探索过的节点集合 st 并将其所关联的所有边加入候选队列中
            st.insert(cur);
            for (auto &e: cur->edges) {
                pq.push(e);
            }
            while (!pq.empty()) {
                /// 每次弹出一条权值最小的边，如果该边会导致形成环，扔了不管；
                /// 否则将该边加入结果集，并
                /// （1）将该边的 dst 节点加入探索过的点集；
                /// （2）将 dst 所指向的新的边们加入候选队列
                /// 从一条边的两个端点出发，会得到两条边：(n1, n2) 和 (n2, n1)，
                /// 它们都会被加入优先队列中。然而，这不影响结果 —— 这是因为借助 st，重复的边不可能被选中，会直接被弹出并跳过。
                Edge *e = pq.top();
                pq.pop();
                if (st.find(e->dst) == st.end()) {
                    st.insert(e->dst);
                    res.insert(e);
                    for (auto *next_e: e->dst->edges) {
                        pq.push(next_e);
                    }
                }
            }
        }
    }
    return res;
}

/**
 * 实现 Dijkstra 算法返回从给定节点到途中任意其他点的最短路径和。
 * 要求给定图没有权值为负数的边（实际上是要求不能有累加权值和为负数的环），否则会导致每转一圈路径和就减小，会无限循环。
 * */

/**
 * Dijkstra 算法的辅助算法：从最短距离表中选择一个距离最小的、尚未被 confirmed 的节点。
 * 这个子程序如果要用小根堆来实现，则需要自己实现堆，而不应该直接采用 STL 提供的优先队列，否则效率很低。
 * */
Node *get_min_dis_and_unselected_node(const map<Node *, int>& node2min_dis, const set<Node *>& selected_nodes) {
    int min_dis = INT_MAX;
    Node *res = nullptr;
    for (auto &node: node2min_dis) {
        if (selected_nodes.find(node.first) == selected_nodes.end() && node.second < min_dis) {
            min_dis = node.second;
            res = node.first;
        }
    }
    return res;
}

map<Node *, int> dijkstra(Graph *g, Node *start) {
    /// 存放从 start 到任意点的最短距离（如果 key 不在其中，则意味着到目前位置，node --> key 的最短距离尚为无穷大）
    map<Node *, int> node2min_dis;
    node2min_dis.insert({start, 0});
    /// 存放已经确认了最短距离的节点
    set<Node *> confirmed;
    Node *min_node = get_min_dis_and_unselected_node(node2min_dis, confirmed);
    while (min_node != nullptr) {
        int dis = node2min_dis[min_node];
        for (auto &e: min_node->edges) {
            Node *dst_node = e->dst;
            if (node2min_dis.find(dst_node) == node2min_dis.end()) {
                /// 新增（从正无穷改为某个数值）
                node2min_dis.insert({dst_node, dis + e->weight});
            } else {
                /// 更新
                node2min_dis[dst_node] = min(node2min_dis[dst_node], dis + e->weight);
            }
        }
        confirmed.insert(min_node);
        min_node = get_min_dis_and_unselected_node(node2min_dis, confirmed);
    }
    return node2min_dis;
}

int main() {
    /// 二维数组的两种创建方式
    int **tuples1 = new int *[5];
    for (int i = 0; i < 5; i++) {
        tuples1[i] = new int[3];
    }
    tuples1[0][0] = 1;
    tuples1[0][1] = 3;
    tuples1[0][2] = 2;
    tuples1[1][0] = 1;
    tuples1[1][1] = 4;
    tuples1[1][2] = 4;
    tuples1[2][0] = 1;
    tuples1[2][1] = 2;
    tuples1[2][2] = 10;
    tuples1[3][0] = 4;
    tuples1[3][1] = 2;
    tuples1[3][2] = 5;
    tuples1[4][0] = 3;
    tuples1[4][1] = 4;
    tuples1[4][2] = 6;
    Graph *g1 = adaptor1_1(tuples1, 5);
    print(g1);

    int tuples2[8][3] = {
            {1,  3,  2},
            {1,  4,  4},
            {1,  2,  10},
            {4,  2,  5},
            {3,  4,  6},
            {2,  9,  11},
            {2,  10, 1},
            {10, 4,  100}
    };
    Graph *g2 = adaptor1_2(tuples2, 8);
    print(g2);

    bfs(g2, g2->nodes.begin()->second);
    dfs(g2, g2->nodes.begin()->second);

    int tuples3[8][3] = {
            {1, 2, 1},
            {2, 3, 1},
            {2, 4, 1},
            {1, 4, 1},
            {3, 5, 1},
            {4, 5, 1},
            {3, 7, 1},
            {5, 6, 1}
    };
    Graph *g3 = adaptor1_2(tuples3, 8);
    print(g3);
    vector<Node *> to3 = topology_order(g3);
    for (auto &e: to3) {
        cout << e->value << " ";
    }
    cout << endl;

    int tuples4[10][3] = {
            {1, 2, 1},
            {2, 3, 1},
            {2, 4, 1},
            {1, 4, 1},
            {3, 5, 1},
            {4, 5, 1},
            {3, 7, 1},
            {5, 6, 1},
            {8, 4, 1},
            {8, 5, 1}
    };
    Graph *g4 = adaptor1_2(tuples4, 10);
    print(g4);
    vector<Node *> to4 = topology_order(g4);
    for (auto &e: to4) {
        cout << e->value << " ";
    }
    cout << endl;

    int **adj_mat1 = new int *[4];
    for (int i = 0; i < 4; i++) {
        adj_mat1[i] = new int[4];
    }
    adj_mat1[0][0] = adj_mat1[1][1] = adj_mat1[2][2] = adj_mat1[3][3] = 1;
    adj_mat1[1][0] = adj_mat1[0][1] = 3;
    adj_mat1[2][0] = adj_mat1[0][2] = 7;
    adj_mat1[3][0] = adj_mat1[0][3] = 100;
    adj_mat1[2][1] = adj_mat1[1][2] = 2;
    adj_mat1[3][1] = adj_mat1[1][3] = 5;
    adj_mat1[3][2] = adj_mat1[2][3] = 1000;
    Graph *g5 = adaptor2(adj_mat1, 4);
    print(g5);
    set<Edge *> min_span_tree5 = kruskal(g5);
    for (auto &e:min_span_tree5){
        cout << "(" << e->src->value << ", " << e->dst->value << "), weight: " << e->weight << endl;
    }
    cout << endl;

    int **adj_mat2 = new int *[6];
    for (int i = 0; i < 6; i++) {
        adj_mat2[i] = new int[6];
    }
    adj_mat2[0][0] = adj_mat2[1][1] = adj_mat2[2][2] = adj_mat2[3][3] = adj_mat2[4][4] = adj_mat2[5][5] = 1;
    adj_mat2[1][0] = adj_mat2[0][1] = adj_mat2[2][4] = adj_mat2[4][2] = adj_mat2[5][4] = adj_mat2[4][5] = 6;
    adj_mat2[2][0] = adj_mat2[0][2] = 1;
    adj_mat2[3][0] = adj_mat2[0][3] = adj_mat2[2][1] = adj_mat2[1][2] = 5;
    adj_mat2[4][0] = adj_mat2[0][4] = adj_mat2[5][0] = adj_mat2[0][5] =
    adj_mat2[1][3] = adj_mat2[3][1] = adj_mat2[1][5] = adj_mat2[5][1] =
    adj_mat2[4][3] = adj_mat2[3][4] = -1;
    adj_mat2[2][1] = adj_mat2[1][2] = 5;
    adj_mat2[3][5] = adj_mat2[5][3] = 2;
    adj_mat2[3][1] = adj_mat2[1][3] = -1;
    adj_mat2[2][3] = adj_mat2[3][2] = 5;
    adj_mat2[1][4] = adj_mat2[4][1] = 3;
    adj_mat2[2][5] = adj_mat2[5][2] = 4;

    Graph *g6 = adaptor2(adj_mat2, 6);
    print(g6);
    set<Edge *> k6 = kruskal(g6);
    for (auto &e: k6) {
        cout << "(" << e->src->value << ", " << e->dst->value << "), weight: " << e->weight << endl;
    }
    cout << endl;
    set<Edge *> p6 = prim(g6);
    for (auto &e: p6) {
        cout << "(" << e->src->value << ", " << e->dst->value << "), weight: " << e->weight << endl;
    }
    cout << endl;

    Node *start = g6->nodes.at(0);
    map<Node *, int> min_dis = dijkstra(g6, start);
    cout << "start node: " << start->value << endl;
    for (auto &n2d: min_dis) {
        cout << "end node: " << n2d.first->value << ", dis: " << n2d.second << endl;
    }
    cout << endl;

    return 0;
}