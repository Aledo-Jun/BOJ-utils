//
// Created by june0 on 2024-06-03.
//
#include <bits/stdc++.h>
#include <bits/extc++.h>
//#pragma GCC optimize("O3,unroll-loops")
#define fastIO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0)
#define endl '\n'
#define all(v) (v).begin(), (v).end()
using namespace std;
using namespace __gnu_cxx;
using ll [[maybe_unused]] = long long;
using ull [[maybe_unused]] = unsigned long long;
using lll [[maybe_unused]] = __int128;
using lld [[maybe_unused]] = long double;
using llld [[maybe_unused]] = __float128;
template<typename T> using graph [[maybe_unused]] = vector<vector<pair<int,T>>>;
template<typename T> using matrix [[maybe_unused]] = vector<vector<T>>;

/**
 *  "Premature optimization is the root of all evil."
 *  <sub> Donald Knuth </sub>
 */

#ifdef ONLINE_JUDGE // Be careful if problem is about strings.
/**
 * Namespace for Fast I/O
 *
 * @class@b INPUT
 * class which can replace the cin
 *
 * @class@b OUTPUT
 * class which can replace the cout
 *
 * @Description
 * These classes use low-level i/o functions (@c fread() for input, @c fwrite() for output)
 * so that they can read or write much faster than cin and cout. <br>
 * Several macros are defined for convenience of using them.
 *
 * @Author  <a href="https://blog.naver.com/jinhan814/222266396476">jinhan814</a>
 * @Date    2021-05-06
 */
namespace FastIO {
    constexpr int SZ = 1 << 20;

    /* Class INPUT */
    class INPUT {
    private:
        char readBuffer[SZ];
        int read_idx, next_idx;
        bool __END_FLAG__, __GET_LINE_FLAG__; // NOLINT
    public:
        explicit operator bool() const { return !__END_FLAG__; }

        static bool IsBlank(char c) { return c == ' ' || c == '\n'; }

        static bool IsEnd(char c) { return c == '\0'; }

        char _ReadChar() { // NOLINT
            if (read_idx == next_idx) {
                next_idx = (int)fread(readBuffer, sizeof(char), SZ, stdin);
                if (next_idx == 0) return 0;
                read_idx = 0;
            }
            return readBuffer[read_idx++];
        }

        char ReadChar() {
            char ret = _ReadChar();
            for (; IsBlank(ret); ret = _ReadChar());

            return ret;
        }

        template<class T>
        T ReadInt() {
            T ret = 0;
            char curr = _ReadChar();
            bool minus_flag = false;

            for (; IsBlank(curr); curr = _ReadChar());
            if (curr == '-') minus_flag = true, curr = _ReadChar();
            for (; !IsBlank(curr) && !IsEnd(curr); curr = _ReadChar())
                ret = 10 * ret + (curr & 15);
            if (IsEnd(curr)) __END_FLAG__ = true;

            return minus_flag ? -ret : ret;
        }

        std::string ReadString() {
            std::string ret;
            char curr = _ReadChar();
            for (; IsBlank(curr); curr = _ReadChar());
            for (; !IsBlank(curr) && !IsEnd(curr); curr = _ReadChar())
                ret += curr;
            if (IsEnd(curr)) __END_FLAG__ = true;

            return ret;
        }

        double ReadDouble() {
            std::string ret = ReadString();
            return std::stod(ret);
        }

        std::string getline() {
            std::string ret;
            char curr = _ReadChar();
            for (; curr != '\n' && !IsEnd(curr); curr = _ReadChar())
                ret += curr;
            if (__GET_LINE_FLAG__) __END_FLAG__ = true;
            if (IsEnd(curr)) __GET_LINE_FLAG__ = true;

            return ret;
        }

        friend INPUT &getline(INPUT &in, std::string &s) {
            s = in.getline();
            return in;
        }
    } _in;
    /* End of Class INPUT */

