#include<bits/stdc++.h>
using namespace std;

// Compute number of partitions of n
long long countPartitions(int n) {
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    
    // dp[n][k] = number of partitions of n using parts of size at most k
    for (int i = 0; i <= n; i++) {
        dp[i][0] = 0;
    }
    dp[0][0] = 1;
    
    for (int i = 1; i <= n; i++) {
        for (int k = 1; k <= n; k++) {
            dp[i][k] = dp[i][k-1];
            if (i >= k) {
                dp[i][k] += dp[i-k][k];
            }
        }
    }
    
    return dp[n][n];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    cin >> n;
    
    // Find prime factorization
    map<long long, int> factors;
    long long temp = n;
    
    for (long long i = 2; i * i <= temp; i++) {
        while (temp % i == 0) {
            factors[i]++;
            temp /= i;
        }
    }
    if (temp > 1) {
        factors[temp]++;
    }
    
    long long result = 1;
    
    // For each prime power, multiply by the number of partitions of its exponent
    for (auto& p : factors) {
        int exponent = p.second;
        result *= countPartitions(exponent);
    }
    
    cout << result << endl;
    
    return 0;
}
