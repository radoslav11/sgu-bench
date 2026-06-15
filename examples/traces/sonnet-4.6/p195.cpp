#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n; cin >> n;
    vector<vector<int>> children(n+1);
    for(int i = 2; i <= n; i++){
        int p; cin >> p;
        children[p].push_back(i);
    }
    
    vector<long long> dp0(n+1,0), dp1(n+1,0);
    vector<int> best_child(n+1, -1);
    
    // Process in reverse order (nodes are numbered > parent's number)
    for(int v = n; v >= 1; v--){
        dp1[v] = 1;
        long long sum0 = 0;
        long long best_gain = 0;
        int bc = -1;
        for(int c : children[v]){
            dp1[v] += dp0[c];
            sum0 += dp0[c];
            long long gain = dp1[c] - dp0[c];
            if(gain > best_gain){ best_gain = gain; bc = c; }
        }
        dp0[v] = sum0 + best_gain;
        best_child[v] = bc;
    }
    
    // Reconstruct
    vector<bool> selected(n+1, false);
    // BFS/DFS with state
    stack<pair<int,int>> stk; // node, state (0 or 1)
    int root_state = (dp1[1] > dp0[1]) ? 1 : 0;
    stk.push({1, root_state});
    while(!stk.empty()){
        auto [v, s] = stk.top(); stk.pop();
        if(s == 1){ selected[v] = true; for(int c:children[v]) stk.push({c,0}); }
        else {
            int bc = best_child[v];
            for(int c:children[v]){ if(c==bc) stk.push({c,1}); else stk.push({c,0}); }
        }
    }
    
    long long total = (root_state==1)?dp1[1]:dp0[1];
    cout << total * 1000 << "\n";
    bool first=true;
    for(int i=1;i<=n;i++) if(selected[i]){ if(!first) cout<<" "; cout<<i; first=false; }
    cout<<"\n";
}
