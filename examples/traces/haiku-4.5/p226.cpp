#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<pair<int, int>>> graph(N + 1); // graph[u] = {(v, color), ...}
    
    for(int i = 0; i < M; i++){
        int x, y, c;
        cin >> x >> y >> c;
        graph[x].push_back({y, c});
    }
    
    // State: (vertex, last_color)
    // dist[v][c] = shortest distance to vertex v where last edge had color c
    // c = 0 means no previous edge (for start state)
    vector<vector<int>> dist(N + 1, vector<int>(4, INT_MAX));
    
    queue<pair<int, int>> q; // (vertex, last_color)
    q.push({1, 0}); // start at vertex 1 with no previous color
    dist[1][0] = 0;
    
    while(!q.empty()){
        auto [u, last_color] = q.front();
        q.pop();
        
        int current_dist = dist[u][last_color];
        
        // Try all outgoing edges
        for(auto [v, color] : graph[u]){
            // Can only take this edge if color is different from last_color
            if(color != last_color){
                int new_dist = current_dist + 1;
                if(new_dist < dist[v][color]){
                    dist[v][color] = new_dist;
                    q.push({v, color});
                }
            }
        }
    }
    
    // Find minimum distance to vertex N with any last color
    int ans = INT_MAX;
    for(int c = 0; c <= 3; c++){
        ans = min(ans, dist[N][c]);
    }
    
    if(ans == INT_MAX){
        cout << -1 << endl;
    } else {
        cout << ans << endl;
    }
    
    return 0;
}
