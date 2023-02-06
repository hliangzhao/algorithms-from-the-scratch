//
// Created by hliangzhao on 5/2/23.
//
#include <iostream>
#include <queue>
#include <map>

using namespace std;

/**
 * 题目一：给定字符串 s 和 m 初始化为：s = "a"; m = s;
 * 再定义两种操作，第一种操作为：
 * m = s;
 * s = s + s;
 * 第二种操作为：
 * s = s + m;
 *
 * 求最小的操作步骤数，使得 s 的长度等于 n。
 *
 * 令 min_op 表示：从当前 s 和 m 出发，经过操作 1 或操作 2 使得 s 最终长度为 n 的最小操作数。
 * */
int min_op(string &s, string &m, int n) {
    if (s.size() > n) {
        return -1;
    } else if (s.size() == n) {
        return 0;
    }

    /// 操作 1
    string s_copy(s), m_copy(m);
    m_copy = s_copy;
    s_copy = s_copy + s_copy;

    /// 操作 2
    s = s + m;

    int res1 = min_op(s_copy, m_copy, n);
    int res2 = min_op(s, m, n);
    if (res1 == -1 && res2 == -1) {
        return -1;
    } else if (res1 == -1) {
        return 1 + res2;
    } else if (res2 == -1) {
        return 1 + res1;
    } else {
        int res = min(res1, res2);
        return 1 + res;
    }
}

/**
 * 方法二：从业务分析的角度出发进行思考。
 *
 * 如果 n 是质数，则只调用 n-1 次操作 2，就是最终解答（第一步可以调用操作 1 或操作 2，但后续操作必须是操作 2）。
 * 如果 n 不是质数，则 n 能够以唯一的方式表示成它的质因数的乘积。不妨设 n = a*b*c*d*...，
 * 则这些 a、b、c、d 的每种组合中使得结果最小的就是答案。
 * */

void sum_and_cnt(int n, int &sum, int &cnt) {
    sum = 0;
    cnt = 0;
    for (int i = 2; i <= n; i++) {
        while (n % i == 0) {
            sum += i;
            cnt++;
            n /= i;
        }
    }
}

bool prime(int n) {
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int min_op2(int n) {
    if (n < 2) {
        return 0;
    }
    if (prime(n)) {
        return n - 1;
    }
    int sum, cnt;
    sum_and_cnt(n, sum, cnt);
    return sum - cnt;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目二：给定一个字符串类型的数组，求其中出现次数最多的前 k 个字符串。
 *
 * 哈希表 + 大根堆
 * */
struct Cmp4BigRootHeap {
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2) {
        return p2.second > p1.second;
    }
};

void top_k(const string *arr, int size, int k) {
    map<string, int> m;
    for (int i = 0; i < size; i++) {
        if (m.find(arr[i]) == m.end()) {
            m[arr[i]] = 1;
        } else {
            m[arr[i]]++;
        }
    }
    priority_queue<pair<string, int>, vector<pair<string, int>>, Cmp4BigRootHeap> pq;
    for (auto &kv: m) {
        pq.emplace(kv.first, kv.second);
    }
    int out = 0;
    while (!pq.empty()) {
        if (out == k) {
            break;
        }
        cout << pq.top().first << ": " << pq.top().second << endl;
        pq.pop();
        out++;
    }
    cout << endl;
}

/**
 * 方法二：使用小根堆来解决。设定一个大小为 k 的小根堆，堆顶是门槛。
 * */

struct Cmp4SmallRootHeap {
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2) {
        return p2.second < p1.second;
    }
};

void top_k2(const string *arr, int size, int k) {
    map<string, int> m;
    for (int i = 0; i < size; i++) {
        if (m.find(arr[i]) == m.end()) {
            m[arr[i]] = 1;
        } else {
            m[arr[i]]++;
        }
    }
    priority_queue<pair<string, int>, vector<pair<string, int>>, Cmp4SmallRootHeap> pq;
    for (auto &kv: m) {
        if (pq.size() >= k && kv.second > pq.top().second) {
            /// 超过了 size，且更大的时候，替换掉堆顶（新的守门员）
            pq.pop();
            pq.emplace(kv.first, kv.second);
        } else {
            /// 超过了 size，但是不够大，则不进堆；没超过 size，不管大不大都可以进堆
            if (pq.size() < k) {
                pq.emplace(kv.first, kv.second);
            }
        }
    }
    while (!pq.empty()) {
        cout << pq.top().first << ": " << pq.top().second << endl;
        pq.pop();
    }
    cout << endl;
}

// --------------------------------------------------------------------------------------------------------------

/**
 * 题目三：整体内容和题目二类似，但是要自主实现一个结构，要求随时能够以较低的代价调用如下两个方法：
 * add(str);
 * void print_top_k().
 *
 * 词频表；
 * 按词频统计结果的小根堆；
 * 词到堆位置 index 的 map。
 * */
struct Node {
    string word;
    int freq;
};

