#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: I-country
 * Goal: Select exactly K squares in an N*M grid such that they form a convex polyomino,
 * maximizing the total amount of oil.
 * A convex polyomino is a set of squares where each row and column segment is contiguous.
 * In terms of row segments [L_i, R_i], it means:
 * 1. For each row i, L_i <= R_i.
 * 2. For all i, [L_i, R_i] overlaps with [L_{i+1}, R_{i+1}].
 * 3. L_i sequence is U-shaped (non-increasing then non-decreasing).
 * 4. R_i sequence is inverted U-shaped (non-decreasing then non-increasing).
 */

int N, M, K;
int grid[16][16];
int oil_sum[16][16][16];
int dp[16][16][16][2][2][226];

struct Parent {
    unsigned char pl, pr, psL, psR;
    bool is_start;
} parent[16][16][16][2][2][226];

int main() {
    // Speed up I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> M >> K)) return 0;

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            cin >> grid[i][j];
        }
    }

    if (K == 0) {
        cout << "Oil : 0" << endl;
        return 0;
    }

    // Precompute oil sum in each row range [l, r]
    for (int i = 1; i <= N; ++i) {
        for (int l = 1; l <= M; ++l) {
            int current_sum = 0;
            for (int r = l; r <= M; ++r) {
                current_sum += grid[i][r];
                oil_sum[i][l][r] = current_sum;
            }
        }
    }

    // Initialize DP table with -1
    for (int i = 0; i <= N; ++i) {
        for (int l = 0; l <= M; ++l) {
            for (int r = 0; r <= M; ++r) {
                for (int sL = 0; sL < 2; ++sL) {
                    for (int sR = 0; sR < 2; ++sR) {
                        for (int k = 0; k <= K; ++k) {
                            dp[i][l][r][sL][sR][k] = -1;
                        }
                    }
                }
            }
        }
    }

    int max_oil = -1;
    int best_i = -1, best_l = -1, best_r = -1, best_sL = -1, best_sR = -1;

    for (int i = 1; i <= N; ++i) {
        // Case 1: Start a new polyomino at row i
        for (int l = 1; l <= M; ++l) {
            for (int r = l; r <= M; ++r) {
                int k = r - l + 1;
                if (k <= K) {
                    dp[i][l][r][0][0][k] = oil_sum[i][l][r];
                    parent[i][l][r][0][0][k] = {0, 0, 0, 0, true};
                }
            }
        }

        // Case 2: Extend an existing polyomino from row i-1
        if (i > 1) {
            for (int k = 1; k < K; ++k) {
                for (int l = 1; l <= M; ++l) {
                    for (int r = l; r <= M; ++r) {
                        for (int sL = 0; sL < 2; ++sL) {
                            for (int sR = 0; sR < 2; ++sR) {
                                if (dp[i-1][l][r][sL][sR][k] == -1) continue;
                                
                                for (int lp = 1; lp <= M; ++lp) {
                                    for (int rp = lp; rp <= M; ++rp) {
                                        int nk = k + (rp - lp + 1);
                                        if (nk > K) continue;
                                        // Check connectivity condition: [l, r] and [lp, rp] must overlap
                                        if (lp <= r && l <= rp) {
                                            // Calculate new states for L (U-shaped)
                                            int nsL = -1;
                                            if (sL == 0) {
                                                if (lp < l) nsL = 0;
                                                else if (lp == l) nsL = 0;
                                                else nsL = 1;
                                            } else { // sL == 1
                                                if (lp >= l) nsL = 1;
                                            }
                                            if (nsL == -1) continue;

                                            // Calculate new states for R (inverted U-shaped)
                                            int nsR = -1;
                                            if (sR == 0) { // Increasing
                                                if (rp > r) nsR = 0;
                                                else if (rp == r) nsR = 0;
                                                else nsR = 1;
                                            } else { // sR == 1: Decreasing
                                                if (rp <= r) nsR = 1;
                                            }
                                            if (nsR == -1) continue;

                                            int new_val = dp[i-1][l][r][sL][sR][k] + oil_sum[i][lp][rp];
                                            if (new_val > dp[i][lp][rp][nsL][nsR][nk]) {
                                                dp[i][lp][rp][nsL][nsR][nk] = new_val;
                                                parent[i][lp][rp][nsL][nsR][nk] = {(unsigned char)l, (unsigned char)r, (unsigned char)sL, (unsigned char)sR, false};
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Check for the maximum oil at current row i with exactly K squares
        for (int l = 1; l <= M; ++l) {
            for (int r = l; r <= M; ++r) {
                for (int sL = 0; sL < 2; ++sL) {
                    for (int sR = 0; sR < 2; ++sR) {
                        if (dp[i][l][r][sL][sR][K] > max_oil) {
                            max_oil = dp[i][l][r][sL][sR][K];
                            best_i = i; best_l = l; best_r = r; best_sL = sL; best_sR = sR;
                        }
                    }
                }
            }
        }
    }

    if (max_oil == -1) {
        // This should not happen given K >= 0 and constraints
        cout << "Oil : 0" << endl;
        return 0;
    }

    cout << "Oil : " << max_oil << endl;

    // Path reconstruction
    vector<pair<int, int>> result;
    int cur_i = best_i, cur_l = best_l, cur_r = best_r, cur_sL = best_sL, cur_sR = best_sR, cur_k = K;

    while (cur_i > 0) {
        for (int j = cur_l; j <= cur_r; ++j) {
            result.push_back({cur_i, j});
        }
        Parent p = parent[cur_i][cur_l][cur_r][cur_sL][cur_sR][cur_k];
        if (p.is_start) break;
        cur_k -= (cur_r - cur_l + 1);
        cur_i--;
        cur_l = p.pl;
        cur_r = p.pr;
        cur_sL = p.psL;
        cur_sR = p.psR;
    }

    for (auto &sq : result) {
        cout << sq.first << " " << sq.second << "\n";
    }

    return 0;
}

