#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    if (N == 0) return 0;

    // Arrays to store the tree structure and DP states
    // P[i] is the parent of node i
    // W[i] is the weight of the edge between i and P[i]
    vector<int> P(N + 1, 0);
    vector<long long> W(N + 1, 0);
    
    // Read the tree edges
    // The problem guarantees that the i-th computer (for i >= 2) is connected
    // to a previously settled computer, meaning P[i] < i.
    for (int i = 2; i <= N; ++i) {
        cin >> P[i] >> W[i];
    }

    // down[i] stores the longest path from i down into its subtree
    vector<long long> down(N + 1, 0);
    // down2[i] stores the second longest path from i down into a different branch of its subtree
    vector<long long> down2(N + 1, 0);
    // down_node[i] stores the child of i that gives the 'down[i]' maximum
    vector<int> down_node(N + 1, 0);
    // up[i] stores the longest path from i going up through its parent
    vector<long long> up(N + 1, 0);

    // Bottom-up pass to compute 'down' and 'down2'
    // Since P[i] < i, processing from N down to 2 guarantees we process children before parents
    for (int i = N; i >= 2; --i) {
        long long d = down[i] + W[i];
        if (d > down[P[i]]) {
            down2[P[i]] = down[P[i]];
            down[P[i]] = d;
            down_node[P[i]] = i;
        } else if (d > down2[P[i]]) {
            down2[P[i]] = d;
        }
    }

    // Top-down pass to compute 'up'
    // Since P[i] < i, processing from 2 to N guarantees we process parents before children
    for (int i = 2; i <= N; ++i) {
        if (down_node[P[i]] == i) {
            // If the longest path down from parent goes through this child,
            // we must use the second longest path down from the parent
            up[i] = W[i] + max(up[P[i]], down2[P[i]]);
        } else {
            // Otherwise, we can safely use the longest path down from the parent
            up[i] = W[i] + max(up[P[i]], down[P[i]]);
        }
    }

    // The maximum distance for any node is the max of its longest downward path and longest upward path
    for (int i = 1; i <= N; ++i) {
        cout << max(down[i], up[i]) << "\n";
    }

    return 0;
}
