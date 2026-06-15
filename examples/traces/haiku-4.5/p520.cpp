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
    
    vector<int> dist(n + 1, INT_MAX);
    queue<int> q;
    q.push(1);
    dist[1] = 0;
    
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v : adj[u]){
            if(dist[v] == INT_MAX){
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    
    map<pair<int,int>, bool> memo;
    
    function<bool(int, int)> canWin = [&](int city, int day) -> bool {
        auto key = make_pair(city, day);
        if(memo.count(key)) return memo[key];
        
        vector<int> safe;
        for(int next : adj[city]){
            if(dist[next] > day){
                safe.push_back(next);
            }
        }
        
        if(safe.empty()){
            return memo[key] = false;
        }
        
        for(int next : safe){
            if(!canWin(next, day + 1)){
                return memo[key] = true;
            }
        }
        
        return memo[key] = false;
    };
    
    if(canWin(1, 1)){
        cout << "Vladimir\n";
    } else {
        cout << "Nikolay\n";
    }
    
    return 0;
}
