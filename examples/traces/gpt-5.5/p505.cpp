#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct Node {
        int ch[26];
        int l, r;
        int head;
        Node() : l(0), r(0), head(-1) {
            memset(ch, 0, sizeof(ch));
        }
    };

    vector<Node> t;
    vector<int> nextTerm;

    Trie(int n = 0) {
        t.reserve(n + 1);
        t.emplace_back();
    }

    int insertString(const string& s, int id) {
        int v = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!t[v].ch[x]) {
                t[v].ch[x] = (int)t.size();
                t.emplace_back();
            }
            v = t[v].ch[x];
        }
        nextTerm[id] = t[v].head;
        t[v].head = id;
        return v;
    }

    int findString(const string& s) const {
        int v = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!t[v].ch[x]) return -1;
            v = t[v].ch[x];
        }
        return v;
    }

    void buildOrder(vector<int>& coord) {
        int timer = 0;
        vector<pair<int, int>> st;
        st.push_back({0, -1});

        while (!st.empty()) {
            int v = st.back().first;
            int &state = st.back().second;

            if (state == -1) {
                t[v].l = timer + 1;

                for (int id = t[v].head; id != -1; id = nextTerm[id]) {
                    coord[id] = ++timer;
                }

                state = 0;
            } else if (state < 26) {
                int c = state++;
                if (t[v].ch[c]) {
                    st.push_back({t[v].ch[c], -1});
                }
            } else {
                t[v].r = timer;
                st.pop_back();
            }
        }
    }
};

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n = 0) : n(n), bit(n + 1, 0) {}

    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    int sumPrefix(int idx) const {
        int res = 0;
        for (; idx > 0; idx -= idx & -idx) res += bit[idx];
        return res;
    }

    int sumRange(int l, int r) const {
        if (l > r) return 0;
        return sumPrefix(r) - sumPrefix(l - 1);
    }
};

struct Event {
    int y, q, coef, xl, xr;

    bool operator<(const Event& other) const {
        return y < other.y;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> genomes(n);
    size_t totalLen = 0;
    for (int i = 0; i < n; ++i) {
        cin >> genomes[i];
        totalLen += genomes[i].size();
    }

    Trie pref((int)totalLen);
    Trie suff((int)totalLen);
    pref.nextTerm.assign(n, -1);
    suff.nextTerm.assign(n, -1);

    for (int i = 0; i < n; ++i) {
        pref.insertString(genomes[i], i);

        string rev = genomes[i];
        reverse(rev.begin(), rev.end());
        suff.insertString(rev, i);
    }

    vector<int> x(n), y(n);
    pref.buildOrder(x);
    suff.buildOrder(y);

    vector<pair<int, int>> points(n);
    for (int i = 0; i < n; ++i) {
        points[i] = {y[i], x[i]};
    }
    sort(points.begin(), points.end());

    int m;
    cin >> m;

    vector<long long> ans(m, 0);
    vector<Event> events;
    events.reserve(2 * m);

    for (int qi = 0; qi < m; ++qi) {
        string p, s;
        cin >> p >> s;

        int vp = pref.findString(p);
        reverse(s.begin(), s.end());
        int vs = suff.findString(s);

        if (vp == -1 || vs == -1) continue;

        int xl = pref.t[vp].l;
        int xr = pref.t[vp].r;
        int yl = suff.t[vs].l;
        int yr = suff.t[vs].r;

        if (xl > xr || yl > yr) continue;

        events.push_back({yr, qi, +1, xl, xr});
        events.push_back({yl - 1, qi, -1, xl, xr});
    }

    sort(events.begin(), events.end());

    Fenwick fw(n);
    int ptr = 0;

    for (const Event& e : events) {
        while (ptr < n && points[ptr].first <= e.y) {
            fw.add(points[ptr].second, 1);
            ++ptr;
        }
        ans[e.q] += 1LL * e.coef * fw.sumRange(e.xl, e.xr);
    }

    for (int i = 0; i < m; ++i) {
        cout << ans[i] << '\n';
    }

    return 0;
}