    /* Class OUTPUT */
    class OUTPUT {
    private:
        char writeBuffer[SZ];
        int write_idx;
    public:
        ~OUTPUT() { Flush(); }

        explicit operator bool() const { return true; }

        void Flush() {
            fwrite(writeBuffer, sizeof(char), write_idx, stdout);
            write_idx = 0;
        }

        void WriteChar(char c) {
            if (write_idx == SZ) Flush();
            writeBuffer[write_idx++] = c;
        }

        template<class T>
        void WriteInt(T total) {
            int sz = GetSize(total);
            if (write_idx + sz + 1 >= SZ) Flush();
            if (total < 0) writeBuffer[write_idx++] = '-', total = -total;
            for (int i = sz; i-- > 0; total /= 10)
                writeBuffer[write_idx + i] = total % 10 | 48;
            write_idx += sz;
        }

        void WriteString(const std::string& s) {
            for (auto &c: s) WriteChar(c);
        }

        void WriteDouble(double d) {
            WriteString(std::to_string(d));
        }

        template<class T>
        int GetSize(T total) {
            int ret = 1;
            for (total = total >= 0 ? total : -total; total >= 10; total /= 10) ret++;

            return ret;
        }
    } _out;
    /* End of Class OUTPUT */

    /* Operators */
    INPUT &operator>>(INPUT &in, char &i) {
        i = in.ReadChar();
        return in;
    }

    INPUT &operator>>(INPUT &in, std::string &i) {
        i = in.ReadString();
        return in;
    }

    template<class T, typename std::enable_if_t<std::is_arithmetic_v<T>> * = nullptr>
    INPUT &operator>>(INPUT &in, T &i) {
        if constexpr (std::is_floating_point_v<T>) i = in.ReadDouble();
        else if constexpr (std::is_integral_v<T>) i = in.ReadInt<T>();
        return in;
    }

    OUTPUT &operator<<(OUTPUT &out, char i) {
        out.WriteChar(i);
        return out;
    }

    OUTPUT &operator<<(OUTPUT &out, const std::string &i) {
        out.WriteString(i);
        return out;
    }

    template<class T, typename std::enable_if_t<std::is_arithmetic_v<T>> * = nullptr>
    OUTPUT &operator<<(OUTPUT &out, T i) {
        if constexpr (std::is_floating_point_v<T>) out.WriteDouble(i);
        else if constexpr (std::is_integral_v<T>) out.WriteInt(i);
        return out;
    }

    /* Macros for convenience */
    #undef fastIO
    #define fastIO 1
    #define cin _in
    #define cout _out
    #define istream INPUT
    #define ostream OUTPUT
};
using namespace FastIO;
#endif

/**
 * Lazy Segment Tree using iterative method
 * @tparam T value type
 * @tparam S lazy type
 * @tparam TT functor to merge tree nodes -> T(T,T)
 * @tparam SS functor to update the lazy value -> S(S,S)
 * @tparam TS functor to apply lazy update to tree node -> T(T,S,int)
 */
template<typename T, typename S = T,
        typename TT = T(T,T),
        typename SS = S(S,S),
        typename TS = T(T,S,int)>
class LazySegTree_iter {
private:
    TT f;
    SS lazy_to_lazy;
    TS lazy_to_tree;
    T default_query;
    S default_lazy;

    vector<T> tree, arr;
    vector<S> lazy;
    int size{}, height{}, n{};

    void init() {
        for (int i = n - 1; i >= 1; i--) pull(i);
    }

    void apply(int node, S value, int offset) {
        tree[node] = lazy_to_tree(tree[node], value, offset);
        if (node < n) lazy[node] = lazy_to_lazy(lazy[node], value);
    }

    void pull(int node) {
        tree[node] = f(tree[node << 1], tree[node << 1 | 1]);
    }

    void pull_all(int l, int r) {
        for (int i = 1; i <= height; i++) {
            if ((l >> i << i) != l) pull(l >> i);
            if ((r >> i << i) != r) pull((r - 1) >> i);
        }
    }

