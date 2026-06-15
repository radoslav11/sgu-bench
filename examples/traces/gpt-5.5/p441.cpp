#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    const int MOD = 2007;

    if (K < 0 || K > N) {
        cout << 0 << '\n';
        return 0;
    }

    vector<int> dp(K + 1, 0);
    dp[0] = 1;

    for (int n = 1; n <= N; ++n) {
        for (int k = min(n, K); k >= 1; --k) {
            dp[k] = (dp[k - 1] + k * dp[k]) % MOD;
        }
        dp[0] = 0;
    }

    cout << dp[K] % MOD << '\n';
    return 0;
}
