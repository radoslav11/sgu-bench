#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> profit(n+1);
    for(int i = 1; i <= n; i++) cin >> profit[i];
    
    vector<vector<int>> adj(n+1);
    for(int i = 0; i < n-1; i++){
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    vector<long long> dp(n+1, 0);
    vector<int> parent(n+1, -1);
    vector<int> order;
    
    vector<bool> visited(n+1, false);
    stack<int> stk;
    stk.push(1);
    visited[1] = true;
    
    while(!stk.empty()){
        int v = stk.top();
        stk.pop();
        order.push_back(v);
        for(int u : adj[v]){
            if(!visited[u]){
                visited[u] = true;
                parent[u] = v;
                stk.push(u);
            }
        }
    }
    
    reverse(order.begin(), order.end());
    
    long long ans = LLONG_MIN;
    
    for(int v : order){
        dp[v] = profit[v];
        for(int u : adj[v]){
            if(u != parent[v]){
                if(dp[u] > 0) dp[v] += dp[u];
            }
        }
        ans = max(ans, dp[v]);
    }
    
    cout << ans << endl;
    
    return 0;
}
