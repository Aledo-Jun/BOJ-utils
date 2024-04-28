//
// Created by june0 on 2024-04-28.
//
#include <bits/stdc++.h>
#include <bits/extc++.h>
#define fastIO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0)
#define endl '\n'
#define all(v) (v).begin(), (v).end()
using namespace std;
using namespace __gnu_cxx;
using ll [[maybe_unused]] = long long;
using ull [[maybe_unused]] = unsigned long long;
using lll [[maybe_unused]] = __int128;
using lld [[maybe_unused]] = long double;
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
 * Segment Tree using iterative method
 * @tparam T type of elements
 * @tparam func function object to be called to perform the query
 * @tparam updating_func function object to be called to update the containing element
 */
template<typename T,
        typename func = plus<T>,
        typename updating_func = plus<T>>
class SegTree_iter {
private:
    func f;
    updating_func updating_f;
    T default_query;

    vector<T> tree, arr;
    int size, height;

    void init() {
        for (int i = size / 2 - 1; i > 0; i--)
            tree[i] = f(tree[i << 1], tree[i << 1 | 1]);
    }

    void _update(int i, T value) {
        i += size / 2;
        tree[i] = updating_f(tree[i], value);
        for (i >>= 1; i > 0; i >>= 1) tree[i] = f(tree[i << 1], tree[i << 1 | 1]);
    }

    T _query(int l, int r) {
        T res1 = default_query, res2 = default_query;
        for (l += size / 2, r += size / 2; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) res1 = f(res1, tree[l++]);
            if (~r & 1) res2 = f(tree[r--], res2);
            // NOTE: There exists cases that the operation's order must be considered
        }
        return f(res1, res2);
    }

public:
    /**
     * Constructor for a segment tree
     * @param v Array that the segment tree will be constructed from
     * @param default_query The result of query that doesn't affect the other query result when performed <i>func</i> with
     */
    SegTree_iter(const vector<T> &v, T default_query = 0) : default_query(std::move(default_query)) {
        arr = v;
        height = (int) ceil(log2(v.size()));
        size = (1 << (height + 1));
        tree.resize(size + 1, default_query);
        for (int i = 0; i < arr.size(); i++) tree[size / 2 + i] = arr[i];
        init();
    }

    void update(int idx, T value) {
        _update(idx, value);
    }

    T query(int left, int right) {
        return _query(left, right);
    }
}; // class SegTree_iter

struct range_max {
    int operator()(int l, int r) const {
        return max(l, r);
    }
};

struct max_update {
    int operator()(int l, int r) const {
        return max(l, r);
    }
};

int n, sqrt_n, k;
struct query {
    int l, r, idx;

    bool operator<(const query& other) const {
        return r < other.r;
    }
};

const int MAX_N = 100'000;

int32_t main() {
    fastIO;
    cin >> n >> k;
    sqrt_n = 400;
    vector<int> v(n + 1, 0);
    for (int i = 1; i <= n; i++) cin >> v[i];
    for (int i = 1; i <= n; i++) (v[i] += v[i - 1]) %= k;

    int m;
    cin >> m;
    vector<query> q(m);
    for (int i = 0; i < m; i++){
        cin >> q[i].l >> q[i].r;
        q[i].l--;
        q[i].idx = i;
    }
    sort(all(q));

    vector<int> ans(m, 0), cnt(k, 0), tree(n + 1, 0), big_chunks;
    vector<vector<int>> w(k);

    for (int i = 0; i <= n; i++) cnt[v[i]]++;
    for (int i = 0; i < k; i++) {
        if (cnt[i] >= sqrt_n) big_chunks.emplace_back(i);
        else cnt[i] = -1;
    }

    // tree[i] = the largest abs(j - i) s.t. v[i] == v[j]
    SegTree_iter<int, range_max, max_update> segTree(tree);

    int ptr = 0;
    for (const auto& [l, r, idx]: q) {
        while (ptr <= r) { // O(n)
            w[v[ptr]].emplace_back(ptr);
            if (cnt[v[ptr]] == -1) { // cnt <= sqrt_n
                for (int i: w[v[ptr]]) // O(sqrt_n)
                    segTree.update(i, ptr - i); // O(log n)
            }
            ptr++;
        } // -> O(n * sqrt_n * log n)

        ans[idx] = segTree.query(l, r);

        for (int i: big_chunks) { // there exist at most sqrt_n chunks
            if (w[i].empty()) continue;
            auto it1 = lower_bound(w[i].begin(), w[i].end(), l);
            auto it2 = --upper_bound(w[i].begin(), w[i].end(), r);

            if (it1 <= it2)
                ans[idx] = max(ans[idx], *it2 - *it1);
        } // -> O(sqrt_n * log n)
    } // => O(n * sqrt_n * log n)

    for (const auto& e : ans) cout << e << endl;

    return 0;
}