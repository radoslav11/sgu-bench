#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    int weight;
    int id;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 1; i <= m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w, i});
        adj[v].push_back({u, w, i});
    }

    int k;
    cin >> k;
    vector<int> S(k);
    for (int i = 0; i < k; ++i) {
        cin >> S[i];
    }

    if (k == 0 || k == 1) {
        cout << 0 << "\n\n";
        return 0;
    }

    auto dijkstra = [&](int start, vector<int>& dist) {
        dist.assign(n + 1, 2e9);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist[start] = 0;
        pq.push({0, start});
        while (!pq.empty()) {
            auto [d_u, u] = pq.top();
            pq.pop();
            if (d_u > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.to] > dist[u] + edge.weight) {
                    dist[edge.to] = dist[u] + edge.weight;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
    };

    vector<int> dist_s;
    dijkstra(S[0], dist_s);

    int u = S[0];
    int max_d = -1;
    for (int city : S) {
        if (dist_s[city] > max_d) {
            max_d = dist_s[city];
            u = city;
        }
    }

    vector<int> dist_u;
    dijkstra(u, dist_u);

    int v = u;
    max_d = -1;
    for (int city : S) {
        if (dist_u[city] > max_d) {
            max_d = dist_u[city];
            v = city;
        }
    }

    vector<int> sorted_S = S;
    sort(sorted_S.begin(), sorted_S.end(), [&](int a, int b) {
        return dist_u[a] < dist_u[b];
    });

    vector<int> final_path;
    vector<bool> visited(n + 1, false);

    for (int i = 0; i < k - 1; ++i) {
        int target = sorted_S[i];
        int start = sorted_S[i + 1];
        int min_d = dist_u[target];

        vector<int> current_segment;
        auto dfs = [&](auto& self, int curr) -> bool {
            if (curr == target) return true;
            if (visited[curr]) return false;
            visited[curr] = true;
            for (const auto& edge : adj[curr]) {
                int next_node = edge.to;
                int w = edge.weight;
                if (dist_u[next_node] + w == dist_u[curr] && dist_u[next_node] >= min_d) {
                    if (!visited[next_node]) {
                        if (self(self, next_node)) {
                            current_segment.push_back(edge.id);
                            return true;
                        }
                    }
                }
            }
            return false;
        };

        if (dfs(dfs, start)) {
            reverse(current_segment.begin(), current_segment.end());
            for (int id : current_segment) {
                final_path.push_back(id);
            }
        }
    }

    cout << final_path.size() << "\n";
    for (int i = 0; i < (int)final_path.size(); ++i) {
        cout << final_path[i] << (i + 1 == (int)final_path.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
