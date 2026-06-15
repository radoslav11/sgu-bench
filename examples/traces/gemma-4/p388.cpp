#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge {
    int to, cap, cost, rev, id, u_orig, v_orig;
};

vector<Edge> adj[305];
int dist[305], parent_v[305], parent_e[305];

void add_edge(int u, int v, int cap, int cost, int id = 0, int u_orig = -1, int v_orig = -1) {
    adj[u].push_back({v, cap, cost, (int)adj[v].size(), id, u_orig, v_orig});
    adj[v].push_back({u, 0, -cost, (int)adj[u].size() - 1, -1, -1, -1});
}

int main() {
    int n, m1, m2;
    if (!(cin >> n >> m1 >> m2)) return 0;
    vector<pair<int, int>> e1(m1), e2(m2);
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m1; ++i) {
        cin >> e1[i].first >> e1[i].second;
        g[e1[i].first].push_back(e1[i].second);
        g[e1[i].second].push_back(e1[i].first);
    }
    for (int i = 0; i < m2; ++i) {
        cin >> e2[i].first >> e2[i].second;
        g[e2[i].first].push_back(e2[i].second);
        g[e2[i].second].push_back(e2[i].first);
    }
    vector<int> color(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        if (color[i] == 0) {
            queue<int> q; q.push(i); color[i] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : g[u]) {
                    if (color[v] == 0) { color[v] = 3 - color[u]; q.push(v); }
                }
            }
        }
    }
    for (int i = 1; i <= n; ++i) add_edge(i, i + n, 1, -1);
    for (int i = 0; i < m1; ++i) {
        int u = e1[i].first, v = e1[i].second;
        if (color[u] == 2) swap(u, v);
        add_edge(u + n, v, 1, 0, 1, e1[i].first, e1[i].second);
    }
    for (int i = 0; i < m2; ++i) {
        int u = e2[i].first, v = e2[i].second;
        if (color[u] == 2) swap(u, v);
        add_edge(v + n, u, 1, 0, 2, e2[i].first, e2[i].second);
    }

    vector<pair<int, int>> juans, rosas;
    while (true) {
        fill(dist, dist + 2 * n + 1, INF);
        fill(parent_v, parent_v + 2 * n + 1, -1);
        fill(parent_e, parent_e + 2 * n + 1, -1);
        vector<int> count(2 * n + 1, 0);
        vector<bool> in_q(2 * n + 1, false);
        queue<int> q;
        for (int i = 1; i <= 2 * n; ++i) { dist[i] = 0; q.push(i); in_q[i] = true; }
        int cycle_node = -1;
        while (!q.empty()) {
            int u = q.front(); q.pop(); in_q[u] = false;
            for (int i = 0; i < adj[u].size(); ++i) {
                Edge &e = adj[u][i];
                if (e.cap > 0 && dist[e.to] > dist[u] + e.cost) {
                    dist[e.to] = dist[u] + e.cost;
                    parent_v[e.to] = u; parent_e[e.to] = i;
                    if (!in_q[e.to]) {
                        q.push(e.to); in_q[e.to] = true;
                        if (++count[e.to] > 2 * n) { cycle_node = e.to; goto found; }
                    }
                }
            }
        }
        break;
    found:
        int curr = cycle_node;
        for (int i = 0; i <= 2 * n; ++i) curr = parent_v[curr];
        int start = curr;
        vector<int> nodes;
        int temp = start;
        do { nodes.push_back(temp); temp = parent_v[temp]; } while (temp != start);
        for (int i = 0; i < nodes.size(); ++i) {
            int v = nodes[i];
            int u = parent_v[v];
            int idx = parent_e[v];
            adj[u][idx].cap--; adj[v][adj[u][idx].rev].cap++;
            if (adj[u][idx].id == 1) juans.push_back({adj[u][idx].u_orig, adj[u][idx].v_orig});
            if (adj[u][idx].id == 2) rosas.push_back({adj[u][idx].u_orig, adj[u][idx].v_orig});
        }
    }
    cout << juans.size() << "\n";
    for (auto p : juans) cout << p.first << " " << p.second << "\n";
    for (auto p : rosas) cout << p.first << " " << p.second << "\n";
    return 0;
}

