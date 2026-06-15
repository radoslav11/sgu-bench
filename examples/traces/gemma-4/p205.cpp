#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Problem: Quantization Problem
 * 
 * The goal is to minimize the deviation (sum of absolute differences) 
 * between input values and quantized values. The quantization level set 
 * for the next measure is determined by the least significant bits 
 * of the current level's ordinal index.
 * 
 * Approach:
 * Dynamic Programming
 * Let dp[j][i] be the minimum deviation for the remaining values (x_j, ..., x_n),
 * given that the j-th value is quantized using level set L_i.
 * The transition is:
 * dp[j][i] = min_{0 <= k < s} { |x_j - L_i[k]| + dp[j+1][k % m] }
 * where m = 2^p and s = 2^q. The bitwise operator k & (m-1) is equivalent to k % m.
 * 
 * Complexity:
 * Number of states: n * m = 1000 * 128 = 128,000
 * Transitions per state: s = 128
 * Total complexity: O(n * m * s) = 1.6 * 10^7 operations.
 * This should fit within the 0.25s time limit using optimized C++.
 * 
 * Constraints:
 * n <= 1000
 * m, s <= 128
 * x_i, levels <= 10^6
 * Memory limit: 65536 KB
 */

using namespace std;

// Using static to allocate in the data segment rather than the stack.
// This is safe and avoids stack overflow for larger n.
static int dp[1005][130];
static int path[1005][130];
static int levels[130][130];
static int x[1005];

int main() {
    // Speed up I/O operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // Read the n input values.
    for (int i = 1; i <= n; ++i) {
        cin >> x[i];
    }

    int m, s;
    if (!(cin >> m >> s)) return 0;

    // Read the level sets.
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < s; ++j) {
            cin >> levels[i][j];
        }
    }

    // Base case: when no more values are left, the deviation is 0.
    for (int i = 0; i < m; ++i) {
        dp[n + 1][i] = 0;
    }

    // m is a power of 2, so k % m is equivalent to k & (m-1).
    int m_mask = m - 1;

    // DP from n down to 1.
    for (int j = n; j >= 1; --j) {
        int xj = x[j];
        for (int i = 0; i < m; ++i) {
            int min_val = 2000000000; // Sufficiently large value (max possible is 10^9).
            int best_k = -1;
            int* levels_i = levels[i];
            for (int k = 0; k < s; ++k) {
                // Calculate |x_j - L_i[k]|.
                int diff = xj - levels_i[k];
                if (diff < 0) diff = -diff;
                
                // current_val = |x_j - L_i[k]| + dp[j+1][next_set_index].
                int current_val = diff + dp[j + 1][k & m_mask];
                
                if (current_val < min_val) {
                    min_val = current_val;
                    best_k = k;
                }
            }
            dp[j][i] = min_val;
            path[j][i] = best_k;
        }
    }

    // The first value is always quantized using set L0.
    cout << dp[1][0] << "\n";

    // Backtrack to reconstruct the chosen levels' indices.
    int current_set = 0;
    for (int j = 1; j <= n; ++j) {
        int k = path[j][current_set];
        cout << k << (j == n ? "" : " ");
        // The next set is determined by the current index k.
        current_set = k & m_mask;
    }
    cout << "\n";

    return 0;
}

