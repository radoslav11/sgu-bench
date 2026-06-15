#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: 518. Kidnapping
 * 
 * We are given a graph representing intersections and bidirectional roads with specific lengths.
 * The agent knows the length of each of the k roads taken during a trip that starts at intersection 1.
 * The kidnappers might visit the same intersection or the same road multiple times.
 * Our task is to find all possible intersections where the coach could have ended up after the k roads.
 * 
 * Approach: Dynamic Programming
 * Let dp[i][v] be a boolean indicating whether it's possible to be at intersection v after i roads.
 * Transition: dp[i][v] = true if there exists a u such that dp[i-1][u] is true and the road (u, v) has length r[i].
 * Since n and k are at most 200, the complexity will be O(k * n^2), which is roughly 8 * 10^6 operations.
 * This will easily fit within the 0.5s time limit.
 */

struct Edge {
    int to;
    int length;
};

int main() {
    // Faster I/O
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // Using an adjacency list to optimize iterating over roads.
    // The input is an adjacency matrix where l_ij is the length of the road between i and j.
    vector<vector<Edge>> adj_list(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            int len;
            cin >> len;
            if (len > 0) {
                adj_list[i].push_back({j, len});
            }
        }
    }

    int k;
    if (!(cin >> k)) return 0;
    vector<int> r(k);
    for (int i = 0; i < k; ++i) {
        cin >> r[i];
    }

    // dp[u] stores whether intersection u is reachable after the current step.
    // Using vector<char> to avoid the bit-packing behavior of vector<bool>.
    vector<char> dp(n + 1, 0);
    dp[1] = 1;

    // Pre-calculate a list of reachable nodes to optimize the DP transition.
    vector<int> current_reachable;
    current_reachable.push_back(1);

    for (int i = 0; i < k; ++i) {
        vector<char> next_dp(n + 1, 0);
        int current_road_len = r[i];
        
        // For each currently reachable node, check its neighbors for matching road lengths.
        for (int u : current_reachable) {
            for (const auto& edge : adj_list[u]) {
                if (edge.length == current_road_len) {
                    next_dp[edge.to] = 1;
                }
            }
        }
        
        // Update the current state and rebuild the current_reachable list for the next step.
        dp = move(next_dp);
        current_reachable.clear();
        for (int u = 1; u <= n; ++u) {
            if (dp[u]) {
                current_reachable.push_back(u);
            }
        }
    }

    // Collect all nodes that are reachable after exactly k steps.
    vector<int> results;
    for (int v = 1; v <= n; ++v) {
        if (dp[v]) {
            results.push_back(v);
        }
    }

    // Output the results in the required format.
    if (results.empty()) {
        cout << 0 << endl;
    } else {
        cout << results.size() << endl;
        for (int i = 0; i < (int)results.size(); ++i) {
            cout << results[i] << (i == (int)results.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

