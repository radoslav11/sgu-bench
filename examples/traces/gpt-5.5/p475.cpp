#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> w(n), t(n), s(n);
    for (int i = 0; i < n; ++i) {
        cin >> w[i] >> t[i] >> s[i];
    }

    int N = 1 << n;

    vector<int> weight(N, 0), changeCost(N, 0), maxWalk(N, 0);

    for (int mask = 0; mask < N; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                weight[mask] += w[i];
                changeCost[mask] += s[i];
            } else {
                maxWalk[mask] = max(maxWalk[mask], t[i]);
            }
        }
    }

    const long long INF = (long long)4e18;

    vector<long long> dp(N, INF);
    dp[0] = 0;

    for (int riffle = 0; riffle < m; ++riffle) {
        int c, D, d;
        cin >> c >> D >> d;

        vector<long long> best = dp;

        for (int bit = 0; bit < n; ++bit) {
            int b = 1 << bit;
            for (int mask = 0; mask < N; ++mask) {
                if ((mask & b) == 0) {
                    long long x = best[mask];
                    long long y = best[mask | b];

                    best[mask] = min(x, y + s[bit]);
                    best[mask | b] = min(y, x + s[bit]);
                }
            }
        }

        vector<long long> ndp(N, INF);

        for (int mask = 1; mask < N; ++mask) {
            int raftTime = (weight[mask] > c ? D : d);
            int segmentTime = max(raftTime, maxWalk[mask]);
            ndp[mask] = best[mask] + segmentTime;
        }

        dp.swap(ndp);
    }

    long long answer = INF;
    for (int mask = 1; mask < N; ++mask) {
        answer = min(answer, dp[mask] + changeCost[mask]);
    }

    cout << answer << '\n';

    return 0;
}
