#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
#include <climits>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, int>> edges;
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        edges.push_back(make_tuple(a, b, c));
    }

    vector<vector<pair<int, int>>> tree(n);
    for (int i = 0; i < n-1; i++) {
        int a = get<0>(edges[i]);
        int b = get<1>(edges[i]);
        tree[a].push_back({b, i});
        tree[b].push_back({a, i});
    }

    vector<int> parent(n, -1);
    vector<int> depth(n, -1);
    queue<int> q;
    q.push(0);
    depth[0] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &edge : tree[u]) {
            int v = edge.first;
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> ans(m);
    for (int i = 0; i < m; i++) {
        ans[i] = get<2>(edges[i]);
    }

    for (int iter = 0; iter < 1000; iter++) {
        bool changed = false;

        for (int i = n-1; i < m; i++) {
            int u = get<0>(edges[i]);
            int v = get<1>(edges[i]);
            vector<int> path_nodes;
            int a = u, b = v;
            while (a != b) {
                if (depth[a] > depth[b]) {
                    path_nodes.push_back(a);
                    a = parent[a];
                } else if (depth[b] > depth[a]) {
                    path_nodes.push_back(b);
                    b = parent[b];
                } else {
                    path_nodes.push_back(a);
                    path_nodes.push_back(b);
                    a = parent[a];
                    b = parent[b];
                }
            }
            path_nodes.push_back(a);

            int max_val = 0;
            for (int j = 0; j < (int)path_nodes.size() - 1; j++) {
                int x = path_nodes[j];
                int y = path_nodes[j+1];
                for (auto &edge : tree[x]) {
                    if (edge.first == y) {
                        int idx = edge.second;
                        if (idx < n-1) {
                            if (ans[idx] > max_val) {
                                max_val = ans[idx];
                            }
                        }
                        break;
                    }
                }
            }
            if (ans[i] < max_val) {
                ans[i] = max_val;
                changed = true;
            }
        }

        for (int i = 0; i < n-1; i++) {
            int a = get<0>(edges[i]);
            int b = get<1>(edges[i]);
            vector<bool> comp(n, false);
            queue<int> qq;
            qq.push(a);
            comp[a] = true;
            while (!qq.empty()) {
                int u = qq.front(); qq.pop();
                for (auto &edge : tree[u]) {
                    int v = edge.first;
                    int idx = edge.second;
                    if (idx == i) continue;
                    if (!comp[v]) {
                        comp[v] = true;
                        qq.push(v);
                    }
                }
            }

            int min_val = INT_MAX;
            for (int j = n-1; j < m; j++) {
                int u = get<0>(edges[j]);
                int v = get<1>(edges[j]);
                if (comp[u] != comp[v]) {
                    if (ans[j] < min_val) {
                        min_val = ans[j];
                    }
                }
            }
            if (min_val < ans[i]) {
                ans[i] = min_val;
                changed = true;
            }
        }

        if (!changed) break;
    }

    for (int i = 0; i < m; i++) {
        cout << ans[i] << '\n';
    }

    return 0;
}
