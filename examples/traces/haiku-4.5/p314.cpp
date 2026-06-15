#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    cin >> n >> m >> k;
    
    int s, t;
    cin >> s >> t;
    s--; t--;  // 0-indexed
    
    vector<vector<pair<int, int>>> adj(n);  // adj[u] = {(v, weight), ...}
    
    for(int i = 0; i < m; i++){
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        adj[a].push_back({b, c});
    }
    
    // For each vertex, store up to k shortest distances to reach it
    vector<vector<long long>> dist(n);
    
    // Priority queue: (distance, vertex)
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    pq.push({0, s});
    dist[s].push_back(0);
    
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        
        // If we already have k paths to this vertex and this distance is >= the k-th shortest, skip
        if(dist[u].size() > k || (dist[u].size() == k && d > dist[u][k-1])){
            continue;
        }
        
        // If this distance is not in our list of shortest paths to u, skip
        if(dist[u].size() > 0 && d > dist[u].back()){
            continue;
        }
        
        for(auto [v, w] : adj[u]){
            long long new_dist = d + w;
            
            // Only add if we haven't found k paths to v yet, or if this is better than some existing path
            if(dist[v].size() < k){
                dist[v].push_back(new_dist);
                sort(dist[v].begin(), dist[v].end());
                pq.push({new_dist, v});
            }
            else if(new_dist < dist[v][k-1]){
                dist[v][k-1] = new_dist;
                sort(dist[v].begin(), dist[v].end());
                pq.push({new_dist, v});
            }
        }
    }
    
    if(dist[t].size() < k){
        for(int i = 0; i < dist[t].size(); i++){
            cout << dist[t][i] << "\n";
        }
        for(int i = dist[t].size(); i < k; i++){
            cout << "NO\n";
        }
    }
    else{
        for(int i = 0; i < k; i++){
            cout << dist[t][i] << "\n";
        }
    }
    
    return 0;
}