Node *new_node(const string &w, int f) {
    auto *wf = new Node;
    wf->word = w;
    wf->freq = f;
    return wf;
}

struct TopKRec {
    Node **heap{};
    int heap_max_size{};
    int heap_cur_size{};
    map<string, Node *> str2node;
    /// node 是否在堆上。无记录或有记录但是 -1：不在堆上
    map<Node *, int> node2idx;
};

TopKRec *new_rec(int size) {
    auto *rec = new TopKRec;
    rec->heap = new Node *[size];
    rec->heap_max_size = size;
    rec->heap_cur_size = 0;
    return rec;
}

void swap(TopKRec *rec, int &idx1, int &idx2) {
    /// 先交换表，后交换堆
    rec->node2idx[rec->heap[idx1]] = idx2;
    rec->node2idx[rec->heap[idx2]] = idx1;

    Node *tmp = rec->heap[idx1];
    rec->heap[idx1] = rec->heap[idx2];
    rec->heap[idx2] = tmp;
}

/**
 * 将 rec->heap[index] 插入到小根堆的合适位置上。
 * */
void heap_insert(TopKRec *rec, int index) {
    while (index != 0) {
        int parent = (index - 1) / 2;
        if (rec->heap[index]->freq < rec->heap[parent]->freq) {
            swap(rec, parent, index);
            index = parent;
        } else {
            break;
        }
    }
}

/**
 * 将 rec->heap[index] 自顶向下调整到小根堆合适的位置上。
 * */
void heapify(TopKRec *rec, int index, int heap_size) {
    int left = 2 * index + 1;
    while (left < heap_size) {
        int smallest = (left + 1 < heap_size && rec->heap[left + 1]->freq < rec->heap[left]->freq) ? left + 1 : left;
        smallest = rec->heap[smallest]->freq < rec->heap[index]->freq ? smallest : index;
        if (smallest == index) {
            break;
        }
        swap(rec, smallest, index);
        index = smallest;
        left = 2 * index + 1;
    }
}

void add(TopKRec *rec, const string &str) {
    Node *cur = nullptr;
    int pre_idx = -1;
    if (rec->str2node.find(str) == rec->str2node.end()) {
        /// 没有记录
        cur = new_node(str, 1);
        rec->str2node.insert({str, cur});
        rec->node2idx.insert({cur, -1});
    } else {
        /// 有记录
        cur = rec->str2node[str];
        cur->freq++;
        pre_idx = rec->node2idx[cur];
    }
    if (pre_idx == -1) {
        /// 没进过堆
        if (rec->heap_cur_size == rec->heap_max_size) {
            /// 是因为堆满了
            if (rec->heap[0]->freq < cur->freq) {
                /// 大于守门员则替换之
                rec->node2idx[rec->heap[0]] = -1;
                rec->node2idx[cur] = 0;
                rec->heap[0] = cur;
                heapify(rec, 0, rec->heap_cur_size);
            }
        } else {
            /// 堆没满，单纯第一次遇到该字符串而已
            rec->node2idx[cur] = rec->heap_cur_size;
            rec->heap[rec->heap_cur_size] = cur;
            heap_insert(rec, rec->heap_cur_size++);
        }
    } else {
        heapify(rec, pre_idx, rec->heap_cur_size);
    }
}

void print_topk(TopKRec *rec) {
    for (int i = 0; i < rec->heap_cur_size; i++) {
        if (rec->heap[i] == nullptr) {
            break;
        }
        cout << rec->heap[i]->word << ": " << rec->heap[i]->freq << endl;
    }
    cout << endl;
}

int main() {
//    for (int i = 4; i <= 40; i++) {
//        string s = "a", m = s;
//        cout << min_op(s, m, i) << " " << min_op2(i) << endl;
//    }

//    int size = 13;
//    auto *arr = new string[size];
//    arr[0] = "abc";
//    arr[1] = "abc";
//    arr[2] = "bck";
//    arr[3] = "efg";
//    arr[4] = "efg";
//    arr[5] = "efg";
//    arr[6] = "bck";
//    arr[7] = "xyz";
//    arr[8] = "efg";
//    arr[9] = "efg";
//    arr[10] = "bck";
//    arr[11] = "xyz";
//    arr[12] = "xyz";
//    top_k(arr, size, 2);
//    top_k2(arr, size, 2);

    TopKRec *rec = new_rec(3);
    add(rec, "abc");
    print_topk(rec);
    add(rec, "abc");
    print_topk(rec);
    add(rec, "bck");
    print_topk(rec);
    add(rec, "efg");
    print_topk(rec);
    add(rec, "efg");
    print_topk(rec);
    add(rec, "efg");
    print_topk(rec);
    add(rec, "bck");
    print_topk(rec);
    add(rec, "xyz");
    print_topk(rec);
    add(rec, "efg");
    print_topk(rec);
    add(rec, "efg");
    print_topk(rec);
    add(rec, "bck");
    print_topk(rec);
    add(rec, "xyz");
    print_topk(rec);
    add(rec, "xyz");
    print_topk(rec);

    return 0;
}
