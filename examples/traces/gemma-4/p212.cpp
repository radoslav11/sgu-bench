#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Data Transmission
 * Task: Find a blocking flow in a layered graph.
 * A flow is blocking if every path from source to target has at least one saturated edge.
 * 
 * Algorithm: Two-pass algorithm to find a blocking flow in a layered graph.
 * 1. Forward Pass:
 *    Start with the source having infinite excess.
 *    Iterate through levels 1 to L-1.
 *    For each node u at current level, push as much flow as possible through its outgoing edges
 *    to nodes in the next level.
 * 2. Backward Pass:
 *    Iterate through levels L-1 down to 2.
 *    For each node u at current level (except the target), any remaining excess flow
 *    must be pushed back to its predecessors to satisfy flow conservation.
 *    Reduce the flow on incoming edges to u by the amount of its excess.
 * 
 * Complexity: O(N + M), which is very efficient.
 * 
 * The time limit is tight (0.25 sec), so we use fast I/O and efficient data structures.
 */

const long long INF = 1e18;

struct Edge {
    int to;
    int from;
    int id;
};

int main() {
    // Optimize I/O speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, L;
    if (!(cin >> N >> M >> L)) return 0;

    vector<int> level(N + 1);
    vector<vector<int>> nodes_at_level(L + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> level[i];
        if (level[i] >= 1 && level[i] <= L) {
            nodes_at_level[level[i]].push_back(i);
        }
    }

    vector<int> cap(M);
    vector<long long> flow(M, 0);
    // adj_out stores outgoing edges, adj_in stores incoming edges
    // Using vector of vectors for adjacency lists
    static vector<Edge> adj_out[1505];
    static vector<Edge> adj_in[1505];

    for (int i = 0; i < M; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        cap[i] = c;
        // Each channel connects a node at level i to a node at level i+1
        adj_out[u].push_back({v, u, i});
        adj_in[v].push_back({u, v, i});
    }

    vector<long long> excess(N + 1, 0);
    int source = -1, target = -1;
    for (int i = 1; i <= N; ++i) {
        if (level[i] == 1) source = i;
        if (level[i] == L) target = i;
    }

    if (source == -1 || target == -1) return 0;

    excess[source] = INF;

    // Forward pass: push as much flow as possible from source to target
    for (int i = 1; i < L; ++i) {
        for (int u : nodes_at_level[i]) {
            for (const auto& e : adj_out[u]) {
                long long push = min((long long)cap[e.id], excess[u]);
                if (push > 0) {
                    flow[e.id] = push;
                    excess[u] -= push;
                    excess[e.to] += push;
                }
            }
        }
    }

    // Backward pass: remove excess flow that cannot reach the target
    for (int i = L - 1; i >= 2; --i) {
        for (int u : nodes_at_level[i]) {
            if (u == target) continue;
            long long to_reduce = excess[u];
            for (const auto& e : adj_in[u]) {
                if (to_reduce <= 0) break;
                long long reduce = min(flow[e.id], to_reduce);
                if (reduce > 0) {
                    flow[e.id] -= reduce;
                    to_reduce -= reduce;
                    excess[e.from] += reduce;
                }
            }
            // The leftover excess is now conceptually pushed back to predecessors
            excess[u] = 0; 
        }
    }

    // Output the flow for each channel in the original order
    for (int i = 0; i < M; ++i) {
        cout << flow[i] << "\n";
    }

    return 0;
}

