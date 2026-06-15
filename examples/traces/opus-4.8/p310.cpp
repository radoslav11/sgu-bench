#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m, k;
    cin >> n >> m >> k;
    int w = m - 1;
    int sz = 1 << w; // number of masks
    vector<long long> dp(sz, 0);
    // after placing m-1 bits, all masks count 1
    for(int mask = 0; mask < sz; mask++) dp[mask] = 1;
    
    long long fullmask = sz - 1; // (1<<(m-1))-1
    
    for(int pos = m; pos <= n; pos++){
        vector<long long> ndp(sz, 0);
        for(int mask = 0; mask < sz; mask++){
            if(dp[mask] == 0) continue;
            for(int b = 0; b <= 1; b++){
                int window = (mask << 1) | b; // m bits
                if(__builtin_popcount(window) >= k){
                    int nmask = window & fullmask;
                    ndp[nmask] += dp[mask];
                }
            }
        }
        dp = ndp;
    }
    
    long long ans = 0;
    for(int mask = 0; mask < sz; mask++) ans += dp[mask];
    cout << ans << endl;
    return 0;
}
