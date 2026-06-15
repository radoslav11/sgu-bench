#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/**
 * The problem asks for the number of ways to place k bishops on an n x n chessboard 
 * such that no two bishops attack each other. Bishops on white squares and black 
 * squares are independent. We can decompose the problem into two independent 
 * problems for each color.
 * 
 * For each color, we can transform the bishop placement problem into a rook 
 * placement problem on a modified board by rotating it 45 degrees. The new 
 * "rows" are the diagonals of the original chessboard.
 * 
 * A bishop at (r, c) on an n x n board can be represented by its diagonal 
 * properties (r+c, r-c). For a fixed parity of (r+c), we want to place 
 * bishops such that no two are in the same (r-c) diagonal. This is 
 * analogous to placing non-attacking rooks on a board where each "row" 
 * is a diagonal of the original board.
 */

// Function to calculate the number of ways to place j rooks on a board 
// with given row lengths L using dynamic programming.
vector<long long> solve(const vector<int>& L, int k_max) {
    int m = L.size();
    // dp[i][j] = number of ways to place j rooks in the first i rows.
    // Since n <= 10, m (number of diagonals) will be at most 19.
    // k_max can be up to n^2 = 100.
    vector<vector<long long>> dp(m + 1, vector<long long>(k_max + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= m; ++i) {
        int row_len = L[i - 1];
        for (int j = 0; j <= k_max; ++j) {
            // Case 1: Do not place a bishop in the current row.
            dp[i][j] = dp[i - 1][j];
            // Case 2: Place a bishop in the current row.
            // There are row_len available columns in this row, but (j-1) 
            // columns have already been occupied by bishops in previous rows.
            if (j > 0 && row_len - (j - 1) > 0) {
                dp[i][j] += dp[i - 1][j - 1] * (row_len - (j - 1));
            }
        }
    }
    return dp[m];
}

int main() {
    // Speed up input/output operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // A board of size 0x0 or negative (though n >= 1) has 1 way to place 0 bishops.
    if (n == 0) {
        if (k == 0) cout << 1 << endl;
        else cout << 0 << endl;
        return 0;
    }

    // Maps to store the frequency of each diagonal (r-c) for each color.
    map<int, int> count1, count2;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            // Group squares based on their (r+c) parity.
            if ((r + c) % 2 == 0) {
                count1[r - c]++;
            } else {
                count2[r - c]++;
            }
        }
    }

    // L1 and L2 contain the lengths of the diagonals (rows in the rotated board).
    vector<int> L1, L2;
    for (auto const& [val, freq] : count1) L1.push_back(freq);
    for (auto const& [val, freq] : count2) L2.push_back(freq);

    // Sorting the lengths is crucial for the rook-placement DP to work properly.
    sort(L1.begin(), L1.end());
    sort(L2.begin(), L2.end());

    // Compute the number of ways to place i bishops for each color.
    vector<long long> dp1 = solve(L1, k);
    vector<long long> dp2 = solve(L2, k);

    // Combine the results using the principle that colors are independent.
    // Total ways = sum (ways(color1, i) * ways(color2, k-i)) for i = 0 to k.
    long long total_ways = 0;
    for (int i = 0; i <= k; ++i) {
        // Check bounds to prevent unnecessary work or error.
        if (i < (int)dp1.size() && (k - i) < (int)dp2.size() && (k - i) >= 0) {
            total_ways += dp1[i] * dp2[k - i];
        }
    }

    cout << total_ways << endl;

    return 0;
}

