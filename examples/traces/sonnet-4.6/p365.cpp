#include <bits/stdc++.h>
using namespace std;

int main() {
    int S;
    cin >> S;
    
    long long dp[10][2] = {};
    
    for (int d = 0; d <= 9; d++) {
        dp[d][0] = 1;
    }
    
    for (int pos = 1; pos < S; pos++) {
        long long ndp[10][2] = {};
        for (int d = 0; d <= 9; d++) {
            if (dp[d][0] > 0) {
                for (int nd = 0; nd <= 9; nd++) {
                    if (nd >= d) ndp[nd][0] += dp[d][0];
                    else ndp[nd][1] += dp[d][0];
                }
            }
            if (dp[d][1] > 0) {
                for (int nd = 0; nd <= d; nd++) {
                    ndp[nd][1] += dp[d][1];
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }
    
    long long ans = 0;
    for (int d = 0; d <= 9; d++) ans += dp[d][0] + dp[d][1];
    
    cout << ans << endl;
    return 0;
}
