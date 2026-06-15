#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: 372. Tea Party
 * Alice organizes a tea party for K guests with N types of tea (black/green).
 * In each round r (from 1 to K), all current guests get the same tea.
 * Number of guests in round r: K - r + 1.
 * After each round, one guest leaves.
 * Total cost: sum of (K-r+1) * cost of tea in round r.
 * Constraints:
 * 1. All tea types used must be distinct.
 * 2. No three consecutive rounds can use the same kind of tea.
 * Minimize total cost.
 * 
 * DP Approach:
 * dp[r][c0][state] = minimum cost from round r to K, given c0 green teas were used in rounds 1 to r-1,
 * and the tea used in round r-1 had state (type, consecutive_count).
 * state 0: (green, 1), state 1: (green, 2), state 2: (black, 1), state 3: (black, 2).
 * 
 * Complexity: O(K*N) states, each O(1) transitions.
 * For K, N <= 1000, total operations ~ 8*10^6, fits in 0.25s.
 * Memory: dp[1002][1001][4] (8 bytes) + choice[1002][1001][4] (1 byte) ≈ 36 MB, fits in 64 MB.
 */

const long long INF = 1e18;

struct Tea {
    int cost;
    int id;
};

long long dp[1002][1002][4];
unsigned char choice[1002][1002][4];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K, N;
    if (!(cin >> K >> N)) return 0;

    vector<Tea> green_teas, black_teas;
    for (int i = 1; i <= N; ++i) {
        int c, s;
        cin >> c >> s;
        if (s == 0) green_teas.push_back({c, i});
        else black_teas.push_back({c, i});
    }

    sort(green_teas.begin(), green_teas.end(), [](const Tea& a, const Tea& b) {
        return a.cost < b.cost;
    });
    sort(black_teas.begin(), black_teas.end(), [](const Tea& a, const Tea& b) {
        return a.cost < b.cost;
    });

    int N0 = green_teas.size();
    int N1 = black_teas.size();

    // Initializing DP table
    for (int r = 1; r <= K + 1; ++r) {
        for (int c0 = 0; c0 <= N0; ++c0) {
            for (int s = 0; s < 4; ++s) {
                dp[r][c0][s] = INF;
            }
        }
    }

    // Base case: r = K + 1
    for (int c0 = 0; c0 <= N0; ++c0) {
        int c1 = K - c0;
        if (c1 >= 0 && c1 <= N1) {
            for (int s = 0; s < 4; ++s) {
                dp[K + 1][c0][s] = 0;
            }
        }
    }

    // Iterative DP from r = K down to 2
    for (int r = K; r >= 2; --r) {
        for (int c0 = 0; c0 <= N0; ++c0) {
            int c1 = (r - 1) - c0;
            if (c1 < 0 || c1 > N1) continue;

            for (int s = 0; s < 4; ++s) {
                int type = s / 2;
                int count = s % 2 + 1;

                // Option x_r = 0 (Green)
                long long cost0 = INF;
                if (!(type == 0 && count == 2)) {
                    if (c0 < N0) {
                        int next_s = (type == 0 ? 1 : 0);
                        long long next_val = dp[r + 1][c0 + 1][next_s];
                        if (next_val != INF) {
                            cost0 = (long long)(K - r + 1) * green_teas[c0].cost + next_val;
                        }
                    }
                }

                // Option x_r = 1 (Black)
                long long cost1 = INF;
                if (!(type == 1 && count == 2)) {
                    if (c1 < N1) {
                        int next_s = (type == 1 ? 3 : 2);
                        long long next_val = dp[r + 1][c0][next_s];
                        if (next_val != INF) {
                            cost1 = (long long)(K - r + 1) * black_teas[c1].cost + next_val;
                        }
                    }
                }

                if (cost0 <= cost1 && cost0 != INF) {
                    dp[r][c0][s] = cost0;
                    choice[r][c0][s] = 0;
                } else if (cost1 != INF) {
                    dp[r][c0][s] = cost1;
                    choice[r][c0][s] = 1;
                }
            }
        }
    }

    // Special case: round r = 1
    long long min_total_cost = INF;
    int ans_x1 = -1;

    // x1 = 0
    if (N0 > 0) {
        long long c0_val = (long long)K * green_teas[0].cost + dp[2][1][0];
        if (c0_val < min_total_cost) {
            min_total_cost = c0_val;
            ans_x1 = 0;
        }
    }
    // x1 = 1
    if (N1 > 0) {
        long long c1_val = (long long)K * black_teas[0].cost + dp[2][0][2];
        if (c1_val < min_total_cost) {
            min_total_cost = c1_val;
            ans_x1 = 1;
        }
    }

    if (min_total_cost >= INF) {
        cout << "Impossible" << endl;
        return 0;
    }

    // Reconstruct the sequence
    vector<int> result_indices;
    int cur_c0 = 0;
    int cur_type = ans_x1;
    int cur_count = 1;

    if (ans_x1 == 0) {
        result_indices.push_back(green_teas[0].id);
        cur_c0 = 1;
    } else {
        result_indices.push_back(black_teas[0].id);
        cur_c0 = 0;
    }

    for (int r = 2; r <= K; ++r) {
        int s = cur_type * 2 + (cur_count - 1);
        int next_x = choice[r][cur_c0][s];
        if (next_x == 0) {
            result_indices.push_back(green_teas[cur_c0].id);
            cur_count = (cur_type == 0 ? cur_count + 1 : 1);
            cur_type = 0;
            cur_c0++;
        } else {
            int c1 = (r - 1) - cur_c0;
            result_indices.push_back(black_teas[c1].id);
            cur_count = (cur_type == 1 ? cur_count + 1 : 1);
            cur_type = 1;
        }
    }

    for (int i = 0; i < K; ++i) {
        cout << result_indices[i] << (i == K - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

