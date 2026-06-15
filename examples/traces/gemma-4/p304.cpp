#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Mars Stomatology
 * The goal is to maximize the number of cured teeth within a given budget P.
 * Each tooth i has a curing cost Ai and is on a gum Ci.
 * If any tooth on gum j is cured, there is an additional anesthesia cost Bj for that gum.
 * 
 * Approach:
 * 1. Group teeth by their gum.
 * 2. For each gum, sort teeth by their curing cost Ai in ascending order.
 * 3. Precompute the cost of curing k teeth on gum j:
 *    S[j][0] = 0
 *    S[j][k] = Bj + (sum of k smallest Ai for teeth on gum j) for k > 0.
 * 4. Use dynamic programming where dp[j][s] is the minimum cost to cure exactly s teeth using gums 1..j.
 *    dp[j][s] = min_{0 <= k <= min(s, size(gum j))} { dp[j-1][s-k] + S[j][k] }
 * 5. The state space is K * N, and the transition is O(N), making the overall complexity O(N^2).
 *    With N, K <= 600, N^2 = 360,000, which easily fits in 0.5s.
 * 6. Reconstruct the solution using the 'choice' array stored during DP.
 */

const int INF = 1000000000;

struct Tooth {
    int cost;
    int id;
};

// Comparator to sort teeth on the same gum by their curing cost.
bool compareTeeth(const Tooth& a, const Tooth& b) {
    return a.cost < b.cost;
}

// Using static arrays for efficiency and to stay within memory limits.
int dp[601][601];
int choice[601][601];
int S[601][601];

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K, P;
    if (!(cin >> N >> K >> P)) return 0;

    // Read anesthesia costs for each gum
    vector<int> B(K + 1);
    for (int i = 1; i <= K; ++i) {
        cin >> B[i];
    }

    // Read tooth descriptions and group them by gum
    vector<vector<Tooth>> teeth_on_gum(K + 1);
    for (int i = 1; i <= N; ++i) {
        int a, c;
        cin >> a >> c;
        teeth_on_gum[c].push_back({a, i});
    }

    // Sort teeth on each gum by their cost
    for (int j = 1; j <= K; ++j) {
        sort(teeth_on_gum[j].begin(), teeth_on_gum[j].end(), compareTeeth);
    }

    // Precompute S[j][k]: the cost to cure k teeth on gum j
    for (int j = 1; j <= K; ++j) {
        for (int k = 0; k <= N; ++k) S[j][k] = INF;
        S[j][0] = 0;
        int current_sum = 0;
        for (int k = 1; k <= (int)teeth_on_gum[j].size(); ++k) {
            current_sum += teeth_on_gum[j][k - 1].cost;
            S[j][k] = B[j] + current_sum;
        }
    }

    // Initialize DP table
    for (int s = 0; s <= N; ++s) {
        dp[0][s] = INF;
    }
    dp[0][0] = 0;

    // Fill DP table
    for (int j = 1; j <= K; ++j) {
        int num_teeth_on_gum = teeth_on_gum[j].size();
        for (int s = 0; s <= N; ++s) {
            dp[j][s] = INF;
            choice[j][s] = 0;
            int max_k = min(s, num_teeth_on_gum);
            for (int k = 0; k <= max_k; ++k) {
                if (dp[j - 1][s - k] != INF) {
                    int current_cost = dp[j - 1][s - k] + S[j][k];
                    if (current_cost < dp[j][s]) {
                        dp[j][s] = current_cost;
                        choice[j][s] = k;
                    }
                }
            }
        }
    }

    // Find the maximum number of teeth S such that the cost is within P
    int max_s = 0;
    for (int s = N; s >= 0; --s) {
        if (dp[K][s] <= P) {
            max_s = s;
            break;
        }
    }

    // Output the results
    cout << max_s << "\n";
    vector<int> result_teeth;
    int current_s = max_s;
    // Backtrack to find which teeth were cured
    for (int j = K; j >= 1; --j) {
        int k = choice[j][current_s];
        for (int m = 0; m < k; ++m) {
            result_teeth.push_back(teeth_on_gum[j][m].id);
        }
        current_s -= k;
    }

    for (int i = 0; i < (int)result_teeth.size(); ++i) {
        cout << result_teeth[i] << (i == (int)result_teeth.size() - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}

