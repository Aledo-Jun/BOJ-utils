#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <functional>
#include <cassert>
#include <limits>
using ull = unsigned long long;

using namespace std;

namespace Utils
{
namespace String
{
    // Trie data structure for checking the existence of the prefix
    struct TrieNode {
        TrieNode *children[10]{}; // may adjust size
        int cnt; // how many elements are in this Trie that include *this
        bool isEnd;

        TrieNode() {
            for (auto &i: children)
                i = nullptr;
            isEnd = false;
        }

        void insert(const string& word) {
            TrieNode *current = this;

            for (char c: word) {
                int index = c - '0';

                if (current->children[index] == nullptr)
                    current->children[index] = new TrieNode();

                current = current->children[index];
                current->cnt++;
            }
        }

        void erase(const string& word) {
            TrieNode *current = this;

            for (char c: word) {
                int index = c - '0';
                current = current->children[index];
                current->cnt--;
            }
        }

        int query(const string& word) {
            int n = (int) word.size();
            TrieNode *current = this;

            int result = 0;
            for (int i = 0; i < n; i++) {
                int index = word[i] - '0';
                // Do something
            }
            return result;
        }
    };

     /**
     * Aho-Corasick algorithm for string-searching in linear time
     * @tparam IndexSize size of the input character set(default is english alphabet, 26)
     */
     template<int IndexSize = 26>
     class Aho_Corasick {
         using Index_Fn = function<int(unsigned char)>;
     private:
         struct TrieNode {
             TrieNode* children[IndexSize];
             TrieNode* fail;
             int isEndOf;

             TrieNode() : isEndOf(-1), fail(nullptr) {
                 fill(children, children + IndexSize, nullptr);
             }
             ~TrieNode() {
                 for (auto child : children) delete child;
             }

             void insert(const string& word, int& sz, const Index_Fn& index_of) {
                 TrieNode* current = this;
                 for (const auto c: word) {
                     int index = index_of(c);
                     if (current->children[index] == nullptr)
                         current->children[index] = new TrieNode;

                     current = current->children[index];
                 }
                 current->isEndOf = sz++;
             }
         };

         TrieNode* _root_;
         bool is_initialized = false;
         int _size_;
         Index_Fn _index_fn_;

     public:
         explicit Aho_Corasick(Index_Fn index_fn) : _root_(new TrieNode), _size_(0), _index_fn_(std::move(index_fn)) {}

         ~Aho_Corasick() { delete _root_; }

         void init() {
             queue<TrieNode*> q;
             q.emplace(_root_);
             _root_->fail = _root_;

             while (!q.empty()) {
                 auto current = q.front();
                 q.pop();
                 for (int i = 0; i < IndexSize; i++) {
                     auto next = current->children[i];
                     if (next == nullptr) continue;

                     if (current == _root_) next->fail = _root_;
                     else {
                         auto fail_node = current->fail;
                         while (fail_node != _root_ && fail_node->children[i] == nullptr)
                             fail_node = fail_node->fail;
                         if (fail_node->children[i] != nullptr) fail_node = fail_node->children[i];

                         next->fail = fail_node;
                     }
                     if (next->fail->isEndOf != -1)
                         next->isEndOf = next->fail->isEndOf;

                     q.emplace(next);
                 }
             }
         }

         void insert(const string& word) {
             _root_->insert(word, _size_, _index_fn_);
         }

         int count(const string& word) {
             if (!is_initialized) {
                 init();
                 is_initialized = true;
             }

             int res = 0;
             auto current = _root_;
             for (const auto c : word) {
                 int index = _index_fn_(c);
                 while (current != _root_ && current->children[index] == nullptr)
                     current = current->fail;
                 if (current->children[index] != nullptr)
                     current = current->children[index];

                 if (current->isEndOf != -1) {
                     res++;
                 }
             }
             return res;
         }

         void clear() {
             delete _root_;
             _root_ = new TrieNode;
             is_initialized = false;
             _size_ = 0;
         }
     };

    // Find pattern's partial matches(failure table) for KMP algorithm
    vector<int> getPartialMatch(const string &P) {
        ull m = P.length();
        vector<int> pi(m, 0);
        int match = 0;
        for (int i = 1; i < m; i++) {
            while (match > 0 && P[i] != P[match])
                match = pi[match - 1];
            if (P[i] == P[match])
                pi[i] = ++match;
        }
        return pi;
    }

    // KMP algorithm to find the position of T's substring that matches P
    vector<int> kmpSearch(const string &T, const string &P) {
        ull n = T.length();
        ull m = P.length();
        vector<int> result;
        vector<int> pi = getPartialMatch(P);
        int match = 0;
        for (int i = 0; i < n; i++) {
            while (match > 0 && T[i] != P[match])
                match = pi[match - 1];
            if (T[i] == P[match]) {
                match++;
                if (match == m) {
                    result.push_back(i - m + 1);
                    match = pi[match - 1];
                }
            }
        }
        return result;
    }

