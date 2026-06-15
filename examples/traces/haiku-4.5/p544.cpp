#include<bits/stdc++.h>
using namespace std;

const long long MOD = 1000000009;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k;
    cin >> n >> k;
    
    vector<long long> b(n), by(n);
    for (int i = 0; i < n; i++) cin >> b[i];
    for (int i = 0; i < n; i++) cin >> by[i];
    
    sort(b.begin(), b.end());
    sort(by.begin(), by.end());
    
    // dp[i][j] = ways to match first i Berland players with j wins
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    dp[0][0] = 1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (dp[i][j] == 0) continue;
            // Match b[i] with some by[x] where x is not yet used
            for (int x = i - j; x < n; x++) {
                // by[x] is at position x in sorted order
                // We've matched i players, using j from first (i) byteland
                // and (i-j) from later byteland
                // Next we need one of remaining
                int wins_add = (b[i] > by[x]) ? 1 : 0;
                dp[i + 1][j + wins_add] = (dp[i + 1][j + wins_add] + dp[i][j]) % MOD;
            }
        }
    }
    
    cout << dp[n][(n + k) / 2] << endl;
    
    return 0;
}
