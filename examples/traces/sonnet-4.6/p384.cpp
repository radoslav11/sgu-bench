#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<pair<int,int>> edges(m+1);
    // adjacency: for each vertex, list of (neighbor, edge_index)
    vector<vector<pair<int,int>>> adj(n+1);
    
    for(int i = 1; i <= m; i++){
        int x, y;
        cin >> x >> y;
        edges[i] = {x, y};
        adj[x].push_back({y, i});
        adj[y].push_back({x, i});
    }
    
    vector<bool> deleted(m+1, false);
    
    string op;
    while(cin >> op){
        if(op == "DELETE"){
            int x; cin >> x;
            deleted[x] = true;
        } else {
            // LENGTH x y
            int x, y;
            cin >> x >> y;
            if(x == y){ cout << 0 << "\n"; continue; }
            // BFS
            vector<int> dist(n+1, -1);
            queue<int> q;
            dist[x] = 0;
            q.push(x);
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto [v, ei] : adj[u]){
                    if(!deleted[ei] && dist[v] == -1){
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
            cout << dist[y] << "\n";
        }
    }
    
    return 0;
}
