#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<pair<int,int>> edges(m + 1);
    vector<bool> deleted(m + 1, false);
    
    for(int i = 1; i <= m; i++){
        int x, y;
        cin >> x >> y;
        edges[i] = {x, y};
    }
    
    string command;
    while(cin >> command){
        if(command == "DELETE"){
            int x;
            cin >> x;
            deleted[x] = true;
        }
        else { // LENGTH
            int x, y;
            cin >> x >> y;
            
            // Build current graph
            vector<vector<int>> adj(n + 1);
            for(int i = 1; i <= m; i++){
                if(!deleted[i]){
                    int u = edges[i].first;
                    int v = edges[i].second;
                    adj[u].push_back(v);
                    adj[v].push_back(u);
                }
            }
            
            // BFS from x to y
            vector<int> dist(n + 1, -1);
            queue<int> q;
            q.push(x);
            dist[x] = 0;
            
            while(!q.empty()){
                int u = q.front();
                q.pop();
                
                for(int v : adj[u]){
                    if(dist[v] == -1){
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
