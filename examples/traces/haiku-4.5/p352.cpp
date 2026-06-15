#include<bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<pair<int, pair<long long, int>>>> adj(N + 1); // {neighbor, {length, isTree}}
    vector<tuple<int, int, long long, int>> edges;
    
    for(int i = 0; i < M; i++){
        int a, b;
        long long l;
        int t;
        cin >> a >> b >> l >> t;
        adj[a].push_back({b, {l, t}});
        adj[b].push_back({a, {l, t}});
        edges.push_back({a, b, l, t});
    }
    
    // Find distances in tree and parent relationships
    vector<long long> dist(N + 1, INF);
    vector<int> parent(N + 1, -1);
    vector<long long> parentEdgeLen(N + 1, 0);
    
    dist[1] = 0;
    queue<int> q;
    q.push(1);
    
    while(!q.empty()){
        int u = q.front();
        q.pop();
        
        for(auto [v, info] : adj[u]){
            auto [len, isTree] = info;
            if(isTree && dist[v] == INF){
                dist[v] = dist[u] + len;
                parent[v] = u;
                parentEdgeLen[v] = len;
                q.push(v);
            }
        }
    }
    
    // For each city, find shortest path when last edge is blocked
    vector<long long> result(N + 1);
    
    for(int i = 2; i <= N; i++){
        int blocked_u = parent[i];
        long long blocked_len = parentEdgeLen[i];
        
        // Dijkstra without using edge (blocked_u, i)
        vector<long long> d(N + 1, INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        
        d[1] = 0;
        pq.push({0, 1});
        
        while(!pq.empty()){
            auto [cost, u] = pq.top();
            pq.pop();
            
            if(cost > d[u]) continue;
            
            for(auto [v, info] : adj[u]){
                auto [len, isTree] = info;
                
                // Skip the blocked edge
                if((u == blocked_u && v == i && len == blocked_len) ||
                   (u == i && v == blocked_u && len == blocked_len)){
                    continue;
                }
                
                if(d[u] + len < d[v]){
                    d[v] = d[u] + len;
                    pq.push({d[v], v});
                }
            }
        }
        
        result[i] = (d[i] == INF) ? -1 : d[i];
    }
    
    for(int i = 2; i <= N; i++){
        cout << result[i];
        if(i < N) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
