#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<vector<pair<int, long long>>> adj(n + 1);
    
    // Read the tree structure
    for(int i = 2; i <= n; i++){
        int parent;
        long long length;
        cin >> parent >> length;
        adj[parent].push_back({i, length});
        adj[i].push_back({parent, length});
    }
    
    // For each computer, find maximum distance to any other computer
    for(int start = 1; start <= n; start++){
        // BFS from start node
        vector<long long> dist(n + 1, -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        
        long long maxDist = 0;
        
        while(!q.empty()){
            int u = q.front();
            q.pop();
            
            for(auto [v, len] : adj[u]){
                if(dist[v] == -1){
                    dist[v] = dist[u] + len;
                    maxDist = max(maxDist, dist[v]);
                    q.push(v);
                }
            }
        }
        
        cout << maxDist << "\n";
    }
    
    return 0;
}
