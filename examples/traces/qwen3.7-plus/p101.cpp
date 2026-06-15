#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int to;
    int id;
    bool is_rotated;
};

vector<Edge> adj[7];
int degree_cnt[7];
bool used_edge[105];
vector<pair<int, bool>> path;

void euler(int u) {
    while (!adj[u].empty()) {
        Edge edge = adj[u].back();
        adj[u].pop_back();
        if (used_edge[edge.id]) continue;
        used_edge[edge.id] = true;
        euler(edge.to);
        path.push_back({edge.id, edge.is_rotated});
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    for (int i = 1; i <= N; i++) {
        int u, v;
        cin >> u >> v;
        degree_cnt[u]++;
        degree_cnt[v]++;
        adj[u].push_back({v, i, false});
        adj[v].push_back({u, i, true});
    }

    int start_vertex = -1;
    int non_zero_count = 0;
    for (int i = 0; i <= 6; i++) {
        if (degree_cnt[i] > 0) {
            non_zero_count++;
            if (start_vertex == -1) {
                start_vertex = i;
            }
        }
    }

    if (start_vertex == -1) {
        cout << "No solution\n";
        return 0;
    }

    vector<bool> vis(7, false);
    queue<int> q;
    q.push(start_vertex);
    vis[start_vertex] = true;
    int reached_count = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        reached_count++;
        for (const auto& edge : adj[u]) {
            if (!vis[edge.to] && degree_cnt[edge.to] > 0) {
                vis[edge.to] = true;
                q.push(edge.to);
            }
        }
    }

    if (reached_count != non_zero_count) {
        cout << "No solution\n";
        return 0;
    }

    int odd_count = 0;
    for (int i = 0; i <= 6; i++) {
        if (degree_cnt[i] % 2 != 0) {
            odd_count++;
        }
    }

    if (odd_count > 2) {
        cout << "No solution\n";
        return 0;
    }

    if (odd_count == 2) {
        for (int i = 0; i <= 6; i++) {
            if (degree_cnt[i] % 2 != 0) {
                start_vertex = i;
                break;
            }
        }
    }

    euler(start_vertex);

    if ((int)path.size() != N) {
        cout << "No solution\n";
        return 0;
    }

    for (int i = (int)path.size() - 1; i >= 0; i--) {
        cout << path[i].first << " " << (path[i].second ? "-" : "+") << "\n";
    }

    return 0;
}