    /// @Ref <a href="https://gist.github.com/koosaga/44532e5dec947132ee55da0458255e05">koosaga</a>
    class SuffixArray {
    private:
        std::string str;
        int n;

        std::vector<int> ord, nord;
        std::vector<int> cnt, aux; // for counting sort
        std::vector<int> sfx, rev, lcp;

        bool lcp_initialized = false;

    public:
        explicit SuffixArray(const std::string& s) : str(s), n((int)s.size()),
                                                     ord(n + 1), nord(n + 1),
                                                     cnt(std::max(256, n + 1)), aux(n),
                                                     sfx(n), rev(n), lcp(n)
        {
            int p = 1;
            std::fill(ord.begin(), ord.end(), 0);
            for (int i = 0; i < n; i++) {
                sfx[i] = i;
                ord[i] = (unsigned char)str[i];
            }

            int pnt = 1;
            while (true) {
                std::fill(cnt.begin(), cnt.end(), 0);
                for (int i = 0; i < n; i++)
                    cnt[ord[std::min(i + p, n)]]++;
                for (int i = 1; i <= n || i <= 255; i++)
                    cnt[i] += cnt[i - 1];
                for (int i = n - 1; i >= 0; i--)
                    aux[--cnt[ord[std::min(i + p, n)]]] = i;

                std::fill(cnt.begin(), cnt.end(), 0);
                for (int i = 0; i < n; i++)
                    cnt[ord[i]]++;
                for (int i = 1; i <= n || i <= 255; i++)
                    cnt[i] += cnt[i - 1];
                for (int i = n - 1; i >= 0; i--)
                    sfx[--cnt[ord[aux[i]]]] = aux[i];

                if (pnt == n) break;
                else pnt = 1;

                nord[sfx[0]] = 1;
                for (int i = 1; i < n; i++){
                    if (ord[sfx[i - 1]] != ord[sfx[i]] || ord[sfx[i - 1] + p] != ord[sfx[i] + p])
                        pnt++;
                    nord[sfx[i]] = pnt;
                }
                ord = nord;
                p *= 2;
            }
            for (int i = 0; i < n; i++) rev[sfx[i]] = i;
        }

        void build_lcp() {
            for (int i = 0, h = 0; i < n; i++) {
                if (rev[i]) {
                    int prev = sfx[rev[i] - 1];
                    while (str[prev + h] == str[i + h]) h++;
                    lcp[rev[i]] = h;
                }
                h = std::max(h - 1, 0);
            }
        }

        int operator[](int i) { return sfx[i]; }
        std::vector<int> get_lcp() {
            if (lcp_initialized) return lcp;

            build_lcp();
            return lcp;
        }
    };

    template<typename String = std::string>
    class SA_IS {
        using T = typename String::value_type;
    private:
        int sz;
        std::vector<int> str; // compressed string
        int bucket_sz; // bucket size after compression (== # of distinct characters in the string)

        std::vector<int> sfx, rev, lcp;
        std::vector<int> ptr; // for counting sort

        bool sfx_initialized = false, lcp_initialized = false;

