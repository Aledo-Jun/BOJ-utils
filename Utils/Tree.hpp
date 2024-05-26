//
// Created by june0 on 2024-02-13.
//
#include <vector>
#include <map>
#include <functional>
#include <cmath>

using namespace std;

#ifndef BOJ_TREE_HPP
#define BOJ_TREE_HPP

namespace Utils
{
namespace Tree
{
    template<typename T>
    using graph = std::vector<std::vector<std::pair<int,T>>>;

    vector<int> getPreorder(const vector<int>& inorder, const vector<int>& postorder) {
        // Create a map to store the indices of elements in the inorder traversal
        unordered_map<int, int> inorder_map;
        for (int i = 0; i < inorder.size(); ++i) {
            inorder_map[inorder[i]] = i;
        }

        // Recursive helper function to build the tree
        function<vector<int>(int, int, int, int)> build = [&](int in_start, int in_end, int post_start, int post_end) -> vector<int> {
            if (in_start > in_end) return {};
            vector<int> preorder;
            int root_val = postorder[post_end];
            int root_idx = inorder_map[root_val];
            int left_size = root_idx - in_start;
            preorder.push_back(root_val);
            // Build left subtree
            vector<int> left_subtree = build(in_start, root_idx - 1, post_start, post_start + left_size - 1);
            preorder.insert(preorder.end(), left_subtree.begin(), left_subtree.end());
            // Build right subtree
            vector<int> right_subtree = build(root_idx + 1, in_end, post_start + left_size, post_end - 1);
            preorder.insert(preorder.end(), right_subtree.begin(), right_subtree.end());
            return preorder;
        };

        return build(0, inorder.size() - 1, 0, postorder.size() - 1);
    }

    class LCA {
    private:
        int MAX_BIT;

        std::vector<int> depth; // Depth of each node
        std::vector<std::vector<int>> parent; // Binary Lifting table
        graph<int> adj; // Adjacency list of the tree, Assume that nodes are 1-indexed

    public:
        LCA(const graph<int>& adj) : adj(adj) {
            int sz = (int) adj.size();
            MAX_BIT = std::ceil(std::log2(sz));
            depth.resize(sz);
            parent.resize(sz, vector<int>(MAX_BIT));
            make_tree(1, 0, 0);
        }

        void make_tree(int u, int p, int d) {
            depth[u] = d;
            parent[u][0] = p;
            for (int i = 1; i < 20; i++) {
                parent[u][i] = parent[parent[u][i - 1]][i - 1];
            }
            for (const auto& [v, w]: adj[u]) {
                if (v != p) {
                    make_tree(v, u, d + 1);
                }
            }
        }

        int lca(int u, int v) {
            if (depth[u] < depth[v]) swap(u, v);
            for (int i = 19; i >= 0; i--) {
                if (depth[u] - (1 << i) >= depth[v]) {
                    u = parent[u][i];
                }
            }
            if (u == v) return u;
            for (int i = 19; i >= 0; i--) {
                if (parent[u][i] != parent[v][i]) {
                    u = parent[u][i];
                    v = parent[v][i];
                }
            }
            return parent[u][0];
        }
    };


/**
 * Splay tree implementation
 * @tparam T Value type
 */
    template<typename T>
    class SplayTree {
        struct Node {
            Node *l, *r, *p;
            T key; // value stored in the node
            int cnt; // # of nodes in the subtree
            bool flip;

            /* Add more variables if needed */

            Node() : l(nullptr), r(nullptr), p(nullptr), key(T(0)), cnt(1), flip(false) {}
            explicit Node(T key) : l(nullptr), r(nullptr), p(nullptr), key(key), cnt(1), flip(false) {
                /* Initialize extra variables */
            }
        } *tree;

        // modify if needed
        void update(Node *x) {
            x->cnt = 1;
            if (x->l) x->cnt += x->l->cnt;
            if (x->r) x->cnt += x->r->cnt;
        }

        void rotate(Node *x) {
            Node *p = x->p;
            Node *b;
            if (x == p->l) {
                p->l = b = x->r;
                x->r = p;
            } else {
                p->r = b = x->l;
                x->l = p;
            }
            x->p = p->p;
            p->p = x;
            if (b) b->p = p;
            (x->p ? p == x->p->l ? x->p->l : x->p->r : tree) = x;

            update(p);
            update(x);
        }

        void push(Node *x) {
            if (x->flip) {
                swap(x->l, x->r);
                if (x->l) x->l->flip ^= 1;
                if (x->r) x->r->flip ^= 1;
                x->flip = false;
            }
        }

        void splay(Node *x) {
            while (x->p) {
                Node *p = x->p;
                Node *g = p->p;

                if (g) push(g);
                push(p);push(x);

                if (g) rotate((x == p->l) == (p == g->l) ? p : x);
                rotate(x);
            }
        }

