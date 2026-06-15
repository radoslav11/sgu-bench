#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

/**
 * Problem: 310. Hippopotamus
 * The problem asks for the number of ways to arrange n boards, each being either wooden (0) or iron (1),
 * such that in every consecutive window of size m, there are at least k iron boards.
 * Constraints: 1 <= n <= 60, 1 <= m <= 15, 0 <= k <= m <= n.
 *
 * Approach:
 * We can use dynamic programming with a bitmask of size m.
 * dp[i][mask] represents the number of valid ways to arrange the first i boards,
 * where 'mask' is a bitmask representing the last m boards.
 *
 * State: dp[i][mask], where i is the number of boards placed so far (up to n)
 * and mask is an integer from 0 to 2^m - 1.
 *
 * Complexity:
 * Time complexity: O(n * 2^m), which is about 60 * 32768 = 1,966,080 operations.
 * Each operation consists of adding a bit and checking the popcount.
 * Space complexity: O(2^m) if we use only two rows for DP, which is well within the 64 MB limit.
 */

// We use a bitmask of size m (up to 15).
// dp[2][1 << 15] to store current and next row in DP.
long long dp[2][1 << 15];
// next_mask_arr[mask][bit] stores the resulting mask after adding 'bit' to the current window.
int next_mask_arr[1 << 15][2];
// is_valid[mask] stores whether the mask has at least k set bits.
bool is_valid[1 << 15];

int main() {
    // Speed up standard I/O.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    // Read input.
    if (!(cin >> n >> m >> k)) return 0;

    // m_mask is the bitmask representing all m bits.
    int m_mask = (1 << m) - 1;

    // Precompute next masks and validity for all possible masks.
    for (int mask = 0; mask <= m_mask; ++mask) {
        // __builtin_popcount is a fast way to count the number of set bits in an integer.
        int pc = __builtin_popcount(mask);
        is_valid[mask] = (pc >= k);
        
        // Adding 0 (wooden board): Shift current mask left and keep only last m bits.
        next_mask_arr[mask][0] = (mask << 1) & m_mask;
        // Adding 1 (iron board): Shift current mask left, add 1, and keep last m bits.
        next_mask_arr[mask][1] = ((mask << 1) | 1) & m_mask;
    }

    // Base case: Initialize DP for the first m boards.
    // All windows of size m must be valid from the start.
    // We initialize dp for i = m boards.
    int start_row = m % 2;
    for (int mask = 0; mask <= m_mask; ++mask) {
        if (is_valid[mask]) {
            dp[start_row][mask] = 1;
        } else {
            dp[start_row][mask] = 0;
        }
    }

    // Transitions: For each number of boards from m up to n-1.
    for (int i = m; i < n; ++i) {
        int cur = i % 2;
        int next = (i + 1) % 2;
        
        // Reset the next row of DP for current step.
        memset(dp[next], 0, sizeof(dp[next]));
        
        for (int mask = 0; mask <= m_mask; ++mask) {
            if (dp[cur][mask] == 0) continue;

            // Try adding a wooden board (0).
            int m0 = next_mask_arr[mask][0];
            if (is_valid[m0]) {
                dp[next][m0] += dp[cur][mask];
            }

            // Try adding an iron board (1).
            int m1 = next_mask_arr[mask][1];
            if (is_valid[m1]) {
                dp[next][m1] += dp[cur][mask];
            }
        }
    }

    // The final answer is the sum of all valid configurations for n boards.
    long long ans = 0;
    for (int mask = 0; mask <= m_mask; ++mask) {
        ans += dp[n % 2][mask];
    }

    // Output the final result.
    cout << ans << endl;

    return 0;
}

