#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <functional>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;
    int M = N * K / 2;
    vector<pair<int, int>> edges;
    map<pair<int, int>, int> edge_index_map;

    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        edges.push_back({u, v});
        edge_index_map[{u, v}] = i + 1;
        edge_index_map[{v, u}] = i + 1;
    }

    // Build bipartite graph: left set [0, N-1], right set [N, 2N-1]
    vector<vector<pair<int, int>>> adj(N); // for left node u: list of (right_node, edge_index)
    for (int i = 0; i < M; i++) {
        int u = edges[i].first;
        int v = edges[i].second;
        adj[u].push_back({v + N, i + 1});
        adj[v].push_back({u + N, i + 1});
    }

    vector<int> matchL(N, -1); // matchL[u] = right node index (in [N, 2N-1]) for left node u
    vector<int> matchR(2 * N, -1); // matchR[v] = left node index for right node v
    vector<int> dist(N);

    auto bfs = [&]() -> bool {
        queue<int> q;
        for (int u = 0; u < N; u++) {
            if (matchL[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &edge : adj[u]) {
                int v_index = edge.first;
                if (matchR[v_index] == -1) {
                    found = true;
                } else if (dist[matchR[v_index]] == -1) {
                    dist[matchR[v_index]] = dist[u] + 1;
                    q.push(matchR[v_index]);
                }
            }
        }
        return found;
    };

    function<bool(int)> dfs = [&](int u) {
        for (auto &edge : adj[u]) {
            int v_index = edge.first;
            if (matchR[v_index] == -1 || (dist[matchR[v_index]] == dist[u] + 1 && dfs(matchR[v_index]))) {
                matchL[u] = v_index;
                matchR[v_index] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    };

    int matching_size = 0;
    while (bfs()) {
        for (int u = 0; u < N; u++) {
            if (matchL[u] == -1 && dfs(u)) {
                matching_size++;
            }
        }
    }

    if (matching_size != N) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
        vector<int> ans;
        for (int u = 0; u < N; u++) {
            int v_right = matchL[u];
            int v = v_right - N;
            int road = edge_index_map[{u, v}];
            ans.push_back(road);
        }
        for (int road : ans) {
            cout << road << '\n';
        }
    }

    return 0;
}
