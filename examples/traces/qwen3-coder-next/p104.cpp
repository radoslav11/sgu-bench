#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int F, V;
    cin >> F >> V;

    vector<vector<int>> A(F + 1, vector<int>(V + 1));
    for (int i = 1; i <= F; ++i) {
        for (int j = 1; j <= V; ++j) {
            cin >> A[i][j];
        }
    }

    // dp[i][j] = maximum aesthetic sum for placing first i flowers in vases 1..j
    // with the i-th flower placed in vase j (or -INF if impossible)
    vector<vector<int>> dp(F + 1, vector<int>(V + 1, INT_MIN));
    // parent[i][j] = the vase used for flower i-1 (for reconstruction)
    vector<vector<int>> parent(F + 1, vector<int>(V + 1, -1));

    // Base case: place flower 1 in vase j
    for (int j = 1; j <= V; ++j) {
        dp[1][j] = A[1][j];
    }

    // Fill DP table
    for (int i = 2; i <= F; ++i) {
        // For flower i, we need at least i vases (since each flower needs its own vase and order must be preserved)
        // So j starts from i
        for (int j = i; j <= V - (F - i); ++j) {
            // Find the best previous vase for flower i-1 (must be < j)
            int best_prev = INT_MIN;
            int best_k = -1;
            for (int k = i - 1; k < j; ++k) {
                if (dp[i-1][k] > best_prev) {
                    best_prev = dp[i-1][k];
                    best_k = k;
                }
            }
            if (best_prev != INT_MIN) {
                dp[i][j] = best_prev + A[i][j];
                parent[i][j] = best_k;
            }
        }
    }

    // Find the maximum sum and the ending vase for all F flowers
    int max_sum = INT_MIN;
    int last_vase = -1;
    for (int j = F; j <= V; ++j) {
        if (dp[F][j] > max_sum) {
            max_sum = dp[F][j];
            last_vase = j;
        }
    }

    // Reconstruct the arrangement
    vector<int> result(F + 1);
    int current_vase = last_vase;
    for (int i = F; i >= 1; --i) {
        result[i] = current_vase;
        current_vase = parent[i][current_vase];
    }

    // Output
    cout << max_sum << "\n";
    for (int i = 1; i <= F; ++i) {
        if (i > 1) cout << " ";
        cout << result[i];
    }
    cout << "\n";

    return 0;
}
