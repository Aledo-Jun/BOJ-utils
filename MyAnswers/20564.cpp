//
// Created by june0 on 2024-06-24.
//
#include <bits/stdc++.h>
#include <bits/extc++.h>
#pragma GCC target("avx2")
#pragma GCC optimize("O3,unroll-loops")
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
 * These classes use low-level i/o functions (@c fread()/mmap() for input, @c fwrite()/write() for output)
 * so that they can read or write much faster than cin and cout. <br>
 * Several macros are defined for convenience of using them.
 *
 * @Author  <a href="https://blog.naver.com/jinhan814/222266396476">jinhan814</a>
 * @Date    2021-05-06
 */
namespace FastIO {
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

    constexpr int SZ = 1 << 20;

    /* Class INPUT */
    class INPUT {
    private:
        char* p;
        bool __END_FLAG__, __GET_LINE_FLAG__; // NOLINT
    public:
        explicit operator bool() const { return !__END_FLAG__; }
        INPUT() {
            struct stat st;
            fstat(0, &st);
            p = (char*)mmap(0, st.st_size, PROT_READ, MAP_SHARED, 0, 0);
        }

        static bool IsBlank(char c) { return c == ' ' || c == '\n'; }

        static bool IsEnd(char c) { return c == '\0'; }

        char _ReadChar() { // NOLINT
            return *p++;
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
            write(1, writeBuffer, write_idx);
            write_idx = 0;
        }

        void WriteChar(char c) {
            if (write_idx == SZ) Flush();
            writeBuffer[write_idx++] = c;
        }

        template<class T>
        void WriteInt(T n) {
            int sz = GetSize(n);
            if (write_idx + sz >= SZ) Flush();
            if (n < 0) writeBuffer[write_idx++] = '-', n = -n;
            for (int i = sz; i-- > 0; n /= 10)
                writeBuffer[write_idx + i] = n % 10 | 48;
            write_idx += sz;
        }

        void WriteString(const std::string& s) {
            for (auto &c: s) WriteChar(c);
        }

        void WriteDouble(double d) {
            WriteString(std::to_string(d));
        }

        template<class T>
        int GetSize(T n) {
            int ret = 1;
            for (n = n >= 0 ? n : -n; n >= 10; n /= 10) ret++;

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

template<typename T = int>
class FenWickTree {
private:
    vector<T> tree1, tree2;
    int size;

    void _update(vector<T>& tree, int x, T val) {
        for (int i = x; i < size; i += (i & -i)) {
            tree[i] += val;
        }
    }

    T _query(const vector<T>& tree, int x) const {
        T res = 0;
        for (int i = x; i > 0; i -= (i & -i)) {
            res += tree[i];
        }
        return res;
    }

public:
    // Empty tree constructor
    FenWickTree(int size) : size(size) {
        tree1.resize(size + 1, 0);
        tree2.resize(size + 1, 0);
    }

    FenWickTree(const vector<T>& v) : FenWickTree((int)v.size()) {
        for (int i = 1; i <= size; i++) {
            update(i, v[i - 1]);
        }
    }

    // Point update: add val to the element at x
    void update(int x, T val) {
        _update(tree1, x, val);
    }

    // Range update: add val to each element in range [left, right]
    void update(int left, int right, T val) {
        _update(tree1, left, val);
        _update(tree1, right + 1, -val);

        _update(tree2, left, -val * (left - 1));
        _update(tree2, right + 1, val * right);
    }

    // Query the prefix sum up to idx
    T query(int idx) const {
        return _query(tree1, idx) + _query(tree2, idx) - idx;
    }

    // Query the sum in the range [left, right]
    T query(int left, int right) const {
        if (left > right) return 0;
        left--;
        T l = _query(tree1, left) * (left) + _query(tree2, left);
        T r = _query(tree1, right) * right + _query(tree2, right);
        return r - l;
    }
};

using BIT = FenWickTree<ll>;

/**
 * @brief Heavy Light Decomposition to perform a query on a path in a tree
 */
class HLD {
private:
    vector<int> sz,     // size of the subtree
    top,    // top of the chain the node belongs to
    S;      // Euler Tour
    vector<vector<int>> parent; // parent of the node (binary lifting table)

    int root, n;
    graph<int> adj;     // adjacent list for original tree
    vector<vector<int>> g;

    vector<bool> visited;
    void dfs(int u) { // construct g
        visited[u] = true;
        for (const auto& [v, w]: adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;
            g[u].emplace_back(v);
            dfs(v);
        }
    }

    // construct sz, depth, parent, and make the nodes adjacent if they belong to the same chain
    void dfs1(int u) {
        sz[u] = 1;
        for (auto& v : g[u]) {
            parent[v][0] = u;
            dfs1(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) std::swap(v, g[u][0]);
        }
    }

    int pv = 0;
    // construct S, T, R, and top
    void dfs2(int u) {
        S[u] = ++pv;
        for (const auto& v: g[u]) {
            top[v] = v == g[u][0] ? top[u] : v;
            dfs2(v);
        }
    }

    /* Add something to Utilize the HLD e.g. SegTree */
    BIT white_cnt;
    vector<bool> is_white;
public:
    explicit
    HLD(graph<int>&& g, int root = 1) : adj(g), root(root), n((int)g.size()), white_cnt(n) {
        sz.resize(n);
        parent.resize(n, vector<int>(1));
        top.resize(n);
        S.resize(n);
        this->g.resize(n);
        visited.resize(n);

        dfs(root), dfs1(root), dfs2(root);

        // initialize additional variables
        is_white.resize(n, false);
    }

    ll ans = 0;
    ll query(int x) {
        bool flag = (is_white[x] = !is_white[x]);

        while (top[x] ^ top[root]) {
            int st = top[x];
            if (flag) {
                ans += white_cnt.query(S[st], S[x]);
                white_cnt.update(S[st], S[x], +1);
            } else {
                white_cnt.update(S[st], S[x], -1);
                ans -= white_cnt.query(S[st], S[x]);
            }
            x = parent[st][0];
        }
        if (x != root) {
            if (flag) {
                ans += white_cnt.query(S[root] + 1, S[x]);
                white_cnt.update(S[root], S[x], +1);
            } else {
                white_cnt.update(S[root], S[x], -1);
                ans -= white_cnt.query(S[root] + 1, S[x]);
            }
        }

        return ans;
    }

}; // class HLD


int32_t main() {
    fastIO;
    int n, q;
    cin >> n >> q;
    vector<int> c(n + 1);
    for (int i = 1; i <= n; i++) cin >> c[i];

    graph<int> g(n + 1);
    for (int v = 2; v <= n; v++) {
        int u;
        cin >> u;
        g[u].emplace_back(v, 1);
    }

    HLD solver(std::move(g));

    ll initial = 0;
    for (int i = 1; i <= n; i++) if (c[i]) initial = solver.query(i);
    cout << initial << endl;

    while (q--) {
        int x;
        cin >> x;
        cout << solver.query(x) << endl;
    }

    return 0;
}