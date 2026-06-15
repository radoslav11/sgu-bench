#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<tuple<int,int,int>> edges(m);
    vector<vector<pair<int,int>>> adj(n+1);
    
    for(int i = 0; i < m; i++){
        int a, b, w;
        cin >> a >> b >> w;
        edges[i] = {a, b, w};
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }
    
    int minCost = INT_MAX;
    vector<int> bestPlan;
    
    // Try removing each city
    for(int captured = 1; captured <= n; captured++){
        // For remaining cities, try all possible cuts
        // Check if removing captured city already disconnects graph
        
        // Try all subsets of edges (2^m is too large for m=500)
        // Instead, use a different approach: for each pair of cities, find min edge cut separating them
        
        // Try cutting edges between two nodes
        for(int s = 1; s <= n; s++){
            if(s == captured) continue;
            for(int t = s+1; t <= n; t++){
                if(t == captured) continue;
                
                // Find min cut between s and t, avoiding captured city
                // Try all subsets of edges connecting s side to t side
                vector<int> edgeIndices;
                for(int i = 0; i < m; i++){
                    auto [a, b, w] = edges[i];
                    if(a == captured || b == captured) continue;
                    edgeIndices.push_back(i);
                }
                
                int sz = edgeIndices.size();
                if(sz > 20) continue; // Too many edges to enumerate
                
                for(int mask = 1; mask < (1 << sz); mask++){
                    vector<bool> destroyed(m, false);
                    int cost = 0;
                    
                    for(int i = 0; i < sz; i++){
                        if(mask & (1 << i)){
                            destroyed[edgeIndices[i]] = true;
                            cost += get<2>(edges[edgeIndices[i]]);
                        }
                    }
                    
                    // Check if s and t are disconnected
                    vector<bool> visited(n+1, false);
                    visited[captured] = true;
                    queue<int> q;
                    q.push(s);
                    visited[s] = true;
                    
                    while(!q.empty()){
                        int u = q.front(); q.pop();
                        for(auto [v, idx] : adj[u]){
                            if(!visited[v] && !destroyed[idx]){
                                visited[v] = true;
                                q.push(v);
                            }
                        }
                    }
                    
                    if(!visited[t] && cost < minCost){
                        minCost = cost;
                        bestPlan.clear();
                        for(int i = 0; i < m; i++){
                            if(destroyed[i]){
                                bestPlan.push_back(i+1);
                            }
                        }
                    }
                }
            }
        }
    }
    
    cout << minCost << "\n";
    cout << bestPlan.size() << "\n";
    for(int i = 0; i < bestPlan.size(); i++){
        if(i > 0) cout << " ";
        cout << bestPlan[i];
    }
    if(bestPlan.size() > 0) cout << "\n";
    
    return 0;
}
