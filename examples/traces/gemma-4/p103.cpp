#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: Traffic Lights
 * Goal: Find the shortest time path from a source to a destination in a graph
 * where edges can only be traversed when both junctions have the same light color.
 * Light colors at each junction alternate between Blue (B) and Purple (P).
 * 
 * Constraints:
 * N <= 300, M <= 14000
 * Edge weights (travel time) <= 100
 * Duration of each color <= 100
 * Initial color's remaining time <= duration of that color
 * 
 * Algorithm: Dijkstra's algorithm is used to find the shortest path.
 * Since the edge weights (including wait times) satisfy the FIFO property,
 * Dijkstra's will find the optimal path.
 */

const int INF = 2000000000;

struct Edge {
    int to, l;
};

struct Junction {
    char Ci; // Initial color ('B' or 'P')
    int riC; // Remaining time for initial color
    int tiB; // Duration of Blue
    int tiP; // Duration of Purple
};

int N, M;
Junction junctions[301];
vector<Edge> adj[301];
int dist[301];
int parent[301];

/**
 * Returns the color of the light at junction 'u' at time 'T'.
 * The colors follow the pattern: initial color for riC, then the other color for its duration,
 * then the initial color for its duration, and so on.
 */
char getColor(int u, int T) {
    if (T < junctions[u].riC) return junctions[u].Ci;
    int T_rem = T - junctions[u].riC;
    int cycle = junctions[u].tiB + junctions[u].tiP;
    int T_mod = T_rem % cycle;
    if (junctions[u].Ci == 'B') {
        // Initial: B(riB), then P(tiP), B(tiB), P(tiP), ...
        return (T_mod < junctions[u].tiP) ? 'P' : 'B';
    } else {
        // Initial: P(riP), then B(tiB), P(tiP), B(tiB), ...
        return (T_mod < junctions[u].tiB) ? 'B' : 'P';
    }
}

int main() {
    // Fast I/O for speed in competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int start_node, dest_node;
    if (!(cin >> start_node >> dest_node)) return 0;
    if (!(cin >> N >> M)) return 0;

    for (int i = 1; i <= N; ++i) {
        string s;
        cin >> s >> junctions[i].riC >> junctions[i].tiB >> junctions[i].tiP;
        junctions[i].Ci = s[0];
    }

    for (int i = 0; i < M; ++i) {
        int u, v, l;
        cin >> u >> v >> l;
        adj[u].push_back({v, l});
        adj[v].push_back({u, l});
    }

    for (int i = 1; i <= N; ++i) {
        dist[i] = INF;
        parent[i] = -1;
    }

    // Dijkstra's with a priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[start_node] = 0;
    pq.push({0, start_node});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == dest_node) break;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int l = edge.l;
            int w = 0;
            
            // Find the minimum wait time 'w' such that colors at u and v match at time d+w
            while (getColor(u, d + w) != getColor(v, d + w)) {
                w++;
                // Given the constraints (durations <= 100), w won't exceed ~400.
            }
            
            if (dist[v] > d + w + l) {
                dist[v] = d + w + l;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[dest_node] == INF) {
        cout << 0 << endl;
    } else {
        cout << dist[dest_node] << endl;
        // Path reconstruction
        vector<int> path;
        for (int curr = dest_node; curr != -1; curr = parent[curr]) {
            path.push_back(curr);
        }
        reverse(path.begin(), path.end());
        for (int i = 0; i < (int)path.size(); ++i) {
            cout << path[i] << (i == (int)path.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

