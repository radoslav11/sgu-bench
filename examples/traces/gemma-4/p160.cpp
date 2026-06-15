#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Problem: Magic Multiplying Machine (MMM)
 * We have N levers, each with a number a[i] (1 <= a[i] <= M).
 * We need to find a subset of levers such that the product of their numbers modulo M is maximized.
 * N <= 10000, M <= 1000.
 * 
 * DP Approach:
 * dp[i][v] = whether product 'v' modulo M is reachable using the first i levers.
 * To reconstruct the subset, we store parent information:
 * - parent_v[i][v] = u (0 <= u < M) if the i-th lever was used to reach value v, where (u * a[i]) % M = v.
 * - parent_v[i][v] = u + M (0 <= u < M) if the i-th lever was NOT used to reach value v, where u = v.
 * - parent_v[i][v] = -1 if the value v is not reachable with the first i levers.
 *
 * Time Complexity: O(N * M)
 * Space Complexity: O(N * M) using short to fit within the 64MB memory limit.
 */

short parent_v[10001][1000];
int a[10001];

int main() {
    // Optimize standard I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    for (int i = 1; i <= N; ++i) {
        cin >> a[i];
    }

    // Initialize DP table
    for (int v = 0; v < M; ++v) {
        parent_v[0][v] = -1;
    }

    // Base case: with zero levers, the score is 1 mod M.
    // (Since M >= 2, 1 % M is always 1)
    int start_v = 1 % M;
    parent_v[0][start_v] = (short)(start_v + M);

    for (int i = 1; i <= N; ++i) {
        // Case 1: The i-th lever is not used to reach value v.
        // We inherit the reachability from the (i-1)-th step.
        for (int v = 0; v < M; ++v) {
            if (parent_v[i - 1][v] != -1) {
                // parent_v[i][v] = v + M means v is reachable without using a[i]
                parent_v[i][v] = (short)(v + M);
            } else {
                parent_v[i][v] = -1;
            }
        }

        // Case 2: The i-th lever is used to reach value next_v.
        for (int v = 0; v < M; ++v) {
            if (parent_v[i - 1][v] != -1) {
                // Extract the previous value u
                int u;
                if (parent_v[i - 1][v] < M) {
                    u = parent_v[i - 1][v];
                } else {
                    u = parent_v[i - 1][v] - M;
                }

                // Calculate product modulo M
                int next_v = (1LL * u * a[i]) % M;
                // If next_v was not already reachable (we prefer not using a[i] as per Case 1), 
                // then mark it as reachable using a[i].
                if (parent_v[i][next_v] == -1) {
                    parent_v[i][next_v] = (short)u;
                }
            }
        }
    }

    // Find the maximum score reachable with N levers.
    int max_score = 0;
    for (int v = M - 1; v >= 0; --v) {
        if (parent_v[N][v] != -1) {
            max_score = v;
            break;
        }
    }

    // Reconstruct the indices of the levers used to achieve the maximum score.
    vector<int> indices;
    int cur_v = max_score;
    for (int i = N; i >= 1; --i) {
        if (parent_v[i][cur_v] < M) {
            // The i-th lever was used to reach cur_v
            indices.push_back(i);
            cur_v = (int)parent_v[i][cur_v];
        } else {
            // The i-th lever was not used
            cur_v = (int)parent_v[i][cur_v] - M;
        }
    }

    // Sort indices in ascending order for output.
    sort(indices.begin(), indices.end());

    // Output the result
    cout << max_score << "\n";
    for (int i = 0; i < indices.size(); ++i) {
        cout << indices[i] << (i == indices.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

