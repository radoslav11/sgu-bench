#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <bitset>

using namespace std;

struct Edge {
    int u, v;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<Edge> edges(m + 1);
    vector<vector<int>> adj_orig(n + 1);
    vector<vector<int>> rev_orig(n + 1);

    for (int i = 1; i <= m; ++i) {
        cin >> edges[i].u >> edges[i].v;
        adj_orig[edges[i].u].push_back(edges[i].v);
        rev_orig[edges[i].v].push_back(edges[i].u);
    }

    vector<bool> visited(n + 1, false);
    vector<int> order;
    order.reserve(n);

    auto dfs1 = [&](auto& self, int u) -> void {
        visited[u] = true;
        for (int v : adj_orig[u]) {
            if (!visited[v]) self(self, v);
        }
        order.push_back(u);
    };

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) dfs1(dfs1, i);
    }

    vector<int> comp(n + 1, 0);
    int k = 0;
    vector<int> sz(n + 1, 0);

    auto dfs2 = [&](auto& self, int u, int c) -> void {
        comp[u] = c;
        sz[c]++;
        for (int v : rev_orig[u]) {
            if (comp[v] == 0) self(self, v, c);
        }
    };

    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        if (comp[u] == 0) {
            k++;
            dfs2(dfs2, u, k);
        }
    }

    int M_0 = 0;
    for (int i = 1; i <= k; ++i) {
        if (sz[i] > M_0) {
            M_0 = sz[i];
        }
    }

    vector<vector<int>> adj(k + 1);
    vector<vector<int>> rev_adj(k + 1);

    for (int i = 1; i <= m; ++i) {
        int cu = comp[edges[i].u];
        int cv = comp[edges[i].v];
        if (cu != cv) {
            adj[cu].push_back(cv);
            rev_adj[cv].push_back(cu);
        }
    }

    for (int i = 1; i <= k; ++i) {
        sort(adj[i].begin(), adj[i].end());
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
        sort(rev_adj[i].begin(), rev_adj[i].end());
        rev_adj[i].erase(unique(rev_adj[i].begin(), rev_adj[i].end()), rev_adj[i].end());
    }

    vector<int> indeg(k + 1, 0);
    for (int u = 1; u <= k; ++u) {
        for (int v : adj[u]) {
            indeg[v]++;
        }
    }

    queue<int> q;
    for (int i = 1; i <= k; ++i) {
        if (indeg[i] == 0) q.push(i);
    }

    vector<int> topo;
    topo.reserve(k);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);
        for (int v : adj[u]) {
            if (--indeg[v] == 0) {
                q.push(v);
            }
        }
    }

    vector<bitset<2048>> down(k + 1);
    for (int i = k - 1; i >= 0; --i) {
        int u = topo[i];
        down[u].set(u);
        for (int v : adj[u]) {
            down[u] |= down[v];
        }
    }

    vector<bitset<2048>> up(k + 1);
    for (int i = 0; i < k; ++i) {
        int u = topo[i];
        up[u].set(u);
        for (int v : rev_adj[u]) {
            up[u] |= up[v];
        }
    }

    int max_w = M_0;
    vector<int> edge_w(m + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int cu = comp[edges[i].u];
        int cv = comp[edges[i].v];
        if (cu == cv) {
            edge_w[i] = M_0;
        } else {
            int current_size = 0;
            bitset<2048> intersect = down[cu] & up[cv];
            for (int j = 1; j <= k; ++j) {
                if (intersect.test(j)) {
                    current_size += sz[j];
                }
            }
            edge_w[i] = max(M_0, current_size);
        }
        if (edge_w[i] > max_w) {
            max_w = edge_w[i];
        }
    }

    vector<int> ans;
    for (int i = 1; i <= m; ++i) {
        if (edge_w[i] == max_w) {
            ans.push_back(i);
        }
    }

    cout << max_w << "\n";
    cout << ans.size() << "\n";
    for (size_t i = 0; i < ans.size(); ++i) {
        cout << ans[i] << (i + 1 == ans.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
