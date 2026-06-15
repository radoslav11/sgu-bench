#include <bits/stdc++.h>
using namespace std;

const int MOD = 2007;

int main() {
    long long n, k;
    cin >> n >> k;
    
    if (k > n) { cout << 0; return 0; }
    if (k == n) { cout << 1 % MOD; return 0; }
    if (k == 1) { cout << 1 % MOD; return 0; }
    
    // S(n,k) with DP, rolling array
    // Need to handle large n,k
    vector<long long> dp(k+1, 0);
    dp[0] = 1;
    
    for (long long i = 1; i <= n; i++) {
        for (long long j = min(i, k); j >= 1; j--) {
            dp[j] = (j * dp[j] + dp[j-1]) % MOD;
        }
    }
    
    cout << dp[k] % MOD;
    return 0;
}
