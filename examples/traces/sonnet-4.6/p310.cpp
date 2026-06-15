#include <bits/stdc++.h>
using namespace std;

int main(){
    long long n, m, k;
    cin >> n >> m >> k;
    
    int states = 1 << (m-1);
    vector<long long> dp(states, 0);
    dp[0] = 1; // 0 boards placed, empty mask
    
    for(int i = 1; i <= n; i++){
        vector<long long> ndp(states, 0);
        for(int mask = 0; mask < states; mask++){
            if(dp[mask] == 0) continue;
            for(int nb = 0; nb <= 1; nb++){
                // check constraint if i >= m
                if(i >= m){
                    int iron = __builtin_popcount(mask) + nb;
                    if(iron < k) continue;
                }
                int nmask = ((mask << 1) | nb) & (states - 1);
                ndp[nmask] += dp[mask];
            }
        }
        dp = ndp;
    }
    
    long long ans = 0;
    for(int mask = 0; mask < states; mask++) ans += dp[mask];
    cout << ans << endl;
}
