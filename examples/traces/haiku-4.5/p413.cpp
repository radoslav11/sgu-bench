#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int tst;
    cin >> tst;
    
    while(tst--) {
        int n, m;
        cin >> n >> m;
        
        vector<vector<int>> adj(n + 1);
        
        for(int i = 0; i < m; i++) {
            int a, b;
            cin >> a >> b;
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        
        vector<int> country(n + 1, 0);
        queue<int> q;
        
        q.push(1);
        country[1] = 1;
        
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            
            for(int v : adj[u]) {
                if(country[v] == 0) {
                    country[v] = 3 - country[u];
                    q.push(v);
                }
            }
        }
        
        for(int i = 1; i <= n; i++) {
            cout << country[i];
            if(i < n) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
