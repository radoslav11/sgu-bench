#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    set<pair<int,int>> berland_edges, beerland_edges;
    vector<vector<int>> berland_adj(n+1), beerland_adj(n+1);
    
    // Read Berland edges
    for(int i = 0; i < n-1; i++){
        int a, b;
        cin >> a >> b;
        berland_edges.insert({min(a,b), max(a,b)});
        berland_adj[a].push_back(b);
        berland_adj[b].push_back(a);
    }
    
    // Read Beerland edges
    for(int i = 0; i < n-1; i++){
        int a, b;
        cin >> a >> b;
        beerland_edges.insert({min(a,b), max(a,b)});
        beerland_adj[a].push_back(b);
        beerland_adj[b].push_back(a);
    }
    
    vector<tuple<int,int,int,int,int>> operations;
    
    // Find edges to remove from Berland
    vector<pair<int,int>> to_remove;
    for(auto edge : berland_edges){
        if(beerland_edges.find(edge) == beerland_edges.end()){
            to_remove.push_back(edge);
        }
    }
    
    // Find edges to add to Berland
    vector<pair<int,int>> to_add;
    for(auto edge : beerland_edges){
        if(berland_edges.find(edge) == berland_edges.end()){
            to_add.push_back(edge);
        }
    }
    
    // For each edge to remove, find corresponding edge to add
    for(int i = 0; i < to_remove.size(); i++){
        auto [a1, b1] = to_remove[i];
        auto [a2, b2] = to_add[i];
        
        // Add edge a2-b2 to Berland (temporarily), find path from a1 to b1 in Beerland
        // The edge on this path that's not in current Berland is what we remove
        
        // Find path from a1 to b1 in Beerland using BFS
        vector<int> parent(n+1, -1);
        queue<int> q;
        q.push(a1);
        parent[a1] = a1;
        
        while(!q.empty()){
            int u = q.front();
            q.pop();
            if(u == b1) break;
            
            for(int v : beerland_adj[u]){
                if(parent[v] == -1){
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        // Reconstruct path
        vector<pair<int,int>> path_edges;
        int curr = b1;
        while(curr != a1){
            int prev = parent[curr];
            path_edges.push_back({min(prev, curr), max(prev, curr)});
            curr = prev;
        }
        
        // The edge a2-b2 should be in the path
        operations.push_back({1, a2, b2, a1, b1});
    }
    
    cout << operations.size() << "\n";
    for(auto [p, a1, b1, a2, b2] : operations){
        cout << p << " " << a1 << " " << b1 << " " << a2 << " " << b2 << "\n";
    }
    
    return 0;
}
