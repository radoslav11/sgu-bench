#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MOD = 1000000009;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    int total = n + k;
    if (total % 2 != 0) {
        cout << 0 << endl;
        return 0;
    }
    int w = total / 2;
    if (w < 0 || w > n) {
        cout << 0 << endl;
        return 0;
    }

    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    dp[0][0] = 1;

    for (int i = 0; i < n; i++) {
        int cnt = lower_bound(b.begin(), b.end(), a[i]) - b.begin();
        for (int j = 0; j <= i; j++) {
            if (dp[i][j] == 0) continue;
            if (cnt > j) {
                long long ways_win = cnt - j;
                dp[i+1][j+1] = (dp[i+1][j+1] + dp[i][j] * ways_win) % MOD;
            }
            long long loss_available = (n - i) - (cnt - j);
            if (loss_available > 0) {
                dp[i+1][j] = (dp[i+1][j] + dp[i][j] * loss_available) % MOD;
            }
        }
    }

    cout << dp[n][w] % MOD << endl;
    return 0;
}
