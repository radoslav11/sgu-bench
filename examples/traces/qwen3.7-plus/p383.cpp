#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Point {
    long long x, y;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Point> pts(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> pts[i].x >> pts[i].y;
    }

    int q;
    cin >> q;

    vector<pair<int, int>> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].first >> queries[i].second;
    }

    if (n == 1) {
        for (int i = 0; i < q; i++) {
            cout << fixed << setprecision(10) << 0.0 << "\n";
        }
        return 0;
    }

    vector<bool> in_mst(n + 1, false);
    vector<int> parent_node(n + 1, -1);
    vector<long long> min_w2(n + 1, -1);

    in_mst[1] = true;
    for (int i = 2; i <= n; i++) {
        long long dx = pts[1].x - pts[i].x;
        long long dy = pts[1].y - pts[i].y;
        min_w2[i] = dx * dx + dy * dy;
        parent_node[i] = 1;
    }

    vector<vector<pair<int, double>>> adj(n + 1);

    for (int i = 1; i < n; i++) {
        int best_v = -1;
        long long best_w2 = -1;
        for (int v = 2; v <= n; v++) {
            if (!in_mst[v]) {
                if (best_v == -1 || min_w2[v] < best_w2) {
                    best_v = v;
                    best_w2 = min_w2[v];
                }
            }
        }

        in_mst[best_v] = true;
        double w = sqrt((double)best_w2);
        adj[best_v].push_back({parent_node[best_v], w});
        adj[parent_node[best_v]].push_back({best_v, w});

        for (int v = 2; v <= n; v++) {
            if (!in_mst[v]) {
                long long dx = pts[best_v].x - pts[v].x;
                long long dy = pts[best_v].y - pts[v].y;
                long long w2 = dx * dx + dy * dy;
                if (w2 < min_w2[v]) {
                    min_w2[v] = w2;
                    parent_node[v] = best_v;
                }
            }
        }
    }

    int LOG = 20;
    vector<vector<int>> up(n + 1, vector<int>(LOG, 0));
    vector<vector<double>> max_w(n + 1, vector<double>(LOG, 0.0));
    vector<int> depth(n + 1, 0);

    auto dfs = [&](auto& self, int u, int p, int d, double w_to_p) -> void {
        depth[u] = d;
        up[u][0] = p;
        max_w[u][0] = w_to_p;
        for (int j = 1; j < LOG; j++) {
            up[u][j] = up[up[u][j-1]][j-1];
            max_w[u][j] = max(max_w[u][j-1], max_w[up[u][j-1]][j-1]);
        }
        for (auto& edge : adj[u]) {
            int v = edge.first;
            double w = edge.second;
            if (v != p) {
                self(self, v, u, d + 1, w);
            }
        }
    };

    dfs(dfs, 1, 0, 0, 0.0);

    auto get_max_w = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        double ans = 0.0;
        int diff = depth[u] - depth[v];
        for (int j = 0; j < LOG; j++) {
            if ((diff >> j) & 1) {
                ans = max(ans, max_w[u][j]);
                u = up[u][j];
            }
        }
        if (u == v) return ans;
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                ans = max({ans, max_w[u][j], max_w[v][j]});
                u = up[u][j];
                v = up[v][j];
            }
        }
        ans = max({ans, max_w[u][0], max_w[v][0]});
        return ans;
    };

    for (int i = 0; i < q; i++) {
        int u = queries[i].first;
        int v = queries[i].second;
        if (u == v) {
            cout << fixed << setprecision(10) << 0.0 << "\n";
        } else {
            cout << fixed << setprecision(10) << get_max_w(u, v) << "\n";
        }
    }

    return 0;
}
