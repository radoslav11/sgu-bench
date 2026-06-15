#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * Two companies are hiring programmers from a group of n programmers with m friendship pairs.
 * The first programmer of each company can be any available programmer.
 * Subsequent programmers must be friends with someone already in the company.
 * There are three geniuses (programmers 1, 2, 3).
 * Each company wants to maximize the number of geniuses it hires.
 * We need to find which company will hire at least two geniuses, assuming optimal play.
 * 
 * Mathematical Modeling:
 * This is a game on a graph where each company expands its connected component.
 * Company 1 (C1) wins if it can choose a starting node v1 such that for any node v2
 * chosen by Company 2 (C2), C1 reaches at least two geniuses.
 * C1 reaches genius g_i before C2 if d(v1, g_i) <= d(v2, g_i).
 * C2 reaches genius g_i before C1 if d(v2, g_i) < d(v1, g_i).
 * 
 * Thus, C1 wins if there exists v1 such that for all v2, at most one i in {1, 2, 3} 
 * satisfies d(v2, g_i) < d(v1, g_i).
 * 
 * Complexity:
 * 1. BFS from each of the 3 geniuses to compute d_1, d_2, d_3: O(N + M)
 * 2. For each pair of geniuses (i, j), precompute:
 *    best_d_i(d) = min { d_i(v) | d_j(v) < d }
 *    This can be done in O(N).
 * 3. Check if there is any v1 such that for all v2, no two d_i(v2) < d_i(v1).
 *    This is equivalent to checking if any v1 is "not dominated" in at least two 
 *    dimensions by some v2.
 *    A node v2 dominates v1 if d_i(v2) < d_i(v1) and d_j(v2) < d_j(v1) for some i != j.
 * 
 * Total time complexity: O(N + M).
 */

const int INF = 1e9;

void bfs(int start, int n, const vector<vector<int>>& adj, vector<int>& dist) {
    dist.assign(n + 1, INF);
    dist[start] = 0;
    queue<int> q;
    q.push(start);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> d1, d2, d3;
    bfs(1, n, adj, d1);
    bfs(2, n, adj, d2);
    bfs(3, n, adj, d3);

    // best_d_i_j[i][j][d] will store min { d_i(v) | d_j(v) < d }
    // Here we have 3 geniuses, so pairs are (1,2), (1,3), (2,3)
    // We only need 3 such tables.
    vector<int> b12(n + 2, INF), b13(n + 2, INF), b23(n + 2, INF);
    vector<int> min_d1_d2(n + 2, INF), min_d1_d3(n + 2, INF), min_d2_d3(n + 2, INF);

    // Using n+1 as a safe upper bound for distances in BFS
    for (int i = 1; i <= n; ++i) {
        if (d2[i] <= n) min_d1_d2[d2[i]] = min(min_d1_d2[d2[i]], d1[i]);
        if (d3[i] <= n) min_d1_d3[d3[i]] = min(min_d1_d3[d3[i]], d1[i]);
        if (d3[i] <= n) min_d2_d3[d3[i]] = min(min_d2_d3[d3[i]], d2[i]);
    }

    int current_min = INF;
    for (int d = 0; d <= n; ++d) {
        b12[d] = current_min;
        current_min = min(current_min, min_d1_d2[d]);
    }
    current_min = INF;
    for (int d = 0; d <= n; ++d) {
        b13[d] = current_min;
        current_min = min(current_min, min_d1_d3[d]);
    }
    current_min = INF;
    for (int d = 0; d <= n; ++d) {
        b23[d] = current_min;
        current_min = min(current_min, min_d2_d3[d]);
    }

    bool company1_wins = false;
    for (int i = 1; i <= n; ++i) {
        bool dominated = false;
        // Check if any v2 dominates i in (1,2)
        if (d2[i] <= n && d1[i] > b12[d2[i]]) dominated = true;
        // Check if any v2 dominates i in (1,3)
        if (!dominated && d3[i] <= n && d1[i] > b13[d3[i]]) dominated = true;
        // Check if any v2 dominates i in (2,3)
        if (!dominated && d3[i] <= n && d2[i] > b23[d3[i]]) dominated = true;

        if (!dominated) {
            company1_wins = true;
            break;
        }
    }

    if (company1_wins) {
        cout << 1 << endl;
    } else {
        cout << 2 << endl;
    }

    return 0;
}

