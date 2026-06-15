#include<bits/stdc++.h>
using std/

;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    
    int M = N * K / 2;
    vector<pair<int,int>> edges(M);
    vector<vector<pair<int,int>>> adj(N + 1); // {neighbor, edge_id}
    
    for(int i = 0; i < M; i++){
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    
    vector<int> degree(N + 1, 0);
    vector<bool> used(M, false);
    vector<int> result;
    
    // Try to build a 2-regular subgraph using cycle decomposition
    for(int start = 1; start <= N; start++){
        if(degree[start] == 2) continue;
        
        // Try to find a cycle starting from this vertex
        vector<int> cycle_edges;
        vector<int> cycle_deg(N + 1, 0);
        
        function<bool(int, int, int)> dfs = [&](int u, int parent_edge, int depth) -> bool {
            if(depth > 0 && u == start){
                // Found a cycle back to start
                return true;
            }
            if(depth > N) return false; // Prevent infinite loops
            
            for(auto [v, eid] : adj[u]){
                if(eid == parent_edge) continue;
                if(used[eid]) continue;
                if(degree[u] + cycle_deg[u] >= 2) continue;
                if(degree[v] + cycle_deg[v] >= 2 && v != start) continue;
                
                cycle_edges.push_back(eid);
                cycle_deg[u]++;
                cycle_deg[v]++;
                
                if(dfs(v, eid, depth + 1)){
                    return true;
                }
                
                cycle_edges.pop_back();
                cycle_deg[u]--;
                cycle_deg[v]--;
            }
            return false;
        };
        
        if(dfs(start, -1, 0)){
            for(int eid : cycle_edges){
                used[eid] = true;
                result.push_back(eid + 1);
                int u = edges[eid].first;
                int v = edges[eid].second;
                degree[u]++;
                degree[v]++;
            }
        }
    }
    
    // Check if all vertices have degree 2
    bool valid = true;
    for(int i = 1; i <= N; i++){
        if(degree[i] != 2){
            valid = false;
            break;
        }
    }
    
    if(valid){
        cout << "YES\n";
        for(int e : result){
            cout << e << "\n";
        }
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