    void push(int node, int offset = 2) {
        if (lazy[node] == default_lazy) return;
        apply(node << 1, lazy[node], offset >> 1);
        apply(node << 1 | 1, lazy[node], offset >> 1);
        lazy[node] = default_lazy;
    }

    void push_all(int l, int r) {
        for (int i = height, k = n; i >= 1; i--, k >>= 1) {
            if ((l >> i << i) != l) push(l >> i, k);
            if ((r >> i << i) != r) push((r - 1) >> i, k);
        }
    }

    void _update(int i, S value) {
        i += n;
        for (int j = height; j >= 1; j--) push(i >> j);
        tree[i] = lazy_to_tree(tree[i], value, 1);
        for (int j = 1; j <= height; j++) pull(i >> j);
    }

    void _update(int l, int r, S value) {
        l += n, r += n;
        push_all(l, r + 1);

        int l0 = l, r0 = r;
        for (int k = 1; l <= r; l >>= 1, r >>= 1, k <<= 1) {
            if (l & 1) apply(l++, value, k);
            if (~r & 1) apply(r--, value, k);
        }

        l = l0, r = r0;
        pull_all(l, r + 1);
    }

    T _query(int i) {
        i += n;
        for (int j = height; j >= 1; j--) push(i >> j);
        return tree[i];
    }

    T _query(int l, int r) {
        l += n, r += n;
        push_all(l, r + 1);

        T res1 = default_query, res2 = default_query;
        for (; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) res1 = f(res1, tree[l++]);
            if (~r & 1) res2 = f(tree[r--], res2);
            // NOTE: There exists cases that the operation's order must be considered
        }
        return f(res1, res2);
    }

public:
    LazySegTree_iter() = default;
    LazySegTree_iter(int sz, T _default_query = 0, S _default_lazy = 0)
            : default_query(std::move(_default_query)), default_lazy(std::move(_default_lazy))
    {
        height = (int) ceil(log2(sz));
        size = (1 << (height + 1));
        n = size >> 1;
        tree.resize(size + 1, default_query);
        lazy.resize(size + 1, default_lazy);
        init();
    }
    /**
     * Constructor for a segment tree
     * @param v Array that the segment tree will be constructed from
     * @param default_query The result of query that doesn't affect the other query result when performed <i>TT</i> with
     */
    explicit
    LazySegTree_iter(const vector<T> &v, T _default_query = 0, S _default_lazy = 0)
            : default_query(std::move(_default_query)), default_lazy(std::move(_default_lazy))
    {
        arr = v;
        height = (int) ceil(log2(v.size()));
        size = (1 << (height + 1));
        n = size >> 1;
        tree.resize(size + 1, default_query);
        lazy.resize(size + 1, default_lazy);
        std::copy(arr.begin(), arr.end(), tree.begin() + n);
        init();
    }

    void update(int idx, T value) {
        _update(idx, value);
    }

    void update(int l, int r, S value) {
        _update(l, r, value);
    }

    T query(int idx) {
        return _query(idx);
    }

    T query(int left, int right) {
        return _query(left, right);
    }
}; // class LazySegTree_iter

struct LazyType {
    unsigned sum = 0, mul = 1;

    bool operator==(const LazyType& r) const {
        return sum == r.sum && mul == r.mul;
    }
};

struct SS {
    LazyType operator()(const LazyType& l, const LazyType& r) {
        return {l.sum * r.mul + r.sum, l.mul * r.mul};
    }
};

struct TS {
    unsigned operator()(const unsigned& l, const LazyType& r, const int& offset) {
        return l * r.mul + offset * r.sum;
    }
};

class HLD {
private:
    vector<int> sz,     // size of the subtree
    depth,  // depth of the node
    parent, // parent of the node
    top,    // top of the chain the node belongs to
    S, T;   // Euler Tour

    graph<int> adj;     // adjacent list for original tree
    vector<vector<int>> g;

