#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, d1, d2;
        cin >> u >> v >> d1 >> d2;
        int w = (d1 == d2) ? 1 : 0;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    vector<bool> visited(n + 1, false);
    vector<int> color(n + 1, -1);
    vector<int> ans;
    bool possible = true;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            vector<int> q;
            q.push_back(i);
            visited[i] = true;
            color[i] = 0;

            int head = 0;
            int count0 = 0, count1 = 0;

            while (head < (int)q.size()) {
                int u = q[head++];
                if (color[u] == 0) count0++;
                else count1++;

                for (auto& edge : adj[u]) {
                    int v = edge.first;
                    int w = edge.second;
                    if (!visited[v]) {
                        visited[v] = true;
                        color[v] = color[u] ^ w;
                        q.push_back(v);
                    } else {
                        if (color[v] != (color[u] ^ w)) {
                            possible = false;
                        }
                    }
                }
            }

            if (!possible) break;

            if (count1 < count0) {
                for (int v : q) {
                    if (color[v] == 1) {
                        ans.push_back(v);
                    }
                }
            } else {
                for (int v : q) {
                    if (color[v] == 0) {
                        ans.push_back(v);
                    }
                }
            }
        }
    }

    if (!possible) {
        cout << "NO\n";
    } else {
        cout << "YES\n";
        cout << ans.size() << "\n";
        for (int v : ans) {
            cout << v << "\n";
        }
    }

    return 0;
}
