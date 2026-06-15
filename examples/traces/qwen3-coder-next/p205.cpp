#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> x(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i];
    }

    int m, s;
    cin >> m >> s;
    vector<vector<int>> levels(m, vector<int>(s));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < s; j++) {
            cin >> levels[i][j];
        }
    }

    const long long INF = 1e18;
    vector<vector<long long>> dp(n + 1, vector<long long>(m, INF));
    vector<vector<int>> prev_set(n + 1, vector<int>(m, -1));
    vector<vector<int>> chosen_k(n + 1, vector<int>(m, -1));

    dp[0][0] = 0;

    for (int i = 0; i < n; i++) {
        for (int cur_set = 0; cur_set < m; cur_set++) {
            if (dp[i][cur_set] == INF) continue;
            for (int k = 0; k < s; k++) {
                int next_s = k & (m - 1);
                long long cost = abs((long long)x[i] - levels[cur_set][k]);
                long long new_val = dp[i][cur_set] + cost;
                if (new_val < dp[i + 1][next_s]) {
                    dp[i + 1][next_s] = new_val;
                    prev_set[i + 1][next_s] = cur_set;
                    chosen_k[i + 1][next_s] = k;
                }
            }
        }
    }

    long long min_dev = INF;
    int final_set = -1;
    for (int s_idx = 0; s_idx < m; s_idx++) {
        if (dp[n][s_idx] < min_dev) {
            min_dev = dp[n][s_idx];
            final_set = s_idx;
        }
    }

    cout << min_dev << "\n";

    vector<int> result(n);
    int cur_set = final_set;
    for (int i = n; i >= 1; i--) {
        result[i - 1] = chosen_k[i][cur_set];
        cur_set = prev_set[i][cur_set];
    }

    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";

    return 0;
}
