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

int n, m;
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n, vector<int>(n, 0));
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }
}

void solve() {
    // Decide whether the graph is the 3-SAT -> clique reduction graph. In
    // that reduction the 3n vertices split into n clauses of 3 vertices
    // each; vertices inside a clause are pairwise non-adjacent (a triple
    // anti-clique), and two vertices in different clauses are non-adjacent
    // exactly when they are contradictory literals of the same variable.
    //
    // First n must be divisible by 3. Then every vertex must belong to
    // exactly one triangle of mutual non-edges (its clause); we enumerate
    // all such anti-clique triples and require each vertex to appear in
    // precisely one. We add intra-clause edges so the remaining non-edges
    // are only the cross-clause contradictions.
    //
    // Those contradictions must pair literals as x / not-x: build a DSU on
    // 2n nodes (vertex i and its "negation" i+n). A non-edge (i, j) forces
    // i ~ j+n and i+n ~ j. The relation is consistent only if no i is in
    // the same class as i+n.
    //
    // Finally each variable class must contradict every literal of all the
    // other clauses exactly: for class of vertex i with cnt_a positive and
    // cnt_b negative occurrences, the number of cross non-edges it carries
    // (sum of complement-degrees) must equal 2 * cnt_a * cnt_b. If all
    // checks pass, print YES, else NO.

    if(n % 3 != 0) {
        cout << "NO\n";
        return;
    }

    vector<vector<tuple<int, int, int>>> three_anti_cliques_per_node(n);
    vector<int> clique_id(n, -1);
    int cnt_clique_id = 0;

    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(adj[i][j] == 0) {
                for(int k = j + 1; k < n; k++) {
                    if(adj[i][k] == 0 && adj[j][k] == 0) {
                        three_anti_cliques_per_node[i].emplace_back(j, k, i);
                        three_anti_cliques_per_node[j].emplace_back(i, k, j);
                        three_anti_cliques_per_node[k].emplace_back(i, j, k);
                        clique_id[i] = clique_id[j] = clique_id[k] =
                            cnt_clique_id++;
                    }
                }
            }
        }
    }

    for(int i = 0; i < n; i++) {
        if(three_anti_cliques_per_node[i].size() != 1) {
            cout << "NO\n";
            return;
        }
    }

    for(int i = 0; i < n; i++) {
        auto [j, k, l] = three_anti_cliques_per_node[i][0];
        adj[i][j] = 1;
        adj[j][i] = 1;
        adj[i][k] = 1;
        adj[k][i] = 1;
        adj[i][l] = 1;
        adj[l][i] = 1;
    }

    DSU dsu(2 * n);
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(adj[i][j] == 0) {
                dsu.unite(i, j + n);
                dsu.unite(i + n, j);
            }
        }
    }

    for(int i = 0; i < n; i++) {
        if(dsu.connected(i, i + n)) {
            cout << "NO\n";
            return;
        }
    }

    vector<int> deg(n, 0);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(adj[i][j] == 0) {
                deg[i]++;
            }
        }
    }

    for(int i = 0; i < n; i++) {
        int cnt_a = 0, cnt_b = 0, sum = 0;
        for(int v = 0; v < n; v++) {
            if(dsu.connected(i, v)) {
                cnt_a++;
                sum += deg[v];
            }
        }

        for(int v = 0; v < n; v++) {
            if(dsu.connected(i, v + n)) {
                cnt_b++;
                sum += deg[v];
            }
        }

        if(cnt_a * cnt_b * 2 != sum) {
            cout << "NO\n";
            return;
        }
    }

    cout << "YES\n";
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
