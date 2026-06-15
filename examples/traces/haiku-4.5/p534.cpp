#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int to, weight, cost, id;
};

vector<Edge> adj[100005];
int n;
int dist[100005];
int parent[100005];
int parent_edge[100005];

void dfs(int u, int p, int d) {
    dist[u] = d;
    parent[u] = p;
    for(auto& e : adj[u]) {
        if(e.to != p) {
            parent_edge[e.to] = e.id;
            dfs(e.to, u, d + e.weight);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    
    vector<tuple<int,int,int,int>> edges;
    
    for(int i = 0; i < n-1; i++) {
        int a, b, t, p;
        cin >> a >> b >> t >> p;
        adj[a].push_back({b, t, p, i+1});
        adj[b].push_back({a, t, p, i+1});
        edges.push_back({a, b, t, p});
    }
    
    // Find diameter
    dfs(1, -1, 0);
    int farthest = 1;
    for(int i = 1; i <= n; i++) {
        if(dist[i] > dist[farthest]) farthest = i;
    }
    
    int original_diameter = dist[farthest];
    
    dfs(farthest, -1, 0);
    int other_end = 1;
    for(int i = 1; i <= n; i++) {
        if(dist[i] > dist[other_end]) other_end = i;
    }
    
    int max_diameter = dist[other_end];
    
    // Find path from farthest to other_end and get edges
    vector<int> path_edges;
    dfs(farthest, -1, 0);
    
    int curr = other_end;
    while(parent[curr] != -1) {
        path_edges.push_back(parent_edge[curr]);
        curr = parent[curr];
    }
    
    // Sort edges on path by cost (descending)
    vector<pair<int,int>> path_info; // cost, edge_id
    for(int eid : path_edges) {
        int cost = get<3>(edges[eid-1]);
        path_info.push_back({cost, eid});
    }
    sort(path_info.rbegin(), path_info.rend());
    
    // Greedily remove edges
    set<int> removed;
    vector<int> result;
    int total_cost = 0;
    
    for(auto [cost, eid] : path_info) {
        removed.insert(eid);
        result.push_back(eid);
        total_cost += cost;
        
        // Check if diameter reduced - recompute
        // Rebuild adj without removed edges
        for(int i = 1; i <= n; i++) adj[i].clear();
        for(int i = 0; i < n-1; i++) {
            if(removed.find(i+1) == removed.end()) {
                auto [a, b, t, p] = edges[i];
                adj[a].push_back({b, t, p, i+1});
                adj[b].push_back({a, t, p, i+1});
            }
        }
        
        dfs(1, -1, 0);
        int far = 1;
        for(int i = 1; i <= n; i++) if(dist[i] > dist[far]) far = i;
        dfs(far, -1, 0);
        int other = 1;
        for(int i = 1; i <= n; i++) if(dist[i] > dist[other]) other = i;
        
        if(dist[other] < max_diameter) break;
    }
    
    cout << total_cost << "\n";
    cout << result.size() << "\n";
    for(int x : result) cout << x << " ";
    if(!result.empty()) cout << "\n";
    
    return 0;
}
