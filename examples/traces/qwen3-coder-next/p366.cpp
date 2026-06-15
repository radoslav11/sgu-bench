#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, K;
    cin >> N >> K;
    vector<pair<int, int>> opponents;
    for (int i = 0; i < N; i++) {
        int a, b;
        cin >> a >> b;
        opponents.push_back({a, b});
    }

    int base = 50 * K;
    int max_d = 100 * K;

    vector<vector<long long>> dp(K+1, vector<long long>(max_d+1, -INF));
    vector<vector<int>> parent_d(K+1, vector<int>(max_d+1, -1));
    vector<vector<int>> parent_opponent(K+1, vector<int>(max_d+1, -1));

    dp[0][base] = 0;

    for (int i = 0; i < N; i++) {
        int a = opponents[i].first;
        int b = opponents[i].second;
        int da = a - b;
        int ab_sum = a + b;

        for (int j = K; j >= 1; j--) {
            if (da >= 0) {
                for (int d_index = max_d; d_index >= da; d_index--) {
                    int prev_d_index = d_index - da;
                    if (dp[j-1][prev_d_index] == -INF) continue;
                    long long candidate = dp[j-1][prev_d_index] + ab_sum;
                    if (candidate > dp[j][d_index]) {
                        dp[j][d_index] = candidate;
                        parent_d[j][d_index] = prev_d_index;
                        parent_opponent[j][d_index] = i;
                    }
                }
            } else {
                int da_negative = -da;
                for (int d_index = da_negative; d_index <= max_d; d_index++) {
                    int prev_d_index = d_index + da;
                    if (dp[j-1][prev_d_index] == -INF) continue;
                    long long candidate = dp[j-1][prev_d_index] + ab_sum;
                    if (candidate > dp[j][d_index]) {
                        dp[j][d_index] = candidate;
                        parent_d[j][d_index] = prev_d_index;
                        parent_opponent[j][d_index] = i;
                    }
                }
            }
        }
    }

    int best_d_index = -1;
    long long best_sum = -INF;
    int best_abs_diff = INT_MAX;

    for (int d_index = 0; d_index <= max_d; d_index++) {
        if (dp[K][d_index] == -INF) continue;
        int d_val = d_index - base;
        int abs_diff = abs(d_val);
        long long sum_ab = dp[K][d_index];
        if (abs_diff < best_abs_diff || (abs_diff == best_abs_diff && sum_ab > best_sum)) {
            best_abs_diff = abs_diff;
            best_sum = sum_ab;
            best_d_index = d_index;
        }
    }

    vector<int> chosen_indices;
    int j = K;
    int d_idx = best_d_index;
    while (j > 0) {
        int opponent_index = parent_opponent[j][d_idx];
        chosen_indices.push_back(opponent_index);
        int prev_d_idx = parent_d[j][d_idx];
        d_idx = prev_d_idx;
        j = j - 1;
    }

    long long A = (best_sum + (best_d_index - base)) / 2;
    long long B = (best_sum - (best_d_index - base)) / 2;
    cout << A << " " << B << "\n";

    sort(chosen_indices.begin(), chosen_indices.end());
    for (int i = 0; i < chosen_indices.size(); i++) {
        if (i > 0) cout << " ";
        cout << chosen_indices[i] + 1;
    }
    cout << "\n";

    return 0;
}
