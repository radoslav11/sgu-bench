#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

class Fenwick {
  public:
    int n;
    vector<int> tr;

    Fenwick(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        tr.assign(n + 1, 0);
    }

    void add(int x) {
        for(; x <= n; x += (x & -x)) {
            tr[x]++;
        }
    }

    int query(int x) {
        int ret = 0;
        for(; x > 0; x -= (x & -x)) {
            ret += tr[x];
        }

        return ret;
    }
};

int n;
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // Count inversions with a Fenwick tree over compressed values. We sort and
    // dedupe the values, map each element to its 1-based rank, then sweep left
    // to right inserting ranks. After inserting the i-th element (i + 1 seen so
    // far), the number of already-inserted values that are <= a[i] is
    // query(a[i]); the rest, i + 1 - query(a[i]), are strictly greater values
    // sitting to its left, i.e. inversions ending at position i.

    vector<int> li = a;
    sort(li.begin(), li.end());
    li.erase(unique(li.begin(), li.end()), li.end());

    for(int i = 0; i < n; i++) {
        a[i] = lower_bound(li.begin(), li.end(), a[i]) - li.begin() + 1;
    }

    Fenwick fen(li.size() + 1);

    uint32_t answer = 0;
    for(int i = 0; i < n; i++) {
        fen.add(a[i]);
        answer += i + 1 - fen.query(a[i]);
    }

    cout << answer << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
