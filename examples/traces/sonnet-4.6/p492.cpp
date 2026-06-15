#include <bits/stdc++.h>
using namespace std;
long long countValid(long long X) {
    if (X <= 0) return 0;
    string s = to_string(X);
    int n = s.size();
    long long dp[2][11];
    memset(dp, 0, sizeof(dp));
    dp[1][10] = 1;
    for (int i = 0; i < n; i++) {
        long long ndp[2][11];
        memset(ndp, 0, sizeof(ndp));
        int lim = s[i] - '0';
        for (int tight = 0; tight <= 1; tight++)
            for (int prev = 0; prev <= 10; prev++) {
                if (!dp[tight][prev]) continue;
                int maxd = tight ? lim : 9;
                for (int d = 0; d <= maxd; d++) {
                    if (prev == 1 && d == 3) continue;
                    int ntight = tight && (d == maxd);
                    int nprev = (prev == 10 && d == 0) ? 10 : d;
                    ndp[ntight][nprev] += dp[tight][prev];
                }
            }
        memcpy(dp, ndp, sizeof(dp));
    }
    long long res = 0;
    for (int tight = 0; tight <= 1; tight++)
        for (int prev = 0; prev <= 9; prev++)
            res += dp[tight][prev];
    return res;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T; cin >> T;
    while (T--) {
        long long N; cin >> N;
        long long lo = 1, hi = (long long)2e18;
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (countValid(mid) >= N) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << "\n";
    }
}
