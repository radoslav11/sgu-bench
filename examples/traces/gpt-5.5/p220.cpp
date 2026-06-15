#include <bits/stdc++.h>
using namespace std;

vector<long long> countWays(vector<int> diagonals, int k) {
    sort(diagonals.begin(), diagonals.end());

    vector<long long> dp(k + 1, 0);
    dp[0] = 1;

    int processed = 0;
    for (int len : diagonals) {
        processed++;
        for (int placed = min(k, processed); placed >= 1; --placed) {
            int available = len - (placed - 1);
            if (available > 0) {
                dp[placed] += dp[placed - 1] * available;
            }
        }
    }

    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> diagonals[2];

    for (int d = 0; d <= 2 * n - 2; ++d) {
        int len = min({d + 1, 2 * n - 1 - d, n});
        diagonals[d % 2].push_back(len);
    }

    vector<long long> ways0 = countWays(diagonals[0], k);
    vector<long long> ways1 = countWays(diagonals[1], k);

    long long answer = 0;
    for (int i = 0; i <= k; ++i) {
        answer += ways0[i] * ways1[k - i];
    }

    cout << answer << '\n';

    return 0;
}
