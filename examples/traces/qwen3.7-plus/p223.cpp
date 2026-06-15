#include <iostream>
#include <vector>

using namespace std;

int get_popcount(int x) {
    int cnt = 0;
    while (x) {
        cnt += (x & 1);
        x >>= 1;
    }
    return cnt;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> valid_masks;
    for (int mask = 0; mask < (1 << n); ++mask) {
        if ((mask & (mask << 1)) == 0) {
            valid_masks.push_back(mask);
        }
    }

    int num_masks = valid_masks.size();
    vector<vector<int>> valid_prev(num_masks);

    for (int cur_idx = 0; cur_idx < num_masks; ++cur_idx) {
        for (int prev_idx = 0; prev_idx < num_masks; ++prev_idx) {
            int m1 = valid_masks[prev_idx];
            int m2 = valid_masks[cur_idx];
            if ((m1 & m2) == 0 && (m1 & (m2 << 1)) == 0 && ((m1 << 1) & m2) == 0) {
                valid_prev[cur_idx].push_back(prev_idx);
            }
        }
    }

    vector<vector<long long>> dp_prev(k + 1, vector<long long>(num_masks, 0));
    int zero_idx = -1;
    for (int i = 0; i < num_masks; ++i) {
        if (valid_masks[i] == 0) {
            zero_idx = i;
            break;
        }
    }

    dp_prev[0][zero_idx] = 1;

    for (int i = 1; i <= n; ++i) {
        vector<vector<long long>> dp_curr(k + 1, vector<long long>(num_masks, 0));
        for (int cur_idx = 0; cur_idx < num_masks; ++cur_idx) {
            int m = valid_masks[cur_idx];
            int c = get_popcount(m);
            for (int k_kings = c; k_kings <= k; ++k_kings) {
                long long ways = 0;
                for (int prev_idx : valid_prev[cur_idx]) {
                    ways += dp_prev[k_kings - c][prev_idx];
                }
                dp_curr[k_kings][cur_idx] = ways;
            }
        }
        dp_prev = move(dp_curr);
    }

    long long total_ways = 0;
    for (int i = 0; i < num_masks; ++i) {
        total_ways += dp_prev[k][i];
    }

    cout << total_ways << "\n";

    return 0;
}
