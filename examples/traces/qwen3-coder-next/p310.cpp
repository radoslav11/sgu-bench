#include <iostream>
#include <vector>
using namespace std;

int main() {
    long long n, m, k;
    cin >> n >> m >> k;

    int num_states = 1 << (m - 1);
    vector<long long> dp(num_states, 0);
    dp[0] = 1;

    for (int i = 0; i < n; i++) {
        vector<long long> new_dp(num_states, 0);
        for (int state = 0; state < num_states; state++) {
            if (dp[state] == 0) 
                continue;
            for (int b = 0; b < 2; b++) {
                if (i + 1 < m) {
                    int new_state = ((state << 1) | b) & (num_states - 1);
                    new_dp[new_state] += dp[state];
                } else {
                    int window = (state << 1) | b;
                    if (__builtin_popcount((unsigned int)window) >= k) {
                        int new_state = ((state << 1) | b) & (num_states - 1);
                        new_dp[new_state] += dp[state];
                    }
                }
            }
        }
        dp = new_dp;
    }

    long long ans = 0;
    for (int state = 0; state < num_states; state++) {
        ans += dp[state];
    }
    cout << ans << endl;

    return 0;
}