    public:
        template<typename Iter>
        SplayTree(const Iter& s, const Iter& e) {
            Node *p = tree = new Node(0);
            for (auto it = s; it != e; it++) {
                p->r = new Node(*it);
                p->r->p = p;
                p = p->r;
            }
            p->r = new Node(0);
            p->r->p = p;
            p = p->r;

            while (p) {
                update(p);
                p = p->p;
            }
        }

        //
        /* Key Based Methods */
        // NOTE: No duplicates are allowed
        /**
         * Insert a new node with the given key
         * @param key the key value to insert
         */
        void insert(T key) {
            Node *p = tree, **pp;
            if (!p) {
                Node *x = new Node(key);
                tree = x;
                return;
            }
            while (true) {
                if (key == p->key) return; // no duplicate allowed
                if (key < p->key) {
                    if (!p->l) {
                        pp = &p->l;
                        break;
                    }
                    p = p->l;
                } else {
                    if (!p->r) {
                        pp = &p->r;
                        break;
                    }
                    p = p->r;
                }
            }
            Node *x = new Node(key);
            *pp = x;  // p -> x
            x->p = p; // x -> p

            splay(x); // Crucial!
        }

        /**
         * The node with the given key becomes the root node after the function call
         * @param key the key value to find
         * @return true if found, false otherwise
         */
        bool find(T key) {
            Node *p = tree;
            if (!p) return false;
            while (p) {
                if (key == p->key) break;
                if (key < p->key) {
                    if (!p->l) break;
                    p = p->l;
                } else {
                    if (!p->r) break;
                    p = p->r;
                }
            }
            splay(p);
            return key == p->key;
        }

        /**
         * Erase the node with the given key
         */
        void erase(T key) {
            if (!find(key)) return;
            Node *p = tree;
            if (p->l) {
                if (p->r) {
                    tree = p->l;
                    tree->p = nullptr;

                    Node *x = tree;
                    while (x->r) x = x->r;
                    x->r = p->r;
                    p->r->p = x;
                    splay(x);
                    delete p;
                    return;
                }
                tree = p->l;
                tree->p = nullptr;
                delete p;
                return;
            }
            if (p->r) {
                tree = p->r;
                tree->p = nullptr;
                delete p;
                return;
            }
            delete p;
            tree = nullptr;
        }

        //
        /* Index Based Methods */
        // NOTE: 1-based indexing
        /**
         * Make the root node to be the k-th
         */
        void find_kth(int k) {
            Node *x = tree; push(x);
            while (true) {
                while (x->l && x->l->cnt > k) {
                    x = x->l; push(x);
                }
                if (x->l) k -= x->l->cnt;
                if (!k--) break;
                x = x->r; push(x);
            }
            splay(x);
        }

        /**
         * Make the tree->r->l to represent the given interval [l,r]
         */
        void interval(int l, int r) {
            find_kth(l - 1);
            auto *p = tree;
            tree = p->r;
            tree->p = nullptr;
            find_kth(r - l + 1);
            p->r = tree;
            tree->p = p;
            tree = p;
        }

        /**
         * Insert a new node with the key of val into the given pos
         */
        void insert(int pos, T val) {
            find_kth(pos);
            auto *p = tree, *x = new Node(val);
            x->p = p;
            p->l->p = x;
            x->l = p->l;
            p->l = x;
            splay(x);
        }

        /**
         * Erase the node at the given pos
         */
        void erase_at(int pos) {
            interval(pos, pos);
            auto *x = tree->r->l;
            x->p->l = nullptr;
            splay(x->p);
            delete x;
        }

        /**
         * Update the node's key at pos to val
         */
        void update(int pos, T val) {
            find_kth(pos);
            tree->key = val;
            update(tree);
        }

        /**
         * Flip the interval[l, r]
         */
        void flip(int l, int r) {
            interval(l, r);
            tree->r->l->flip ^= 1;
        }

        /**
         * Cyclic shift the range[l, r] k steps
         * @param k positive to right, negative to left
         */
        void shift(int l, int r, int k) {
            interval(l, r);
            k %= r - l + 1;
            if (k < 0) k += r - l + 1;
            if (k) flip(l, r), flip(l, l + k - 1), flip(l + k, r);
        }

        T operator[](int idx) {
            find_kth(idx);
            return tree->key;
        }

        T query(int l, int r, int k) {
            interval(l, r);
            // Do something within the interval
        }
    }; // class SplayTree

    pair<vector<int>,vector<int>> EulerTour(const graph<int>& g) {
        int n = (int) g.size() - 1;
        vector<int> S(n + 1), T(n + 1);

        vector<bool> visited(n + 1, false);
        function<void(int,int&)> dfs = [&](int u, int& d) -> void {
            S[u] = d;
            for (const auto& [v, _]: g[u]) {
                if (visited[v]) continue;
                visited[v] = true;
                dfs(v, ++d);
            }
            T[u] = d;
        };

        int d = 1;
        dfs(1, d);

        return {S, T};
    }

} // namespace Tree
} // namespace Utils

#endif //BOJ_TREE_HPP
