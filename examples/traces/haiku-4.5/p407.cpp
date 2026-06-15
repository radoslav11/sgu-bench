#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<long long>> dp(m + 1, vector<long long>(n + 1, 0));
    
    dp[0][0] = 1;
    
    for(int step = 0; step < m; step++){
        for(int node = 0; node <= n; node++){
            if(dp[step][node] == 0) continue;
            
            if(node == 0){
                for(int next = 1; next <= n; next++){
                    dp[step + 1][next] += dp[step][node];
                }
            } else {
                dp[step + 1][0] += dp[step][node];
                int left = (node == 1) ? n : (node - 1);
                dp[step + 1][left] += dp[step][node];
                int right = (node == n) ? 1 : (node + 1);
                dp[step + 1][right] += dp[step][node];
            }
        }
    }
    
    cout << dp[m][0] << endl;
    
    return 0;
}
