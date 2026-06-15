#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> w(n), t(n), s(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> t[i] >> s[i];
    }

    vector<int> c(m), D(m), d(m);
    for (int i = 0; i < m; i++) {
        cin >> c[i] >> D[i] >> d[i];
    }

    int total_masks = 1 << n;
    vector<long long> cost_mask(total_masks, 0);
    vector<int> weight(total_masks, 0);
    vector<int> walking_time(total_masks, 0);

    for (int mask = 0; mask < total_masks; mask++) {
        for (int j = 0; j < n; j++) {
            if (mask & (1 << j)) {
                cost_mask[mask] += s[j];
                weight[mask] += w[j];
            } else {
                walking_time[mask] = max(walking_time[mask], t[j]);
            }
        }
    }

    const long long INF = 1e15;
    vector<long long> dp(total_masks, INF);
    dp[0] = 0;

    for (int i = 0; i < m; i++) {
        vector<long long> A(total_masks, INF);
        for (int mask = 0; mask < total_masks; mask++) {
            if (mask == 0) {
                A[mask] = INF;
            } else {
                long long raft_time = (weight[mask] > c[i]) ? D[i] : d[i];
                A[mask] = max(raft_time, (long long)walking_time[mask]);
            }
        }

        vector<long long> F(total_masks, INF);
        for (int V = 0; V < total_masks; V++) {
            for (int U = 0; U < total_masks; U++) {
                if (dp[U] >= INF) continue;
                int x = U ^ V;
                long long candidate = dp[U] + cost_mask[x];
                if (candidate < F[V]) {
                    F[V] = candidate;
                }
            }
        }

        vector<long long> next_dp(total_masks, INF);
        for (int W = 0; W < total_masks; W++) {
            for (int V = 1; V < total_masks; V++) {
                if (A[V] >= INF) continue;
                int x = V ^ W;
                long long candidate = A[V] + cost_mask[x] + F[V];
                if (candidate < next_dp[W]) {
                    next_dp[W] = candidate;
                }
            }
        }

        dp = move(next_dp);
    }

    cout << dp[0] << endl;

    return 0;
}
