#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Problem: 218. Unstable Systems
 * Goal: Minimize the maximum unsafety value in a perfect assignment of n programs to n computers.
 * Strategy: Binary search on the possible unsafety values and use bipartite matching (DFS-based)
 *           to check if a perfect matching exists for a given maximum unsafety value.
 * 
 * Complexity:
 * - Binary search: log(n^2) iterations, approx. 18 for n=500.
 * - Matching: DFS-based bipartite matching is O(V * E), where V is number of computers (n)
 *   and E is the number of allowed connections (at most n^2).
 * - Overall: O(log(n^2) * n^3) in the worst case. In practice, DFS matching is very efficient.
 */

int n;
int A[505][505];
int match_p[505]; // match_p[program] = computer
int match_c[505]; // match_c[computer] = program
int vis[505];     // vis[program] = timer (to avoid memset in each DFS)
int timer;
vector<int> adj[505]; // Adjacency list for current X to speed up DFS

// DFS-based bipartite matching to find an augmenting path
bool dfs(int u) {
    for (int v : adj[u]) {
        if (vis[v] != timer) {
            vis[v] = timer;
            if (match_p[v] == -1 || dfs(match_p[v])) {
                match_p[v] = u;
                match_c[u] = v;
                return true;
            }
        }
    }
    return false;
}

// Check if a perfect matching exists for a given threshold X
bool check(int X) {
    // Build adjacency list of edges (i, j) such that A[i][j] <= X
    for (int i = 0; i < n; i++) {
        adj[i].clear();
        for (int j = 0; j < n; j++) {
            if (A[i][j] <= X) {
                adj[i].push_back(j);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        match_p[i] = -1;
        match_c[i] = -1;
        vis[i] = 0;
    }
    timer = 0;
    int count = 0;

    // 1. Greedy matching to quickly find as many edges as possible
    for (int i = 0; i < n; i++) {
        for (int v : adj[i]) {
            if (match_p[v] == -1) {
                match_p[v] = i;
                match_c[i] = v;
                count++;
                break;
            }
        }
    }

    // 2. DFS for remaining unmatched computers to find augmenting paths
    for (int i = 0; i < n; i++) {
        if (match_c[i] == -1) {
            timer++;
            if (dfs(i)) {
                count++;
            } else {
                // If a computer cannot be matched, no perfect matching exists
                return false;
            }
        }
    }
    return count == n;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n)) return 0;

    vector<int> vals;
    vals.reserve(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
            vals.push_back(A[i][j]);
        }
    }

    // Sort and unique the possible answers for binary search
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    // Binary search over the sorted unique values
    int low = 0, high = (int)vals.size() - 1;
    int ans_val = vals.back();
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (check(vals[mid])) {
            ans_val = vals[mid];
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    // Re-run check for the optimal ans_val to finalize match_c array
    check(ans_val);

    // Output result
    cout << ans_val << "\n";
    for (int i = 0; i < n; i++) {
        // Output computer index and the program index assigned to it
        cout << (i + 1) << " " << (match_c[i] + 1) << "\n";
    }

    return 0;
}

