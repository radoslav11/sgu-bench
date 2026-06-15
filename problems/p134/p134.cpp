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

int n;
vector<vector<int>> adj;
vector<int> tr_sz;

void read() {
    cin >> n;
    adj.assign(n + 1, {});
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

void dfs(int u, int pr) {
    tr_sz[u] = 1;
    for(int v: adj[u]) {
        if(v != pr) {
            dfs(v, u);
            tr_sz[u] += tr_sz[v];
        }
    }
}

void solve() {
    /*
     * Centroid by subtree sizes. Root the tree at vertex 1 and compute tr_sz
     * via DFS. Removing vertex u leaves components: the "up" part of size
     * n - tr_sz[u], and one component per child v of size tr_sz[v]. The value
     * val[u] is the largest such component, i.e. max(n - tr_sz[u], max child
     * subtree). A neighbour v is a child iff tr_sz[v] < tr_sz[u] (its size is
     * smaller under this rooting). The centroids are all vertices attaining the
     * minimum val, listed in ascending order.
     */

    tr_sz.assign(n + 1, 0);
    dfs(1, 1);

    vector<int> val(n + 1, 0);
    int mn = n;
    for(int u = 1; u <= n; u++) {
        val[u] = n - tr_sz[u];
        for(int v: adj[u]) {
            if(tr_sz[v] < tr_sz[u]) {
                val[u] = max(val[u], tr_sz[v]);
            }
        }

        mn = min(mn, val[u]);
    }

    vector<int> li;
    for(int i = 1; i <= n; i++) {
        if(val[i] == mn) {
            li.push_back(i);
        }
    }

    cout << mn << " " << li.size() << '\n';
    for(int x: li) {
        cout << x << " ";
    }
    cout << '\n';
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
