#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    // Build adjacency list with color information
    // adj[u] = list of {v, color}
    vector<vector<pair<int, int>>> adj(N + 1);
    
    for (int i = 0; i < M; i++) {
        int x, y, c;
        cin >> x >> y >> c;
        adj[x].push_back({y, c});
    }
    
    // dist[v][c] = shortest distance to vertex v, where last edge used color c
    // c can be 0, 1, 2 (we'll store colors as 0-indexed internally)
    // Initialize with INF
    vector<vector<int>> dist(N + 1, vector<int>(3, INT_MAX));
    
    // BFS/Dijkstra-like approach since all edges have weight 1 (we can use BFS)
    // But we need to track the last color used to avoid consecutive same colors
    // Queue: {vertex, last_color}
    // last_color: 0, 1, 2 for colors 1, 2, 3; -1 for starting state (no previous color)
    
    queue<pair<int, int>> q;
    
    // Initialize: start at vertex 1 with no previous color (-1)
    // We can start with any outgoing edge from vertex 1
    for (auto& edge : adj[1]) {
        int next = edge.first;
        int color = edge.second - 1; // convert to 0-indexed
        dist[next][color] = 1;
        q.push({next, color});
    }
    
    while (!q.empty()) {
        auto [u, last_color] = q.front();
        q.pop();
        
        // Try all outgoing edges from u
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int color = edge.second - 1; // 0-indexed
            
            // Skip if same color as last edge
            if (color == last_color) continue;
            
            // If we found a shorter path to v with this color
            if (dist[u][last_color] + 1 < dist[v][color]) {
                dist[v][color] = dist[u][last_color] + 1;
                q.push({v, color});
            }
        }
    }
    
    // Find minimum distance to vertex N with any last color
    int ans = *min_element(dist[N].begin(), dist[N].end());
    
    if (ans == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << ans << endl;
    }
    
    return 0;
}
