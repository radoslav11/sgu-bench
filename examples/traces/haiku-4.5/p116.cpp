#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    // Sieve of Eratosthenes
    vector<bool> is_prime(10001, true);
    is_prime[0] = is_prime[1] = false;
    for(int i = 2; i * i <= 10000; i++){
        if(is_prime[i]){
            for(int j = i * i; j <= 10000; j += i){
                is_prime[j] = false;
            }
        }
    }
    
    // Get all primes
    vector<int> primes;
    for(int i = 2; i <= 10000; i++){
        if(is_prime[i]) primes.push_back(i);
    }
    
    // Identify super-primes
    vector<int> super_primes;
    for(int i = 0; i < primes.size(); i++){
        int index = i + 1; // 1-indexed
        if(index <= 10000 && is_prime[index]){
            super_primes.push_back(primes[i]);
        }
    }
    
    // BFS/DP to find minimum super-primes summing to n
    vector<int> dp(n + 1, INT_MAX);
    vector<int> parent(n + 1, -1);
    dp[0] = 0;
    
    for(int i = 0; i <= n; i++){
        if(dp[i] == INT_MAX) continue;
        for(int sp : super_primes){
            if(i + sp <= n){
                if(dp[i] + 1 < dp[i + sp]){
                    dp[i + sp] = dp[i] + 1;
                    parent[i + sp] = i;
                }
            }
        }
    }
    
    if(dp[n] == INT_MAX){
        cout << 0 << "\n";
    } else {
        vector<int> result;
        int curr = n;
        while(curr > 0){
            int prev = parent[curr];
            result.push_back(curr - prev);
            curr = prev;
        }
        
        sort(result.begin(), result.end(), greater<int>());
        
        cout << result.size() << "\n";
        for(int i = 0; i < result.size(); i++){
            if(i > 0) cout << " ";
            cout << result[i];
        }
        cout << "\n";
    }
    
    return 0;
}
