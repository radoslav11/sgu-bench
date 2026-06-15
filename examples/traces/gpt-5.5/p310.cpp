#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    int S = 1 << m;
    int maskLimit = S - 1;

    vector<unsigned long long> dp(S, 0), ndp(S, 0);
    dp[0] = 1;

    vector<int> pop(S);
    for (int i = 1; i < S; ++i) {
        pop[i] = pop[i >> 1] + (i & 1);
    }

    for (int pos = 0; pos < n; ++pos) {
        fill(ndp.begin(), ndp.end(), 0);

        for (int mask = 0; mask < S; ++mask) {
            if (dp[mask] == 0) continue;

            for (int bit = 0; bit <= 1; ++bit) {
                int newMask = ((mask << 1) | bit) & maskLimit;

                if (pos + 1 >= m && pop[newMask] < k) {
                    continue;
                }

                ndp[newMask] += dp[mask];
            }
        }

        dp.swap(ndp);
    }

    unsigned long long ans = 0;
    for (unsigned long long x : dp) {
        ans += x;
    }

    cout << ans << '\n';

    return 0;
}
