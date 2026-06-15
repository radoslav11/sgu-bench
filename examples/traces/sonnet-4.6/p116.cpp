#include <bits/stdc++.h>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

int main() {
    vector<int> primes;
    for (int i = 2; i <= 10000; i++) {
        if (isPrime(i)) primes.push_back(i);
    }
    
    vector<int> superPrimes;
    for (int i = 0; i < (int)primes.size(); i++) {
        int idx = i + 1;
        if (isPrime(idx)) {
            superPrimes.push_back(primes[i]);
        }
    }
    
    int n;
    cin >> n;
    
    vector<int> dp(n + 1, INT_MAX);
    vector<int> parent(n + 1, -1);
    dp[0] = 0;
    
    for (int i = 1; i <= n; i++) {
        for (int sp : superPrimes) {
            if (sp > i) break;
            if (dp[i - sp] != INT_MAX && dp[i - sp] + 1 < dp[i]) {
                dp[i] = dp[i - sp] + 1;
                parent[i] = sp;
            }
        }
    }
    
    if (dp[n] == INT_MAX) {
        cout << 0 << endl;
    } else {
        cout << dp[n] << endl;
        vector<int> result;
        int cur = n;
        while (cur > 0) {
            result.push_back(parent[cur]);
            cur -= parent[cur];
        }
        sort(result.begin(), result.end(), greater<int>());
        for (int i = 0; i < (int)result.size(); i++) {
            if (i > 0) cout << " ";
            cout << result[i];
        }
        cout << endl;
    }
    
    return 0;
}
