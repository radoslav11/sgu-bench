#include <bits/stdc++.h>
using namespace std;

int main() {
    // Compute partition numbers p(0..40)
    vector<vector<long long>> dp(41, vector<long long>(41, 0));
    for (int j = 0; j <= 40; j++) dp[0][j] = 1;
    for (int i = 1; i <= 40; i++) {
        dp[i][0] = 0;
        for (int j = 1; j <= 40; j++) {
            dp[i][j] = dp[i][j-1] + (i >= j ? dp[i-j][j] : 0);
        }
    }
    vector<long long> part(41, 0);
    for (int i = 0; i <= 40; i++) part[i] = dp[i][i];
    
    long long n;
    cin >> n;
    
    long long ans = 1;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                exp++;
                n /= p;
            }
            ans *= part[exp];
        }
    }
    if (n > 1) {
        ans *= part[1];
    }
    
    cout << ans << endl;
    return 0;
}
