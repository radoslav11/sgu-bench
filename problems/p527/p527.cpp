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

int n, m;
vector<string> tbl;

void read() {
    cin >> n >> m;
    tbl.resize(n);
    cin >> tbl;
}

void solve() {
    // Each bomb clears one full row and one full column at once, so if a plan
    // bombs r distinct rows and c distinct columns we can pair them up and only
    // need max(r, c) bombs. We enumerate every subset i of the n rows that we
    // choose to bomb: those rows are cleared completely, and the rocks in the
    // remaining rows must be killed by bombing their columns. mask[s] holds the
    // union (bitmask over columns) of rock columns across the rows in set s,
    // built by SOS-style merging of single-row masks. For each chosen row set i
    // the needed columns are mask of the complement, and the cost is
    // max(popcount(rows), popcount(columns)); we minimise this over all subsets.

    vector<int> mask(1 << n, 0);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tbl[i][j] == '*') {
                mask[1 << i] |= 1 << j;
            }
        }
    }

    for(int i = 1; i < (1 << n); i++) {
        int l = i & -i;
        mask[i] = mask[l] | mask[i ^ l];
    }

    int ans = n + m;
    for(int i = 0; i < (1 << n); i++) {
        int mask_m = mask[((1 << n) - 1) ^ i];
        ans = min(ans, max(__builtin_popcount(i), __builtin_popcount(mask_m)));
    }

    cout << ans << '\n';
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
