#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * We need to find the maximum number of disjoint S-T cuts.
 * Let d(v) be the shortest distance from star S to star v.
 * The number of disjoint S-T cuts is equal to the length of the shortest S-T path,
 * which is d(T).
 * 
 * For each i in {1, ..., d(T)}, we define a cut E_i as the set of all edges (u, v) 
 * such that min(d(u), d(v)) = i - 1 and max(d(u), d(v)) = i.
 * 
 * Proof:
 * 1. Every edge (u, v) satisfies |d(u) - d(v)| <= 1.
 *    This is because if |d(u) - d(v)| > 1, then one could find a shorter path, 
 *    contradicting the BFS distance properties.
 * 2. For any S-T path, the distance from S increases from 0 to d(T) in steps of 
 *    at most 1. Thus, for each i from 1 to d(T), the path must include at least 
 *    one edge (u, v) where d(u) = i-1 and d(v) = i.
 * 3. Therefore, E_i is an S-T cut.
 * 4. Since each edge (u, v) has a unique pair {d(u), d(v)}, it can belong to 
 *    at most one E_i (the one where i = min(d(u), d(v)) + 1, provided |d(u)-d(v)|=1).
 *    So all E_i are disjoint.
 * 5. Since each E_i must contain at least one edge from every S-T path, and 
 *    the shortest path has d(T) edges, the maximum number of disjoint cuts 
 *    is at most d(T). Our construction reaches this.
 */

struct Edge {
    int u, v;
};

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, S, T;
    if (!(cin >> N >> M >> S >> T)) return 0;

    vector<Edge> edges(M + 1);
    vector<vector<pair<int, int>>> adj(N + 1);
    for (int i = 1; i <= M; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    // BFS to find shortest distance from S to all nodes
    vector<int> dist(N + 1, -1);
    queue<int> q;

    dist[S] = 0;
    q.push(S);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto& neighbor : adj[u]) {
            int v = neighbor.first;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    // The number of crystal types is the shortest distance to T
    int num_crystals = dist[T];
    if (num_crystals <= 0) {
        // T is not reachable from S or S == T, but problem says S != T and connected.
        // Given problem constraints, num_crystals should be at least 1.
        cout << 0 << endl;
        return 0;
    }

    // Group edges into disjoint cuts E_i
    vector<vector<int>> E(num_crystals + 1);
    for (int i = 1; i <= M; ++i) {
        int u = edges[i].u;
        int v = edges[i].v;
        if (dist[u] != -1 && dist[v] != -1) {
            int d1 = dist[u];
            int d2 = dist[v];
            if (abs(d1 - d2) == 1) {
                int layer = min(d1, d2) + 1;
                if (layer >= 1 && layer <= num_crystals) {
                    E[layer].push_back(i);
                }
            }
        }
    }

    // Output the result
    cout << num_crystals << "\n";
    for (int i = 1; i <= num_crystals; ++i) {
        cout << E[i].size();
        for (int edge_idx : E[i]) {
            cout << " " << edge_idx;
        }
        cout << "\n";
    }

    return 0;
}

