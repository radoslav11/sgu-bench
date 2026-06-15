#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 1e9;

int n, m;
vector<vector<int>> adj;
vector<int> color;

bool is_bipartite() {
    color.assign(n + 1, -1);
    for (int i = 1; i <= n; ++i) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

vector<int> match_u, match_v;
vector<int> dist;

bool bfs() {
    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (match_u[i] == 0) {
            dist[i] = 0;
            q.push(i);
        } else {
            dist[i] = INF;
        }
    }
    dist[0] = INF;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (dist[u] < dist[0]) {
            for (int v : adj[u]) {
                if (dist[match_v[v]] == INF) {
                    dist[match_v[v]] = dist[u] + 1;
                    q.push(match_v[v]);
                }
            }
        }
    }
    return dist[0] != INF;
}

bool dfs(int u) {
    if (u != 0) {
        for (int v : adj[u]) {
            if (dist[match_v[v]] == dist[u] + 1) {
                if (dfs(match_v[v])) {
                    match_v[v] = u;
                    match_u[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INF;
        return false;
    }
    return true;
}

int hopcroft_karp() {
    match_u.assign(n + 1, 0);
    match_v.assign(n + 1, 0);
    dist.assign(n + 1, 0);
    int result = 0;
    while (bfs()) {
        for (int i = 1; i <= n; ++i) {
            if (match_u[i] == 0 && dfs(i)) {
                result++;
            }
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n >> m)) return 0;
    
    adj.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    if (!is_bipartite()) {
        cout << 1 << "\n";
        return 0;
    }

    int max_match = hopcroft_karp();
    if (max_match * 2 == n) {
        cout << 2 << "\n";
    } else {
        cout << 1 << "\n";
    }

    return 0;
}
