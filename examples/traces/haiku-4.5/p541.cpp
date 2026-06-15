#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> adj(n + 1);
    for(int i = 0; i < m; i++){
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    vector<int> color(n + 1, -1);
    vector<int> company1, company2;
    
    // Try to 2-color the graph
    for(int start = 1; start <= n; start++){
        if(color[start] == -1){
            queue<int> q;
            q.push(start);
            color[start] = 0;
            
            while(!q.empty()){
                int u = q.front();
                q.pop();
                
                for(int v : adj[u]){
                    if(color[v] == -1){
                        color[v] = 1 - color[u];
                        q.push(v);
                    }
                }
            }
        }
    }
    
    // Count nodes in each color
    int count0 = 0, count1 = 0;
    for(int i = 1; i <= n; i++){
        if(color[i] == 0) count0++;
        else count1++;
    }
    
    // Assign to companies - larger group to company 1
    if(count0 >= count1){
        for(int i = 1; i <= n; i++){
            if(color[i] == 0) company1.push_back(i);
            else company2.push_back(i);
        }
    } else {
        for(int i = 1; i <= n; i++){
            if(color[i] == 1) company1.push_back(i);
            else company2.push_back(i);
        }
    }
    
    cout << company1.size() << "\n";
    for(int x : company1) cout << x << " ";
    if(company1.size() > 0) cout << "\n";
    else cout << "\n";
    
    cout << company2.size() << "\n";
    for(int x : company2) cout << x << " ";
    if(company2.size() > 0) cout << "\n";
    else cout << "\n";
    
    return 0;
}
