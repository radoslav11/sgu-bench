#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

/**
 * Problem: Big Bishops
 * The problem asks for the number of ways to place k non-attacking bishops on an n x n board.
 * 
 * Key insights:
 * 1. Bishops on white squares and bishops on black squares are independent.
 *    A bishop on a white square (r+c is even) can only attack other bishops on white squares.
 *    A bishop on a black square (r+c is odd) can only attack other bishops on black squares.
 * 2. The problem of placing non-attacking bishops can be transformed into the problem of
 *    placing non-attacking rooks on a transformed board where the "rows" are the diagonals
 *    in one direction and the "columns" are the diagonals in the other direction.
 * 3. We separate the squares into two sets (white and black) and find the number of ways 
 *    to place i bishops on one set and k-i on the other, for all valid i.
 * 4. The diagonal lengths can be found by counting how many squares share the same r-c value 
 *    for each color.
 * 5. For a board with row lengths L1, L2, ..., Lm (sorted such that L1 <= L2 <= ... <= Lm),
 *    the number of ways to place j non-attacking rooks can be found using dynamic programming:
 *    dp[i][j] = dp[i-1][j] + dp[i-1][j-1] * (Li - (j-1))
 * 
 * Constraints: n <= 50, k <= n^2.
 * Complexity: O(n^2), which is well within the time and memory limits.
 */

// Using __int128 to handle potentially very large results since no modulo is given.
typedef __int128_t int128;

// Custom function to print __int128 as it is not natively supported by std::cout.
void print128(int128 n) {
    if (n == 0) {
        cout << "0" << endl;
        return;
    }
    string s = "";
    while (n > 0) {
        s += (char)('0' + (long long)(n % 10));
        n /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s << endl;
}

/**
 * get_ways calculates the number of ways to place j non-attacking rooks
 * on a board whose row lengths are given in L.
 * L is a vector of the lengths of the diagonals in one direction for a set of squares.
 */
vector<int128> get_ways(vector<int> L) {
    int m = L.size();
    sort(L.begin(), L.end());
    
    // dp[i][j] = number of ways to place j non-attacking rooks in the first i rows.
    vector<vector<int128>> dp(m + 1, vector<int128>(m + 1, 0));
    dp[0][0] = 1;
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 0; j <= i; ++j) {
            // Case 1: No rook is placed in the i-th row.
            dp[i][j] = dp[i - 1][j];
            
            // Case 2: One rook is placed in the i-th row.
            if (j > 0) {
                // The i-th row has L[i-1] columns. 
                // (j-1) columns have already been occupied by rooks in previous rows.
                long long available_cols = (long long)L[i - 1] - (j - 1);
                if (available_cols > 0) {
                    dp[i][j] += dp[i - 1][j - 1] * (int128)available_cols;
                }
            }
        }
    }
    
    // The results are the number of ways for each possible number of rooks j.
    vector<int128> ways(m + 1);
    for (int j = 0; j <= m; ++j) {
        ways[j] = dp[m][j];
    }
    return ways;
}

int main() {
    // Faster input/output.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // Handle trivial case: if k is greater than the total squares, no placement is possible.
    if (k > n * n) {
        cout << 0 << endl;
        return 0;
    }

    // We split the board into white squares and black squares.
    // Squares (r, c) where (r+c) is even are white.
    // Squares (r, c) where (r+c) is odd are black.
    // Within each set, bishops attack along diagonals (r-c constant).
    // We group squares by their r-c values to find the length of each "row" in the rook problem.
    map<int, int> white_diag_counts, black_diag_counts;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if ((r + c) % 2 == 0) {
                white_diag_counts[r - c]++;
            } else {
                black_diag_counts[r - c]++;
            }
        }
    }

    // Convert map counts into vectors of row lengths for the DP.
    vector<int> L_white, L_black;
    for (auto const& pair : white_diag_counts) {
        L_white.push_back(pair.second);
    }
    for (auto const& pair : black_diag_counts) {
        L_black.push_back(pair.second);
    }

    // Precalculate the number of ways to place i rooks on white and j rooks on black.
    vector<int128> ways_white = get_ways(L_white);
    vector<int128> ways_black = get_ways(L_black);

    int128 total_ways = 0;
    // The total number of ways to place exactly k bishops is the sum of
    // (ways to place i on white) * (ways to place k-i on black).
    for (int i = 0; i <= k; ++i) {
        int j = k - i;
        if (i < (int)ways_white.size() && j < (int)ways_black.size()) {
            total_ways += ways_white[i] * ways_black[j];
        }
    }

    print128(total_ways);

    return 0;
}

