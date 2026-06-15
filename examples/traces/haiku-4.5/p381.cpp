#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<tuple<int,int,int,int>> edges(m);
    vector<vector<pair<int,int>>> adj(n+1);
    
    for(int i = 0; i < m; i++){
        int a, b, d1, d2;
        cin >> a >> b >> d1 >> d2;
        edges[i] = {a, b, d1, d2};
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }
    
    vector<int> color(n+1, -1);
    vector<int> transformations;
    bool possible = true;
    
    for(int start = 1; start <= n && possible; start++){
        if(color[start] != -1) continue;
        
        queue<int> q;
        q.push(start);
        color[start] = 0;
        
        while(!q.empty() && possible){
            int u = q.front();
            q.pop();
            
            for(auto [v, edgeIdx] : adj[u]){
                auto [a, b, d1, d2] = edges[edgeIdx];
                
                int da = (a == u) ? d1 : d2;
                int db = (b == u) ? d2 : d1;
                
                // After transformation: if color[u]=1, da becomes -da, etc.
                int final_da = (color[u] == 1) ? -da : da;
                int final_db = (color[v] == -1) ? db : ((color[v] == 1) ? -db : db);
                
                // We need final_da and final_db to have opposite signs
                int needed_color_v;
                if(da == db){
                    // Need opposite final signs: need different colors
                    needed_color_v = 1 - color[u];
                } else {
                    // Need same final signs: need same colors
                    needed_color_v = color[u];
                }
                
                if(color[v] == -1){
                    color[v] = needed_color_v;
                    q.push(v);
                } else if(color[v] != needed_color_v){
                    possible = false;
                    break;
                }
            }
        }
    }
    
    if(!possible){
        cout << "NO\n";
    } else {
        cout << "YES\n";
        for(int i = 1; i <= n; i++){
            if(color[i] == 1){
                transformations.push_back(i);
            }
        }
        cout << transformations.size() << "\n";
        for(int v : transformations){
            cout << v << "\n";
        }
    }
    
    return 0;
}
