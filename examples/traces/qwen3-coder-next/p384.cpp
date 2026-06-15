#include <bits/stdc++.h>
using namespace std;

const int INF = 1000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges;
    vector<int> deg(n + 1, 0);
    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
        deg[u]++;
        deg[v]++;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int center = 1;
    if (n > 1) {
        for (int i = 1; i <= n; i++) {
            if (deg[i] == n - 1) {
                center = i;
                break;
            }
        }
    }

    vector<int> partner(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        if (i == center) continue;
        for (int neighbor : adj[i]) {
            if (neighbor != center) {
                partner[i] = neighbor;
                break;
            }
        }
    }

    set<pair<int, int>> delSet;

    string op;
    while (cin >> op) {
        if (op == "DELETE") {
            int idx;
            cin >> idx;
            idx--;
            int u = edges[idx].first, v = edges[idx].second;
            delSet.insert({min(u, v), max(u, v)});
        } else if (op == "LENGTH") {
            int u, v;
            cin >> u >> v;

            if (u == v) {
                cout << 0 << '\n';
                continue;
            }

            auto getDist = [&](int x) -> int {
                if (x == center) {
                    return 0;
                }
                auto it = delSet.find({min(center, x), max(center, x)});
                if (it == delSet.end()) {
                    return 1;
                }
                int p = partner[x];
                if (p == 0) {
                    return INF;
                }
                auto it1 = delSet.find({min(center, p), max(center, p)});
                auto it2 = delSet.find({min(x, p), max(x, p)});
                if (it1 == delSet.end() && it2 == delSet.end()) {
                    return 2;
                }
                return INF;
            };

            int d1 = getDist(u);
            int d2 = getDist(v);

            if (d1 == INF || d2 == INF) {
                if (d1 == INF && d2 == INF) {
                    if (partner[u] == v && delSet.find({min(u, v), max(u, v)}) == delSet.end()) {
                        cout << 1 << '\n';
                    } else {
                        cout << -1 << '\n';
                    }
                } else {
                    cout << -1 << '\n';
                }
            } else {
                if (u == center) {
                    cout << d2 << '\n';
                } else if (v == center) {
                    cout << d1 << '\n';
                } else {
                    if (partner[u] == v && delSet.find({min(u, v), max(u, v)}) == delSet.end()) {
                        cout << 1 << '\n';
                    } else {
                        cout << d1 + d2 << '\n';
                    }
                }
            }
        }
    }

    return 0;
}
