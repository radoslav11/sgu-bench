#include<bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<pair<int,pair<int,long long>>>> graph(N+1); // graph[u] = {v, {edge_id, weight}}
    
    for(int i = 0; i < M; i++){
        int x, y;
        long long l;
        cin >> x >> y >> l;
        graph[x].push_back({y, {i, l}});
        graph[y].push_back({x, {i, l}});
    }
    
    // Dijkstra from node 1
    auto dijkstra = [&](int start) -> vector<long long> {
        vector<long long> dist(N+1, INF);
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
        dist[start] = 0;
        pq.push({0, start});
        
        while(!pq.empty()){
            auto [d, u] = pq.top();
            pq.pop();
            if(d > dist[u]) continue;
            
            for(auto [v, wp] : graph[u]){
                long long w = wp.second;
                if(dist[u] + w < dist[v]){
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    };
    
    vector<long long> dist1 = dijkstra(1);
    long long shortest = dist1[N];
    
    if(shortest == INF){
        cout << "No solution\n";
        return 0;
    }
    
    // Find two paths - greedy approach
    set<pair<int,int>> usedEdges;
    
    auto findPath = [&]() -> vector<int> {
        vector<int> path;
        vector<int> parent(N+1, -1);
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
        vector<long long> dist(N+1, INF);
        dist[1] = 0;
        pq.push({0, 1});
        
        while(!pq.empty()){
            auto [d, u] = pq.top();
            pq.pop();
            if(d > dist[u]) continue;
            
            for(auto [v, wp] : graph[u]){
                int edge_id = wp.first;
                long long w = wp.second;
                int a = min(u, v), b = max(u, v);
                if(usedEdges.count({a, b})) continue;
                
                if(dist[u] + w < dist[v]){
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        
        if(dist[N] == INF) return {};
        
        int cur = N;
        while(cur != -1){
            path.push_back(cur);
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        
        // Mark edges as used
        for(int i = 0; i < (int)path.size()-1; i++){
            int a = min(path[i], path[i+1]);
            int b = max(path[i], path[i+1]);
            usedEdges.insert({a, b});
        }
        
        return path;
    };
    
    vector<int> path1 = findPath();
    vector<int> path2 = findPath();
    
    if(path1.empty() || path2.empty()){
        cout << "No solution\n";
        return 0;
    }
    
    for(int i = 0; i < path1.size(); i++){
        if(i) cout << " ";
        cout << path1[i];
    }
    cout << "\n";
    for(int i = 0; i < path2.size(); i++){
        if(i) cout << " ";
        cout << path2[i];
    }
    cout << "\n";
    
    return 0;
}
