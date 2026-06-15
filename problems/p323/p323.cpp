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

    vector<vector<int>> components() {
        vector<vector<int>> comp(n + 1);
        for(int i = 0; i <= n; i++) {
            comp[root(i)].push_back(i);
        }
        return comp;
    }
};

using Edge = tuple<int, int, int, int, int>;

int n, m, k;
vector<Edge> edges;
vector<vector<pair<int, int>>> airlines;

void read() {
    cin >> n >> m >> k;
    edges.resize(k);
    airlines.assign(m, {});
    int idx = 0;
    for(auto& [p, c, u, v, i]: edges) {
        cin >> u >> v >> c >> p;
        i = idx++;
        u--;
        v--;
        c--;

        airlines[c].push_back({u, v});
    }
}

void solve() {
    // We must pick one airline to survive and buy enough other flights (each
    // at its tax cost) so the survivor's set of flights connects all towns.
    // For a fixed surviving airline this is a minimum spanning forest problem:
    // start with the survivor's own flights already contracted (cost 0), then
    // add the cheapest remaining flights to finish connecting everything.
    //
    // Key reduction: the only flights ever worth buying belong to the global
    // minimum spanning tree built over all flights by tax. Any non-MST edge
    // can always be replaced by a cheaper MST path, so we first Kruskal over
    // all K edges to keep just the n-1 relevant tree edges.
    //
    // Then for each candidate airline we run a DSU pre-seeded with that
    // airline's free flights and Kruskal over the relevant edges (already
    // sorted by tax), summing the bought taxes and recording their indices.
    // We take the airline giving the minimum total cost.

    sort(edges.begin(), edges.end());

    DSU global_mst(n);
    vector<Edge> relevant;
    for(auto [p, c, u, v, i]: edges) {
        if(!global_mst.connected(u, v)) {
            global_mst.unite(u, v);
            relevant.push_back({p, c, u, v, i});
        }
    }

    int64_t best_cost = LLONG_MAX;
    int best_airline = -1;
    vector<int> best_purchases;

    for(int airline_to_keep = 0; airline_to_keep < m; airline_to_keep++) {
        DSU d(n);
        for(auto [u, v]: airlines[airline_to_keep]) {
            d.unite(u, v);
        }

        int64_t curr_ans = 0;
        vector<int> purchases;
        for(auto [p, c, u, v, i]: relevant) {
            if(!d.connected(u, v)) {
                d.unite(u, v);
                curr_ans += p;
                purchases.push_back(i + 1);
            }
        }

        if(curr_ans < best_cost) {
            best_cost = curr_ans;
            best_airline = airline_to_keep;
            best_purchases = purchases;
        }
    }

    cout << best_cost << ' ' << best_airline + 1 << ' ' << best_purchases.size()
         << '\n';
    for(int idx: best_purchases) {
        cout << idx << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
