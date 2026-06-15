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
vector<int> deg;

void read() {
    cin >> n >> m;
    deg.resize(n);
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        deg[u]++;
        deg[v]++;
    }
}

void solve() {
    // - Entry (j, k) of A^T A counts vertices incident to both edges j and k, so
    //   its value is 1 when edges j and k share an endpoint (including j == k).
    //   The total sum therefore counts all ordered pairs of edges sharing a
    //   vertex.
    //
    // - For a vertex of degree d there are d * (d - 1) ordered pairs of distinct
    //   edges meeting at it, plus the m diagonal entries counted as 2 * m
    //   (each edge shares both of its two endpoints with itself). Summing
    //   deg[i] * (deg[i] - 1) over vertices and adding 2 * m gives the answer.

    int64_t ans = 0;
    for(int i = 0; i < n; i++) {
        ans += (int64_t)deg[i] * (deg[i] - 1);
    }

    ans += m * 2;
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
