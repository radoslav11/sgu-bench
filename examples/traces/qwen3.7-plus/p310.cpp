#include <iostream>
#include <vector>
#include <utility>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    int m, k;
    if (!(cin >> n >> m >> k)) return 0;

    int num_states = 1 << (m - 1);
    vector<uint64_t> dp(num_states, 0);
    dp[0] = 1;

    for (long long i = 0; i < n; ++i) {
        vector<uint64_t> next_dp(num_states, 0);
        for (int mask = 0; mask < num_states; ++mask) {
            if (dp[mask] == 0) continue;

            for (int c = 0; c < 2; ++c) {
                if (i + 1 >= m) {
                    if (__builtin_popcount(mask) + c < k) {
                        continue;
                    }
                }
                int next_mask = ((mask << 1) | c) & (num_states - 1);
                next_dp[next_mask] += dp[mask];
            }
        }
        dp = move(next_dp);
    }

    uint64_t ans = 0;
    for (int mask = 0; mask < num_states; ++mask) {
        ans += dp[mask];
    }

    cout << ans << "\n";

    return 0;
}
