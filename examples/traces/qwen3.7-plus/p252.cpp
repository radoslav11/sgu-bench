#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge {
    int to;
    int cap;
    int flow;
    int cost;
    int rev;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int S = 0;
    int T = 2 * n + 1;
    int nodes = T + 1;

    vector<vector<Edge>> adj(nodes);

    auto add_edge = [&](int from, int to, int cap, int cost) {
        adj[from].push_back({to, cap, 0, cost, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, -cost, (int)adj[from].size() - 1});
    };

    for (int i = 1; i <= n; ++i) {
        add_edge(S, i, 1, 0);
        add_edge(n + i, T, 1, 0);
    }

    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        add_edge(u, n + v, 1, c);
    }

    int total_flow = 0;
    int total_cost = 0;

    while (true) {
        vector<int> dist(nodes, INF);
        vector<int> parent_node(nodes, -1);
        vector<int> parent_edge(nodes, -1);
        vector<bool> in_queue(nodes, false);
        queue<int> q;

        dist[S] = 0;
        q.push(S);
        in_queue[S] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = false;

            for (int i = 0; i < (int)adj[u].size(); ++i) {
                auto& edge = adj[u][i];
                if (edge.cap - edge.flow > 0 && dist[edge.to] > dist[u] + edge.cost) {
                    dist[edge.to] = dist[u] + edge.cost;
                    parent_node[edge.to] = u;
                    parent_edge[edge.to] = i;
                    if (!in_queue[edge.to]) {
                        q.push(edge.to);
                        in_queue[edge.to] = true;
                    }
                }
            }
        }

        if (dist[T] == INF) {
            break;
        }

        int push = INF;
        int curr = T;
        while (curr != S) {
            int p = parent_node[curr];
            int idx = parent_edge[curr];
            push = min(push, adj[p][idx].cap - adj[p][idx].flow);
            curr = p;
        }

        curr = T;
        while (curr != S) {
            int p = parent_node[curr];
            int idx = parent_edge[curr];
            int rev_idx = adj[p][idx].rev;
            adj[p][idx].flow += push;
            adj[curr][rev_idx].flow -= push;
            total_cost += push * adj[p][idx].cost;
            curr = p;
        }
        total_flow += push;
    }

    int num_paths = n - total_flow;

    vector<int> next_node(n + 1, -1);
    vector<int> in_degree(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        for (auto& edge : adj[i]) {
            if (edge.flow == 1 && edge.to > n && edge.to <= 2 * n) {
                int next_v = edge.to - n;
                next_node[i] = next_v;
                in_degree[next_v]++;
            }
        }
    }

    vector<vector<int>> paths;
    for (int i = 1; i <= n; ++i) {
        if (in_degree[i] == 0) {
            vector<int> path;
            int curr = i;
            while (curr != -1) {
                path.push_back(curr);
                curr = next_node[curr];
            }
            paths.push_back(path);
        }
    }

    cout << num_paths << " " << total_cost << "\n";
    for (auto& path : paths) {
        cout << path.size();
        for (int node : path) {
            cout << " " << node;
        }
        cout << "\n";
    }

    return 0;
}
