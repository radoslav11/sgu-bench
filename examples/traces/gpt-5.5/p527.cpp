#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (int i = 0; i < n; ++i) cin >> grid[i];

    int N, M;
    vector<uint32_t> adj;

    if (n <= m) {
        N = n;
        M = m;
        adj.assign(N, 0);
        for (int i = 0; i < n; ++i) {
            uint32_t mask = 0;
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '*') mask |= (1u << j);
            }
            adj[i] = mask;
        }
    } else {
        N = m;
        M = n;
        adj.assign(N, 0);
        for (int j = 0; j < m; ++j) {
            uint32_t mask = 0;
            for (int i = 0; i < n; ++i) {
                if (grid[i][j] == '*') mask |= (1u << i);
            }
            adj[j] = mask;
        }
    }

    uint32_t total = 1u << N;
    vector<uint32_t> masks(total, 0);

    int ans = min(n, m);

    for (uint32_t subset = 0; subset < total; ++subset) {
        if (subset != 0) {
            uint32_t lsb = subset & -subset;
            int bit = __builtin_ctz(lsb);
            masks[subset] = masks[subset ^ lsb] | adj[bit];
        }

        int unselectedA = __builtin_popcount(subset);
        int selectedA = N - unselectedA;
        int selectedB = __builtin_popcount(masks[subset]);

        ans = min(ans, max(selectedA, selectedB));
    }

    cout << ans << '\n';

    return 0;
}
