#include<bits/stdc++.h>
using namespace std;

int n;
vector<int> children[500005];
long long dp[500005][2];
vector<int> result;

void dfs(int u) {
    if(children[u].empty()) {
        dp[u][0] = 1;
        dp[u][1] = 0;
        return;
    }
    
    for(int v : children[u]) {
        dfs(v);
    }
    
    long long val_receive = 1;
    for(int v : children[u]) {
        val_receive += max(dp[v][0], dp[v][1]);
    }
    
    long long val_not_receive = 0;
    for(int v : children[u]) {
        val_not_receive += max(dp[v][0], dp[v][1]);
    }
    
    for(int v : children[u]) {
        long long val_assign = 1;
        val_assign += max(dp[v][0], dp[v][1]);
        for(int w : children[u]) {
            if(w != v) {
                val_assign += max(dp[w][0], dp[w][1]);
            }
        }
        val_not_receive = max(val_not_receive, val_assign);
    }
    
    dp[u][0] = val_receive;
    dp[u][1] = val_not_receive;
}

void reconstruct(int u, int received) {
    if(received) {
        result.push_back(u);
        for(int v : children[u]) {
            reconstruct(v, max(dp[v][0], dp[v][1]) == dp[v][0]);
        }
    } else {
        long long best = 0;
        int best_child = -1;
        
        for(int v : children[u]) {
            long long val = max(dp[v][0], dp[v][1]);
            if(val > 0) {
                long long val_assign = 1 + max(dp[v][0], dp[v][1]);
                for(int w : children[u]) {
                    if(w != v) val_assign += max(dp[w][0], dp[w][1]);
                }
                if(val_assign > best) {
                    best = val_assign;
                    best_child = v;
                }
            }
        }
        
        for(int v : children[u]) {
            if(v == best_child) {
                reconstruct(v, true);
            } else {
                reconstruct(v, max(dp[v][0], dp[v][1]) == dp[v][0]);
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    for(int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        children[p].push_back(i);
    }
    
    dfs(1);
    
    long long ans = max(dp[1][0], dp[1][1]);
    reconstruct(1, dp[1][0] > dp[1][1]);
    
    sort(result.begin(), result.end());
    cout << ans * 1000 << "\n";
    for(int x : result) cout << x << " ";
    if(!result.empty()) cout << "\n";
    
    return 0;
}
