#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cctype>
#include <tuple>

using namespace std;

typedef long long ll;
typedef pair<ll, int> pli; // (distance, node)

const ll INF = 1LL << 60;

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return a * b / gcd(a, b);
}

struct Node {
    int init_color; // 0 for 'B', 1 for 'P'
    int rem_time;
    int dur[2]; // dur[0] = blue duration, dur[1] = purple duration
};

int get_color(const vector<Node>& nodes, int u, ll t) {
    const Node& node = nodes[u];
    
    // Initial phase: color remains init_color for rem_time
    if (t < node.rem_time) {
        return node.init_color;
    }
    
    // After initial phase, the cycle is periodic
    ll cycle = node.dur[0] + node.dur[1];
    ll elapsed = t - node.rem_time;
    
    // Handle the case where cycle might be 0 (shouldn't happen per constraints)
    if (cycle == 0) return node.init_color;
    
    elapsed %= cycle;
    
    // After rem_time, the color alternates
    if (node.init_color == 0) { // started with blue
        // First after rem_time: purple for dur[1]
        if (elapsed < node.dur[1]) {
            return 1; // purple
        } else {
            return 0; // blue
        }
    } else { // started with purple
        // First after rem_time: blue for dur[0]
        if (elapsed < node.dur[0]) {
            return 0; // blue
        } else {
            return 1; // purple
        }
    }
}

ll same_color_time(const vector<Node>& nodes, int u, int v, ll t) {
    // Check times up to t + lcm(cycle[u], cycle[v])
    int cycle_u = nodes[u].dur[0] + nodes[u].dur[1];
    int cycle_v = nodes[v].dur[0] + nodes[v].dur[1];
    int combined_cycle = lcm(cycle_u, cycle_v);
    
    // We check at most combined_cycle time units from t
    for (ll check_time = t; check_time <= t + combined_cycle; check_time++) {
        if (get_color(nodes, u, check_time) == get_color(nodes, v, check_time)) {
            return check_time;
        }
    }
    
    // Fallback (shouldn't happen in practice)
    return t + combined_cycle;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int src, dest;
    cin >> src >> dest;
    
    int N, M;
    cin >> N >> M;
    
    vector<Node> nodes(N + 1); // 1-indexed
    
    for (int i = 1; i <= N; i++) {
        char color_char;
        cin >> color_char;
        
        if (color_char == 'B') {
            nodes[i].init_color = 0;
        } else {
            nodes[i].init_color = 1;
        }
        
        cin >> nodes[i].rem_time >> nodes[i].dur[0] >> nodes[i].dur[1];
    }
    
    // Build adjacency list: adj[u] = list of (v, weight)
    vector<vector<pair<int, int>>> adj(N + 1);
    
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // undirected graph
    }
    
    // Dijkstra's algorithm with time-dependent edge weights
    vector<ll> dist(N + 1, INF);
    vector<int> parent(N + 1, -1);
    
    // Priority queue: (distance, node)
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    
    // Initialize source
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        // Skip if we've found a better path already
        if (d != dist[u]) continue;
        
        // Explore neighbors
        for (auto [v, w] : adj[u]) {
            // Find the earliest time >= dist[u] when both u and v have same color
            ll start_time = same_color_time(nodes, u, v, dist[u]);
            
            // Total time to reach v through this edge
            ll new_time = start_time + w;
            
            if (new_time < dist[v]) {
                dist[v] = new_time;
                parent[v] = u;
                pq.push({new_time, v});
            }
        }
    }
    
    // Output result
    if (dist[dest] == INF) {
        cout << 0 << endl;
    } else {
        // Reconstruct path
        vector<int> path;
        int current = dest;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        reverse(path.begin(), path.end());
        
        cout << dist[dest] << endl;
        
        for (int i = 0; i < path.size(); i++) {
            if (i > 0) cout << " ";
            cout << path[i];
        }
        cout << endl;
    }
    
    return 0;
}