    vector<bool> visited;
    void dfs(int u = 1) { // construct g
        visited[u] = true;
        for (const auto& [v, _]: adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;
            g[u].emplace_back(v);
            dfs(v);
        }
    }

    // construct sz, depth, parent, and make the nodes adjacent if they belong to the same chain
    void dfs1(int u = 1) {
        sz[u] = 1;
        for (auto& v : g[u]) {
            depth[v] = depth[u] + 1;
            parent[v] = u;
            dfs1(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) std::swap(v, g[u][0]);
        }
    }

    int pv = 0;
    // construct S, T, and top
    void dfs2(int u = 1) {
        S[u] = ++pv;
        for (const auto& v: g[u]) {
            top[v] = v == g[u][0] ? top[u] : v;
            dfs2(v);
        }
        T[u] = pv;
    }

    /* Add something to Utilize the HLD */
    LazySegTree_iter<unsigned,LazyType,plus<>,SS,TS> segTree;
public:
    explicit
    HLD(const graph<int>& g) : adj(g) {
        int n = (int) g.size();
        sz.resize(n);
        depth.resize(n);
        parent.resize(n);
        top.resize(n);
        S.resize(n); T.resize(n);
        this->g.resize(n);
        visited.resize(n);
        dfs(), dfs1(), dfs2();

        segTree = LazySegTree_iter<unsigned,LazyType,plus<>,SS,TS>(n, 0, {0, 1});
    }

    void add(int x, unsigned val) {
        segTree.update(S[x], T[x], {val, 1});
    }

    void add(int x, int y, unsigned val) {
        while (top[x] ^ top[y]) {
            if (depth[top[x]] < depth[top[y]]) swap(x, y);
            int st = top[x];
            segTree.update(S[st], S[x], {val, 1});
            x = parent[st];
        }
        // at this point x and y are in a same chain
        if (depth[x] > depth[y]) swap(x, y);
        segTree.update(S[x], S[y], {val, 1});
    }

    void multiply(int x, unsigned val) {
        segTree.update(S[x], T[x], {0, val});
    }

    void multiply(int x, int y, unsigned val) {
        while (top[x] ^ top[y]) {
            if (depth[top[x]] < depth[top[y]]) swap(x, y);
            int st = top[x];
            segTree.update(S[st], S[x], {0, val});
            x = parent[st];
        }
        // at this point x and y are in a same chain
        if (depth[x] > depth[y]) swap(x, y);
        segTree.update(S[x], S[y], {0, val});
    }

    unsigned query(int x) {
        return segTree.query(S[x], T[x]);
    }

    unsigned query(int x, int y) {
        unsigned res = 0;
        while (top[x] ^ top[y]) {
            if (depth[top[x]] < depth[top[y]]) swap(x, y);
            int st = top[x];
            res += segTree.query(S[st], S[x]);
            x = parent[st];
        }
        // at this point x and y are in a same chain
        if (depth[x] > depth[y]) swap(x, y);
        res += segTree.query(S[x], S[y]);
        return res;
    }
};

int32_t main() {
    fastIO;
    int n, q;
    cin >> n >> q;
    graph<int> g(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].emplace_back(v, 1);
        g[v].emplace_back(u, 1);
    }

    HLD solver(g);

    while (q--) {
        int cmd, x, y, v;
        cin >> cmd;
        if (cmd == 1) {
            cin >> x >> v;
            solver.add(x, v);
        } else if (cmd == 2) {
            cin >> x >> y >> v;
            solver.add(x, y, v);
        } else if (cmd == 3) {
            cin >> x >> v;
            solver.multiply(x, v);
        } else if (cmd == 4) {
            cin >> x >> y >> v;
            solver.multiply(x, y, v);
        } else if (cmd == 5) {
            cin >> x;
            cout << solver.query(x) << endl;
        } else {
            cin >> x >> y;
            cout << solver.query(x, y) << endl;
        }
    }

    return 0;
}