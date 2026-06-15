#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int V, E;
    if (!(cin >> V >> E)) return 0;

    if (V % 3 != 0) {
        cout << "NO\n";
        return 0;
    }

    vector<vector<bool>> adj(V + 1, vector<bool>(V + 1, false));
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u][v] = true;
        adj[v][u] = true;
    }

    vector<vector<bool>> H(V + 1, vector<bool>(V + 1, false));
    for (int u = 1; u <= V; ++u) {
        for (int v = 1; v <= V; ++v) {
            if (u == v) {
                H[u][v] = true;
            } else {
                H[u][v] = !adj[u][v];
            }
        }
    }

    vector<int> n1(V + 1, -1), n2(V + 1, -1);
    for (int u = 1; u <= V; ++u) {
        vector<int> neighbors;
        for (int v = 1; v <= V; ++v) {
            if (u != v && H[u][v]) {
                neighbors.push_back(v);
            }
        }
        int edge_count = 0;
        int edge_v1 = -1, edge_v2 = -1;
        for (size_t i = 0; i < neighbors.size(); ++i) {
            for (size_t j = i + 1; j < neighbors.size(); ++j) {
                if (H[neighbors[i]][neighbors[j]]) {
                    edge_count++;
                    edge_v1 = neighbors[i];
                    edge_v2 = neighbors[j];
                }
            }
        }
        if (edge_count != 1) {
            cout << "NO\n";
            return 0;
        }
        n1[u] = edge_v1;
        n2[u] = edge_v2;
    }

    vector<int> clause_id(V + 1, -1);
    int c_id = 0;
    for (int u = 1; u <= V; ++u) {
        if (clause_id[u] == -1) {
            int v1 = n1[u];
            int v2 = n2[u];
            if (v1 == -1 || v2 == -1) {
                cout << "NO\n";
                return 0;
            }
            vector<int> c = {u, v1, v2};
            if (c[0] == c[1] || c[0] == c[2] || c[1] == c[2]) {
                cout << "NO\n";
                return 0;
            }
            for (int x : c) {
                if (clause_id[x] != -1) {
                    cout << "NO\n";
                    return 0;
                }
                clause_id[x] = c_id;
            }
            c_id++;
        }
    }

    if (c_id != V / 3) {
        cout << "NO\n";
        return 0;
    }

    vector<vector<int>> H_prime(V + 1);
    for (int u = 1; u <= V; ++u) {
        for (int v = 1; v <= V; ++v) {
            if (u != v && H[u][v] && clause_id[u] != clause_id[v]) {
                H_prime[u].push_back(v);
            }
        }
    }

    vector<int> color(V + 1, -1);
    for (int u = 1; u <= V; ++u) {
        if (color[u] == -1) {
            queue<int> q;
            q.push(u);
            color[u] = 0;
            vector<int> comp;
            bool bipartite = true;
            while (!q.empty()) {
                int curr = q.front();
                q.pop();
                comp.push_back(curr);
                for (int nxt : H_prime[curr]) {
                    if (color[nxt] == -1) {
                        color[nxt] = 1 - color[curr];
                        q.push(nxt);
                    } else if (color[nxt] == color[curr]) {
                        bipartite = false;
                    }
                }
            }
            if (!bipartite) {
                cout << "NO\n";
                return 0;
            }
            vector<bool> seen_c(c_id, false);
            for (int x : comp) {
                if (seen_c[clause_id[x]]) {
                    cout << "NO\n";
                    return 0;
                }
                seen_c[clause_id[x]] = true;
            }
        }
    }

    cout << "YES\n";
    return 0;
}
