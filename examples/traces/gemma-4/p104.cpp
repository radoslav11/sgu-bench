#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Little shop of flowers
 * We need to place F flower bunches into V vases such that:
 * 1. Each bunch is placed in exactly one vase.
 * 2. The bunches are placed in increasing order of their IDs (bunch i in vase j_i, 
 *    bunch i+1 in vase j_{i+1}, such that j_i < j_{i+1}).
 * 3. The total aesthetic value is maximized.
 * 
 * Constraints:
 * F, V <= 100
 * Aij between -50 and 50.
 * 
 * Approach:
 * This is a dynamic programming problem.
 * Let dp[i][j] be the maximum aesthetic sum of placing the first i bunches, 
 * with the i-th bunch placed in the j-th vase.
 * 
 * DP Recurrence:
 * For i = 1:
 *   dp[1][j] = A[1][j] for j = 1 to V
 * For i > 1:
 *   dp[i][j] = A[i][j] + max(dp[i-1][k] for 1 <= k < j)
 * 
 * Time Complexity: O(F * V^2), which is approximately 100^3 = 1,000,000 operations.
 * This is well within the 0.25s time limit.
 * Space Complexity: O(F * V) for the dp and parent arrays, which is about 120 KB.
 */

const int INF = 1e9;

int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int F, V;
    if (!(cin >> F >> V)) return 0;

    // A[i][j] is the aesthetic value of putting bunch i in vase j
    // Using 1-based indexing for both i and j to match problem statement
    vector<vector<int>> A(F + 1, vector<int>(V + 1));
    for (int i = 1; i <= F; ++i) {
        for (int j = 1; j <= V; ++j) {
            cin >> A[i][j];
        }
    }

    // dp[i][j] stores the max sum for the first i flowers where flower i is in vase j
    vector<vector<int>> dp(F + 1, vector<int>(V + 1, -INF));
    // parent[i][j] stores the vase index of the (i-1)-th flower that yielded the max dp[i][j]
    vector<vector<int>> parent(F + 1, vector<int>(V + 1, 0));

    // Base case: first flower can be in any vase from 1 to V
    for (int j = 1; j <= V; ++j) {
        dp[1][j] = A[1][j];
    }

    // Fill the DP table
    for (int i = 2; i <= F; ++i) {
        for (int j = 1; j <= V; ++j) {
            for (int k = 1; k < j; ++k) {
                if (dp[i - 1][k] != -INF) {
                    int current_sum = dp[i - 1][k] + A[i][j];
                    if (current_sum > dp[i][j]) {
                        dp[i][j] = current_sum;
                        parent[i][j] = k;
                    }
                }
            }
        }
    }

    // Find the maximum sum after placing all F flowers
    int max_sum = -INF;
    int last_vase = -1;
    for (int j = 1; j <= V; ++j) {
        if (dp[F][j] > max_sum) {
            max_sum = dp[F][j];
            last_vase = j;
        }
    }

    // If no valid arrangement is found (shouldn't happen per constraints)
    if (last_vase == -1) {
        return 0;
    }

    // Backtrack to find the vase indices for each flower
    vector<int> result(F + 1);
    int curr = last_vase;
    for (int i = F; i >= 1; --i) {
        result[i] = curr;
        curr = parent[i][curr];
    }

    // Output the result
    cout << max_sum << "\n";
    for (int i = 1; i <= F; ++i) {
        cout << result[i] << (i == F ? "" : " ");
    }
    cout << endl;

    return 0;
}

