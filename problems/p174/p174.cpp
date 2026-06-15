#include <bits/stdc++.h>
// #include <coding_library/graph/dsu.hpp>

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

class DSU {
  public:
    int n;
    vector<int> par;
    vector<int> sz;

    DSU(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        par.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        for(int i = 0; i <= n; i++) {
            par[i] = i;
            sz[i] = 1;
        }
    }

    int root(int u) { return par[u] = ((u == par[u]) ? u : root(par[u])); }
    bool connected(int x, int y) { return root(x) == root(y); }

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) {
            return x;
        }
        if(sz[x] > sz[y]) {
            swap(x, y);
        }
        par[x] = y;
        sz[y] += sz[x];
        return y;
    }
};

int m;

void read() { cin >> m; }

void solve() {
    // The walls form a planar graph whose vertices are the endpoints; a region
    // becomes enclosed exactly when adding a wall creates the first cycle.
    // Maintain a DSU over endpoints (each distinct coordinate pair gets an id
    // via get_id). For each wall, if its two endpoints are already in the same
    // component the wall closes a cycle, so print its 1-based index; otherwise
    // union the components. If no wall ever closes a cycle, print 0.

    DSU dsu(2 * m);
    map<pair<int, int>, int> mp;

    function<int(int, int)> get_id = [&](int x, int y) {
        if(mp.count({x, y})) {
            return mp[{x, y}];
        }
        return mp[{x, y}] = mp.size();
    };

    for(int i = 0; i < m; i++) {
        int x, y, a, b;
        cin >> x >> y >> a >> b;
        if(dsu.connected(get_id(x, y), get_id(a, b))) {
            cout << i + 1 << '\n';
            return;
        }

        dsu.unite(get_id(x, y), get_id(a, b));
    }

    cout << 0 << '\n';
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
