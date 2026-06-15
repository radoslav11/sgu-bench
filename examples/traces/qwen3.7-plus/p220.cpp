#include <iostream>
#include <vector>
#include <map>

using namespace std;

vector<long long> solve_color(int n, int parity) {
    vector<vector<int>> diagonals;
    map<int, int> rc_to_id;
    int current_id = 0;

    for (int sum = parity; sum < 2 * n; sum += 2) {
        vector<int> current_diag;
        for (int r = 0; r < n; r++) {
            int c = sum - r;
            if (c >= 0 && c < n) {
                int rc = r - c;
                if (rc_to_id.find(rc) == rc_to_id.end()) {
                    rc_to_id[rc] = current_id++;
                }
                current_diag.push_back(rc_to_id[rc]);
            }
        }
        diagonals.push_back(current_diag);
    }

    int m = current_id;
    int max_mask = 1 << m;
    int max_k = n; 
    
    vector<vector<long long>> dp(max_mask, vector<long long>(max_k + 1, 0));
    dp[0][0] = 1;

    for (const auto& diag : diagonals) {
        vector<vector<long long>> next_dp(max_mask, vector<long long>(max_k + 1, 0));
        for (int mask = 0; mask < max_mask; mask++) {
            for (int k = 0; k <= max_k; k++) {
                if (dp[mask][k] == 0) continue;

                // Option 1: place no bishop on this diagonal
                next_dp[mask][k] += dp[mask][k];

                // Option 2: place a bishop on one of the squares in this diagonal
                for (int id : diag) {
                    if ((mask & (1 << id)) == 0) {
                        next_dp[mask | (1 << id)][k + 1] += dp[mask][k];
                    }
                }
            }
        }
        dp = move(next_dp);
    }

    vector<long long> ways(max_k + 1, 0);
    for (int mask = 0; mask < max_mask; mask++) {
        for (int k = 0; k <= max_k; k++) {
            ways[k] += dp[mask][k];
        }
    }
    return ways;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> ways_even = solve_color(n, 0);
    vector<long long> ways_odd = solve_color(n, 1);

    vector<long long> total_ways(n * n + 1, 0);
    for (size_t i = 0; i < ways_even.size(); i++) {
        for (size_t j = 0; j < ways_odd.size(); j++) {
            if (i + j <= n * n) {
                total_ways[i + j] += ways_even[i] * ways_odd[j];
            }
        }
    }

    if (k <= n * n) {
        cout << total_ways[k] << "\n";
    } else {
        cout << 0 << "\n";
    }

    return 0;
}
