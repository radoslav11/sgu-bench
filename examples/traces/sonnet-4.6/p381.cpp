#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    // For each edge, compute c_e = d1*d2
    // constraint: x_u XOR x_v = (c_e == 1) ? 1 : 0
    // i.e., x_u XOR x_v = (c_e + 1)/2 ... wait
    // c_e = -1 -> need x_u XOR x_v = 0
    // c_e = +1 -> need x_u XOR x_v = 1
    
    vector<vector<pair<int,int>>> adj(n+1); // adj[u] = {v, required_xor}
    
    for(int i = 0; i < m; i++){
        int a, b, d1, d2;
        cin >> a >> b >> d1 >> d2;
        int c = d1 * d2;
        int req = (c == 1) ? 1 : 0;
        adj[a].push_back({b, req});
        adj[b].push_back({a, req});
    }
    
    vector<int> color(n+1, -1);
    bool possible = true;
    vector<int> result;
    
    for(int start = 1; start <= n && possible; start++){
        if(color[start] != -1) continue;
        // BFS
        queue<int> q;
        q.push(start);
        color[start] = 0;
        int cnt0 = 0, cnt1 = 0;
        vector<int> comp;
        while(!q.empty() && possible){
            int u = q.front(); q.pop();
            comp.push_back(u);
            if(color[u] == 0) cnt0++; else cnt1++;
            for(auto [v, req] : adj[u]){
                int expected = color[u] ^ req;
                if(color[v] == -1){
                    color[v] = expected;
                    q.push(v);
                } else {
                    if(color[v] != expected){
                        possible = false;
                        break;
                    }
                }
            }
        }
        if(!possible) break;
        // choose which group to flip (minimize count)
        // if cnt1 < cnt0, flip group with color=0 -> assign x=1 to color=0 group
        // we want minimum 1s
        int flip = (cnt1 < cnt0) ? 0 : 1; // color group to assign x=1
        for(int v : comp){
            if(color[v] == flip) result.push_back(v);
        }
    }
    
    if(!possible){
        cout << "NO\n";
    } else {
        cout << "YES\n";
        cout << result.size() << "\n";
        for(int v : result) cout << v << "\n";
    }
    
    return 0;
}
