#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, K;
    cin >> N >> K;
    
    const int MOD = 2007;
    
    vector<vector<long long>> dp(N+1, vector<long long>(K+1, 0));
    dp[0][0] = 1;
    
    for(int n = 1; n <= N; n++){
        for(int k = 1; k <= min(n, K); k++){
            dp[n][k] = (k * dp[n-1][k] + dp[n-1][k-1]) % MOD;
        }
    }
    
    cout << dp[N][K] << endl;
    
    return 0;
}
