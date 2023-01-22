//
// Created by hliangzhao on 20/1/23.
//
#include <iostream>
#include <vector>

using namespace std;

/**
 * 要点：使用前缀树解决公共前缀子串的问题
 * */

const int CHAR_NUM = 26;

struct TrieNode;

TrieNode *new_trie_node();

void print(TrieNode *root);

void print_proc(TrieNode *root, int tab_num);

void insert(TrieNode *root, const string &word);

int query_str(TrieNode *root, const string &word);

int query_prefix(TrieNode *root, const string &prefix);

void release(TrieNode *node);

void del(TrieNode *root, const string &word);

/**
 * TrieNode 表示一个节点，本节点延伸出去的边代表一个字符，用长度为 26 的 nexts 数组的 index 表示。
 * */
struct TrieNode {
    int pass;                   // 从本节点经过的字符串的个数
    int end;                    // 以本节点作为结尾的字符串的个数
    // 如果某些字符串的字符用到了本节点，将这些字符作为边，边的 dst 节点挂到本节点的 nexts 数组中
    /// 如果字符串不仅仅由 26 个字母组成，还包括别的字符，则可以将 nexts 换成 map
//    map<char, TrieNode *> nexts;
    TrieNode *nexts[CHAR_NUM];
};

TrieNode *new_trie_node() {
    auto *node = new TrieNode;
    node->pass = node->end = 0;
    for (auto &next: node->nexts) {
        next = nullptr;
    }
    return node;
}

void print(TrieNode *root) {
    print_proc(root, 0);
    cout << endl;
}

void print_proc(TrieNode *root, int tab_num) {
    for (int i = 0; i < tab_num; i++) {
        cout << "|----";
    }
    cout << "pass: " << root->pass << ", end: " << root->end << endl;
    for (auto &next: root->nexts) {
        if (next != nullptr) {
            print_proc(next, tab_num + 1);
        }
    }
}

void insert(TrieNode *root, const string &word) {
    if (word.empty()) {
        /// 加入了一个空串，空串以 root 收尾，因此 root->end++
        root->end++;
        return;
    }
    /// C++ 中，将字符串转换为字符数组（注意长度要 +1，留给 '\0'）
    /// 实际上并不需要转换为字节数组，因为 C++ 中 string 可以直接被遍历（和容器一样）
//    char c_arr[word.length() + 1];
//    ::strcpy(c_arr, word.data());
    TrieNode *node = root;
    /// word 会从根出发，传进来一个 word，根的 pass 首先 ++
    node->pass++;

    /// 不断往下走，创建节点（如需要）并修改字段
    for (auto &c: word) {
        int idx = c - 'a';
        if (node->nexts[idx] == nullptr) {
            node->nexts[idx] = new_trie_node();
        }
        node = node->nexts[idx];
        node->pass++;
    }
    node->end++;
}

/**
 * 查询 word 这个字符串出现过几次
 * */
int query_str(TrieNode *root, const string &word) {
    /// 无需判断 null string，这在 C++ 中不存在。

    TrieNode *node = root;
    for (auto &c: word) {
        int idx = c - 'a';
        if (node->nexts[idx] == nullptr) {
            return 0;
        }
        node = node->nexts[idx];
    }
    return node->end; /// 空串将返回 root->end
}

/**
 * 查询 prefix 这个前缀出现过几次
 * */
int query_prefix(TrieNode *root, const string &prefix) {
    /// 无需判断 null string，这在 C++ 中不存在。

    TrieNode *node = root;
    for (auto &c: prefix) {
        int idx = c - 'a';
        if (node->nexts[idx] == nullptr) {
            return 0;
        }
        node = node->nexts[idx];
    }
    return node->pass; /// 空串将返回 root->pass
}

/**
 * 借助递归删除「从叶子节点到 node」的内存占用。
 * */
