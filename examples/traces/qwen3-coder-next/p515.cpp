#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, int> pli;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<tuple<int, int, int>>> adj(n+1); // to, weight, edge_index
    
    for (int i = 1; i <= m; i++) {
        int a, b, t;
        cin >> a >> b >> t;
        adj[a].emplace_back(b, t, i);
        adj[b].emplace_back(a, t, i);
    }
    
    int k;
    cin >> k;
    vector<int> special(k);
    for (int i = 0; i < k; i++) {
        cin >> special[i];
    }
    
    // If only one special city, path has 0 edges
    if (k == 1) {
        cout << 0 << "\n";
        return 0;
    }
    
    // Dijkstra function
    auto dijkstra = [&](int start, vector<int>& parent, vector<ll>& dist) {
        dist.assign(n+1, LLONG_MAX);
        parent.assign(n+1, -1);
        dist[start] = 0;
        
        priority_queue<pli, vector<pli>, greater<pli>> pq;
        pq.emplace(0, start);
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            if (d != dist[u]) continue;
            
            for (auto [v, w, idx] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.emplace(dist[v], v);
                }
            }
        }
    };
    
    vector<int> parent;
    vector<ll> dist;
    
    // Find the farthest special city from special[0]
    dijkstra(special[0], parent, dist);
    
    int u = special[0];
    for (int city : special) {
        if (dist[city] > dist[u]) {
            u = city;
        }
    }
    
    // Find the farthest special city from u
    dijkstra(u, parent, dist);
    
    int v = u;
    for (int city : special) {
        if (dist[city] > dist[v]) {
            v = city;
        }
    }
    
    // Reconstruct path from u to v
    vector<int> path_nodes;
    for (int cur = v; cur != -1; cur = parent[cur]) {
        path_nodes.push_back(cur);
    }
    reverse(path_nodes.begin(), path_nodes.end());
    
    // Now get the edge indices along the path
    vector<int> edge_indices;
    for (int i = 0; i < (int)path_nodes.size() - 1; i++) {
        int a = path_nodes[i], b = path_nodes[i+1];
        // Find the edge between a and b that was used
        for (auto [to, w, idx] : adj[a]) {
            if (to == b) {
                edge_indices.push_back(idx);
                break;
            }
        }
    }
    
    cout << edge_indices.size() << "\n";
    for (int i = 0; i < (int)edge_indices.size(); i++) {
        if (i > 0) cout << " ";
        cout << edge_indices[i];
    }
    cout << "\n";
    
    return 0;
}
