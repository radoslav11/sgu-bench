#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Problem: Integer Linear Programming
 * Minimize sum(x_i) subject to sum(c_i * x_i) = V, where x_i >= 0 are integers.
 * Given: N <= 3, c[i] <= 10^6, V <= 10^6.
 * 
 * Approach:
 * This is equivalent to the unbounded knapsack problem where each "item" c_i
 * has a "weight" of c_i and a "value" of 1. We want to find the minimum total 
 * value (sum of x_i) to reach exactly weight V.
 * 
 * Since V <= 10^6 and N <= 3, the complexity of a Dynamic Programming 
 * approach is O(N * V). 
 * For V = 10^6 and N = 3, N * V = 3 * 10^6, which easily fits within the 
 * 0.25s time limit in C++.
 * 
 * Space Complexity: O(V), which is 10^6 * 4 bytes (approx. 4 MB),
 * well within the 64 MB memory limit.
 */

using namespace std;

int main() {
    // Set up fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<int> c(N);
    for (int i = 0; i < N; ++i) {
        cin >> c[i];
    }

    int V;
    if (!(cin >> V)) return 0;

    // dp[v] will store the minimum number of x_i needed to make the sum exactly v.
    // Initialize with a value representing infinity.
    // Since V is at most 1,000,000, a value of 1,000,001 is enough to represent infinity.
    // Using a larger value like 1,000,000,007 is also safe.
    const int INF = 1000000007;
    vector<int> dp(V + 1, INF);
    
    // Base case: 0 items are needed to reach a sum of 0.
    dp[0] = 0;

    // Perform the unbounded knapsack DP.
    for (int i = 0; i < N; ++i) {
        int weight = c[i];
        // Optimization: skip if weight is larger than target V
        if (weight > V) continue;
        
        for (int v = weight; v <= V; ++v) {
            // If the state (v - weight) is reachable, update dp[v].
            if (dp[v - weight] != INF) {
                if (dp[v - weight] + 1 < dp[v]) {
                    dp[v] = dp[v - weight] + 1;
                }
            }
        }
    }

    // Output the result.
    if (dp[V] >= INF) {
        cout << -1 << endl;
    } else {
        cout << dp[V] << endl;
    }

    return 0;
}