void release(TrieNode *node) {
    for (auto &next: node->nexts) {
        if (next != nullptr) {
            release(next);
            next = nullptr;
        }
    }
    /// 不同的编译器下 delete 的操作有所不同。很多时候只是将对应的 mem 标记为 free，但是有时候这块 mem 仍然是可访问的。
    /// 因此最佳实践是 delete 之后将 ptr 设置为 NULL。
    delete node;
}

/**
 * 从 trie 树中删除 word 字符串
 * TODO: Check this! Especially the memory deletion.
 * */
void del(TrieNode *root, const string &word) {
    if (query_str(root, word) != 0) {
        TrieNode *node = root;
        node->pass--;
        for (auto &c: word) {
            int idx = c - 'a';
            /// 不论 if 内的代码是否会被执行，pass 字段都会自减
            if (--node->nexts[idx]->pass == 0) {
                /// 此时这个节点无意义，需要把它延伸出去的节点全部删除
                release(node->nexts[idx]);
                node->nexts[idx] = nullptr;
                return;
            }
            node = node->nexts[idx];
        }
        node->end--;
    }
}

/// ---------------------- for test ----------------------
struct NodeForTestDelete1 {
    int x;
    NodeForTestDelete1 *children[3];
};

NodeForTestDelete1 *new_test_node1(int x) {
    NodeForTestDelete1 *node = new NodeForTestDelete1;
    node->x = x;
    for (auto &child: node->children) {
        child = nullptr;
    }
    return node;
}

void del_test_node1(NodeForTestDelete1 *root) {
    for (auto &child: root->children) {
        if (child != nullptr) {
            del_test_node1(child);
            child = nullptr;
        }
    }
    delete root;
}

struct NodeForTestDelete2 {
    int x;
    NodeForTestDelete2 *left, *right;
};

NodeForTestDelete2 *new_test_node2(int x) {
    NodeForTestDelete2 *node = new NodeForTestDelete2;
    node->x = x;
    node->left = node->right = nullptr;
    return node;
}

void del_test_node2(NodeForTestDelete2 *root) {
    if (root->left != nullptr) {
        del_test_node2(root->left);
        root->left = nullptr;
    }
    if (root->right != nullptr) {
        del_test_node2(root->right);
        root->right = nullptr;
    }
    delete root;
}
/// ------------------------------------------------------

int main() {
    /// ------------------ 测试 Trie 的使用 ------------------
    auto *trie = new_trie_node();
    string arr[] = {"abc", "bd", "ade", "bf", "", "ade", "abcde", "abcd"};
    for (const auto &str: arr) {
        insert(trie, str);
    }
    cout << query_str(trie, "ade") << endl;
    cout << query_prefix(trie, "ab") << endl;
    print(trie);
    del(trie, "bf");
    print(trie);
    del(trie, "bd");
    print(trie);

    /// ------------------ 测试递归释放内存占用 ------------------
    NodeForTestDelete1 *root = new_test_node1(0);
    root->children[0] = new_test_node1(1);
    root->children[1] = new_test_node1(2);
    root->children[0]->children[0] = new_test_node1(3);
    /// 执行完 del_test_node1(root) 之后，再执行下面的 delete root 会报错：pointer being freed was not allocated
    /// 也就是说，root 的内存占用的确是被释放了，然而，我们仍然可以访问 root->x。我们需要手动设定其为 nullptr
    del_test_node1(root);
//    delete root;
    root = nullptr;
    cout << root->x << endl; /// 报错就对了

    NodeForTestDelete2 *r2 = new_test_node2(0);
    r2->left = new_test_node2(1);
    r2->right = new_test_node2(2);
    r2->left->left = new_test_node2(3);
    del_test_node2(r2);
    /// 执行完 del_test_node2(r2); 之后，再执行下面的 delete r2; 会报错：pointer being freed was not allocated
    /// 也就是说，r2 的内存占用的确是被释放了，然而，我们仍然可以访问 r2->x。我们需要手动设定其指向 null
//    delete r2;
    r2 = nullptr;
    cout << r2->x << endl; /// 报错就对了

    return 0;
}