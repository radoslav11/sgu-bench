#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> y(n);
    for (int i = 0; i < n; ++i) cin >> y[i];

    if (n <= 2) {
        cout << "YES\n";
        return 0;
    }

    int m = n - 1; // segments: [i, i+1], 0 <= i < m

    vector<long long> lo(m), hi(m);
    for (int i = 0; i < m; ++i) {
        lo[i] = min(y[i], y[i + 1]);
        hi[i] = max(y[i], y[i + 1]);
    }

    auto intersect = [&](int a, int b) -> bool {
        return max(lo[a], lo[b]) <= min(hi[a], hi[b]);
    };

    vector<vector<char>> dp(m, vector<char>(m, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = m - 1; j >= 0; --j) {
            if (!intersect(i, j)) continue;

            if (i == 0 && j == m - 1) {
                dp[i][j] = 1;
            } else {
                if (i > 0 && dp[i - 1][j]) dp[i][j] = 1;
                if (j + 1 < m && dp[i][j + 1]) dp[i][j] = 1;
            }

            if (dp[i][j] && i >= j) {
                cout << "YES\n";
                return 0;
            }
        }
    }

    cout << "NO\n";
    return 0;
}
