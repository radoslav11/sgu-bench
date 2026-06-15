#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    int M, N;
    cin >> M >> N;
    vector<string> grid(M);
    for (int i = 0; i < M; i++) {
        cin >> grid[i];
    }

    // Precompute valid masks for each row: masks that have no two adjacent 1-bits and do not have 1-bits at candle positions.
    vector<vector<int>> valid_masks(M);
    for (int i = 0; i < M; i++) {
        for (int mask = 0; mask < (1 << N); mask++) {
            if (mask & (mask << 1)) {
                continue;
            }
            bool valid = true;
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == '*' && (mask & (1 << j))) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                valid_masks[i].push_back(mask);
            }
        }
    }

    // dp[a][b] = maximum uncovered squares from row 0 to row i-1, 
    //   where a is the uncovered status of row i-1, b for row i.
    // We'll use a 2D vector of size (1<<N) x (1<<N), initialize to -1.
    int total_masks = 1 << N;
    vector<vector<int>> dp(total_masks, vector<int>(total_masks, -1));
    dp[0][0] = 0;

    for (int i = 0; i < M; i++) {
        vector<vector<int>> new_dp(total_masks, vector<int>(total_masks, -1));
        vector<int>& prev_list = (i == 0) ? vector<int>({0}) : valid_masks[i-1];
        vector<int>& curr_list = valid_masks[i];
        vector<int>& next_list = (i == M-1) ? vector<int>({0}) : valid_masks[i+1];

        for (int a : prev_list) {
            for (int b : curr_list) {
                if (dp[a][b] == -1) continue;
                for (int c : next_list) {
                    // Check vertical adjacency between row i-1 and i: a and b should not share a 1-bit (if i>0)
                    if (i > 0 && (a & b)) {
                        continue;
                    }
                    // Check vertical adjacency between row i and i+1: b and c should not share a 1-bit
                    if (b & c) {
                        continue;
                    }

                    // Check for row i-1: for each column j, if grid[i-1][j] is '.' and a has j-th bit 0 (meaning (i-1,j) is not uncovered), 
                    // then grid[i][j] must be '.' (so that we can cover (i-1,j) with a vertical domino to (i,j))
                    if (i > 0) {
                        bool ok = true;
                        for (int j = 0; j < N; j++) {
                            if (grid[i-1][j] == '.' && !(a & (1 << j)) && grid[i][j] != '.') {
                                ok = false;
                                break;
                            }
                        }
                        if (!ok) continue;
                    }

                    // Compute R_mask for row i: columns j where (i,j) is empty, not uncovered (b[j]==0), and not covered by vertical from row i-1.
                    int R_mask = 0;
                    for (int j = 0; j < N; j++) {
                        if (grid[i][j] == '.') {
                            if (b & (1 << j)) {
                                // uncovered
                                continue;
                            }
                            if (i > 0 && grid[i-1][j] == '.' && !(a & (1 << j))) {
                                // covered by vertical domino from row i-1
                                continue;
                            }
                            R_mask |= (1 << j);
                        }
                    }

                    // Check if R_mask can be covered by horizontal dominoes (i.e., no two adjacent uncovered in R_mask, and they come in pairs)
                    int j = 0;
                    bool validR = true;
                    while (j < N) {
                        if (R_mask & (1 << j)) {
                            if (j + 1 >= N || !(R_mask & (1 << (j+1)))) {
                                validR = false;
                                break;
                            }
                            j += 2;
                        } else {
                            j++;
                        }
                    }
                    if (!validR) continue;

                    int add = __builtin_popcount(b);
                    int new_val = dp[a][b] + add;
                    if (new_val > new_dp[b][c]) {
                        new_dp[b][c] = new_val;
                    }
                }
            }
        }
        dp = move(new_dp);
    }

    int max_uncovered = 0;
    for (int a = 0; a < total_masks; a++) {
        for (int b = 0; b < total_masks; b++) {
            if (dp[a][b] > max_uncovered) {
                max_uncovered = dp[a][b];
            }
        }
    }

    int total_empty = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == '.') {
                total_empty++;
            }
        }
    }

    int dominoes = (total_empty - max_uncovered) / 2;
    cout << dominoes << endl;

    return 0;
}