        void build_sfx(const std::vector<int>& s, int bucket, std::vector<int>& sa) {
            int n = static_cast<int>(s.size());
            if (n == 1) {
                sa[0] = 0;
                return;
            }

            std::vector<bool> is_S_type(n, false);
            std::vector<int> cnt(bucket + 1, 0); // for counting sort

            // 1. Check the type of each character and the number of LMS.
            int lms_cnt = 0;
            for (int i = n - 2; i >= 0; i--) {
                if (s[i] < s[i + 1]) is_S_type[i] = true;
                else if (s[i] == s[i + 1]) is_S_type[i] = is_S_type[i + 1];
                else lms_cnt += is_S_type[i + 1];
            }

            std::fill(sa.begin(), sa.end(), -1);
            for (int i = 0; i < n; i++) cnt[s[i] + 1]++;
            for (int i = 1; i <= bucket; i++) cnt[i] += cnt[i - 1];

            // 2. Sort LMS's using counting sort.
            if (lms_cnt) {
                std::vector<int> lms_pos(lms_cnt + 1), sub(lms_cnt, 0);
                std::vector<int> sub_sa(lms_cnt, 0); // Guessed SA only using LMS's

                // 2-1. Specify the position of LMS's
                ptr.assign(std::next(cnt.begin()), cnt.end());
                for (int i = 1, j = 0; i < n; i++) {
                    if (is_S_type[i] && !is_S_type[i - 1]) {
                        rev[i] = j; // there is j-th lms at i
                        lms_pos[j++] = i;
                        sa[--ptr[s[i]]] = i;
                    }
                }
                lms_pos[lms_cnt] = n;

                // 2-2. Induced sorting.
                ptr.assign(cnt.begin(), std::prev(cnt.end()));
                sa[ptr[s[n - 1]]++] = n - 1;
                for (int i = 0; i < n; i++) { // sort L-type (left->right)
                    int j = sa[i] - 1;
                    if (j >= 0 && !is_S_type[j])
                        sa[ptr[s[j]]++] = j;
                }
                ptr.assign(std::next(cnt.begin()), cnt.end());
                for (int i = n - 1; i >= 0; i--) { // sort S-type (right->left)
                    int j = sa[i] - 1;
                    if (j >= 0 && is_S_type[j])
                        sa[--ptr[s[j]]] = j;
                }

                for (int i = 0, j = 0; i < n; i++) {
                    if (sa[i] > 0 && is_S_type[sa[i]] && !is_S_type[sa[i] - 1]) {
                        sub_sa[j++] = rev[sa[i]];
                    }
                }

                // 2-3. For each consecutive LMS's in sub_sa,
                for (int i = 1; i < lms_cnt; i++) {
                    int q1 = sub_sa[i - 1], q2 = sub_sa[i];
                    int j1 = lms_pos[q1], j2 = lms_pos[q2];
                    int n1 = lms_pos[q1 + 1] - j1;
                    int n2 = lms_pos[q2 + 1] - j2;
                    sub[q2] = sub[q1];
                    // Check whether they are equal or not.
                    // If not, give the second one another name.
                    if (n1 != n2) sub[q2]++;
                    else {
                        for (int j = 0; j < n1; j++) {
                            if (s[j1 + j] < s[j2 + j] || is_S_type[j1 + j] < is_S_type[j2 + j]) {
                                sub[q2]++;
                                break;
                            }
                        }
                    }
                }

                // 2-4. At this point, sub_sa is sorted.
                // But the actual SA might not be sorted properly(might exist duplicates).
                int sub_bucket = sub[sub_sa[lms_cnt - 1]] + 1;
                if (sub_bucket < lms_cnt)               // if not properly sorted,
                    build_sfx(sub, sub_bucket, sub_sa); // recurse

                // 2-5. Merge sorted sub_sa to sa.
                std::fill(sa.begin(), sa.end(), -1);
                ptr.assign(std::next(cnt.begin()), cnt.end());
                for (int i = lms_cnt - 1; i >= 0; i--) {
                    int j = lms_pos[sub_sa[i]];
                    sa[--ptr[s[j]]] = j;
                }
            }

            // 3. Finally, clean up the rest of the characters.
            ptr.assign(cnt.begin(), std::prev(cnt.end()));
            sa[ptr[s[n - 1]]++] = n - 1;
            for (int i = 0; i < n; i++) {
                int j = sa[i] - 1;
                if (j >= 0 && !is_S_type[j])
                    sa[ptr[s[j]]++] = j;
            }
            ptr.assign(std::next(cnt.begin()), cnt.end());
            for (int i = n - 1; i >= 0; i--) {
                int j = sa[i] - 1;
                if (j >= 0 && is_S_type[j])
                    sa[--ptr[s[j]]] = j;
            }
        }

        void build_lcp() {
            assert(sfx_initialized);
            for (int i = 0; i < sz; i++) rev[sfx[i]] = i;
            str.emplace_back(-1); // IDK why this is needed. maybe sentinel or something?
            for (int i = 0, h = 0; i < sz; i++) {
                if (rev[i]) {
                    int prev = sfx[rev[i] - 1];
                    while (str[prev + h] == str[i + h]) h++;
                    lcp[rev[i]] = h;
                }
                h = std::max(h - 1, 0);
            }
            str.pop_back();
        }

    public:
        /* implicit */ SA_IS(const String& s) // NOLINT
                : sz(static_cast<int>(s.size())), str(sz), rev(sz), sfx(sz)
        {
            const auto mn = std::numeric_limits<T>::min();
            const auto mx = std::numeric_limits<T>::max();
            std::vector<int> compressor(mx - mn + 1);
            for (int i = 0; i < sz; i++) compressor[s[i] - mn] = 1;
            for (int i = 1; i < static_cast<int>(compressor.size()); i++) compressor[i] += compressor[i - 1];
            for (int i = 0; i < sz; i++) str[i] = compressor[s[i] - mn] - 1;
            bucket_sz = compressor.back();

            build_sfx(str, bucket_sz, sfx);
            sfx_initialized = true;
        }

        int operator[](int i) const { return sfx[i]; }
        std::vector<int> get_sfx() { return sfx; }
        std::vector<int> get_lcp() {
            if (lcp_initialized) return lcp;

            lcp.resize(sz, 0);
            build_lcp();
            lcp_initialized = true;
            return lcp;
        }
    };
} // namespace String
} // namespace Utils