#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    const int MAX_N = 10000;

    // Sieve for primes up to MAX_N
    vector<bool> is_prime(MAX_N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_N; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_N; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // Get list of primes (all primes <= MAX_N)
    vector<int> primes;
    for (int i = 2; i <= MAX_N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }

    // Get super-primes: primes at prime indices (1-indexed)
    vector<int> super_primes;
    for (int i = 0; i < primes.size(); i++) {
        int index_1based = i + 1;
        if (index_1based >= 2 && is_prime[index_1based]) {
            super_primes.push_back(primes[i]);
        }
    }

    int n;
    cin >> n;

    const int INF = 1000000;
    vector<int> dp(n + 1, INF);
    vector<int> prev(n + 1, -1);

    dp[0] = 0;

    for (int i = 1; i <= n; i++) {
        for (int s : super_primes) {
            if (s <= i) {
                if (dp[i - s] + 1 < dp[i]) {
                    dp[i] = dp[i - s] + 1;
                    prev[i] = s;
                }
            }
        }
    }

    if (dp[n] >= INF) {
        cout << 0 << endl;
    } else {
        cout << dp[n] << endl;
        vector<int> coins;
        int cur = n;
        while (cur > 0) {
            coins.push_back(prev[cur]);
            cur -= prev[cur];
        }
        sort(coins.begin(), coins.end(), greater<int>());
        for (int i = 0; i < coins.size(); i++) {
            if (i > 0) cout << " ";
            cout << coins[i];
        }
        cout << endl;
    }

    return 0;
}
