#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int to, T, R, P;
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, H, S, E;
    cin >> N >> M >> H >> S >> E;
    
    vector<vector<Edge>> graph(N + 1);
    
    for(int i = 0; i < M; i++){
        int A, B, T, R, P;
        cin >> A >> B >> T >> R >> P;
        graph[A].push_back({B, T, R, P});
        graph[B].push_back({A, T, R, P});
    }
    
    set<int> exits;
    for(int i = 0; i < E; i++){
        int f;
        cin >> f;
        exits.insert(f);
    }
    
    // Dijkstra with state (max_heat, node)
    vector<int> best_heat(N + 1, INT_MAX);
    vector<int> parent(N + 1, -1);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    
    pq.push({0, S});
    best_heat[S] = 0;
    
    while(!pq.empty()){
        auto [heat, u] = pq.top();
        pq.pop();
        
        if(heat > best_heat[u]) continue;
        
        for(auto& edge : graph[u]){
            int v = edge.to;
            int corridor_heat = edge.R + edge.P * edge.T;
            int new_heat = max(heat, corridor_heat);
            
            if(new_heat < best_heat[v]){
                best_heat[v] = new_heat;
                parent[v] = u;
                pq.push({new_heat, v});
            }
        }
    }
    
    // Find the exit with minimum heat
    int best_exit = -1;
    int min_heat = INT_MAX;
    
    for(int exit : exits){
        if(best_heat[exit] < min_heat){
            min_heat = best_heat[exit];
            best_exit = exit;
        }
    }
    
    if(best_exit == -1 || min_heat > H){
        cout << "NO\n";
    } else {
        cout << "YES\n";
        cout << min_heat << "\n";
        
        // Reconstruct path
        vector<int> path;
        int curr = best_exit;
        while(curr != -1){
            path.push_back(curr);
            curr = parent[curr];
        }
        
        reverse(path.begin(), path.end());
        
        cout << path.size();
        for(int node : path){
            cout << " " << node;
        }
        cout << "\n";
    }
    
    return 0;
}
