#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

/**
 * Problem: Treediff
 * Goal: For each internal node in a rooted tree, find the minimum absolute difference
 * between any two leaf values in its subtree. If a subtree has only one leaf, output 2^31 - 1.
 * 
 * Approach: 
 * Use small-to-large merging of sets of leaf values. For each node, 
 * maintain a set of leaf values in its subtree. When merging child subtrees, 
 * merge the smaller sets into the largest set. During merging, update the 
 * minimum difference using the values being inserted and their neighbors in the larger set.
 * 
 * Complexity:
 * Time: O(n log^2 n), where n is the number of nodes.
 * Space: O(n).
 * Given the extremely tight 0.25s time limit, using std::set and efficient small-to-large 
 * merging is critical.
 */

const int MAXN = 200005;
const int INF = 2147483647;

// Tree structure and leaf properties
vector<int> adj[MAXN];
int leaf_val[MAXN];
int ans[MAXN];
bool is_leaf[MAXN];
// Sets to store leaf values for each subtree (small-to-large merging)
set<int> s[MAXN];

/**
 * DFS function to compute the minimum difference for each node.
 * @param u The current node being processed.
 */
void dfs(int u) {
    if (is_leaf[u]) {
        s[u].insert(leaf_val[u]);
        ans[u] = INF;
        return;
    }

    int largest_child = -1;
    ans[u] = INF;

    // First pass: process children and find the child with the largest set of leaves
    for (int v : adj[u]) {
        dfs(v);
        // The minimum difference for current node is at least the minimum 
        // difference within any child's subtree.
        if (ans[v] < ans[u]) {
            ans[u] = ans[v];
        }
        if (largest_child == -1 || s[v].size() > s[largest_child].size()) {
            largest_child = v;
        }
    }

    // Second pass: merge smaller child sets into the largest child's set (small-to-large)
    if (largest_child != -1) {
        // Swap current node's set with the largest child's set to reuse it.
        swap(s[u], s[largest_child]);

        for (int v : adj[u]) {
            if (v == largest_child) continue;
            for (int val : s[v]) {
                // Find the neighbors of the new value in the existing set to update min difference.
                auto it = s[u].lower_bound(val);
                if (it != s[u].end()) {
                    int diff = *it - val;
                    if (diff < ans[u]) ans[u] = diff;
                }
                if (it != s[u].begin()) {
                    int diff = val - *prev(it);
                    if (diff < ans[u]) ans[u] = diff;
                }
                s[u].insert(val);
            }
            // Free memory from the merged set.
            s[v].clear();
        }
    }
}

int main() {
    // Fast I/O to handle large input/output and the tight time limit.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Read parent nodes for nodes 2, 3, ..., n
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        adj[p].push_back(i);
    }

    // Reset leaf flags and read values for leaves (nodes n-m+1 to n)
    for (int i = 1; i <= n; i++) is_leaf[i] = false;
    for (int i = n - m + 1; i <= n; ++i) {
        cin >> leaf_val[i];
        is_leaf[i] = true;
    }

    // Start DFS from the root (node 1)
    dfs(1);

    // Output the results for internal nodes (1 to n-m)
    for (int i = 1; i <= n - m; ++i) {
        cout << ans[i] << (i == n - m ? "" : " ");
    }
    cout << "\n";

    return 0;
}

