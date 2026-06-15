#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem: 479. Funny Feature
 * 
 * The problem asks for an ordering of n x m cell plantings such that the final
 * pumpkin count in each cell (i, j) is a_ij = 1 + (number of neighbors of (i, j) 
 * planted after (i, j)).
 * 
 * Let S be the set of currently unvisited cells. A cell (i, j) can be the last
 * cell in the current sequence of unvisited cells if its target pumpkin count
 * a_ij is exactly 1 (meaning no neighbor is planted after it among the remaining cells).
 * 
 * Algorithm:
 * 1. Maintain the current required pumpkin count a_ij for each cell.
 * 2. Use a queue to find all cells (i, j) that currently have a_ij = 1.
 * 3. While the queue is not empty:
 *    a. Pop a cell (r, c).
 *    b. If (r, c) is already visited or a_rc is no longer 1, skip it.
 *    c. Mark (r, c) as visited and add it to a result list (this list will
 *       contain the cells in reverse order).
 *    d. For each unvisited neighbor (nr, nc) of (r, c), decrement a_nr,nc.
 *       If a_nr,nc becomes 1, add (nr, nc) to the queue.
 * 4. If the result list contains all n * m cells, the solution exists.
 *    The sequence is the reverse of the result list.
 * 5. Otherwise, no solution exists.
 * 
 * Complexity: O(N*M) time and space, where N and M are the grid dimensions.
 */

int n, m;
int a[205][205];
bool visited[205][205];
int dr[] = {0, 0, 1, -1};
int dc[] = {1, -1, 0, 0};

int main() {
    // Faster I/O for competitive programming
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read grid dimensions
    if (!(cin >> n >> m)) return 0;

    // Read the target pumpkin counts
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cin >> a[i][j];
        }
    }

    // Queue for cells that can be the "last" in the current set of unvisited cells
    queue<pair<int, int>> q;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i][j] == 1) {
                q.push({i, j});
            }
        }
    }

    vector<pair<int, int>> result;
    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

        int r = curr.first;
        int c = curr.second;

        // If cell is already processed or its count is no longer 1, skip it.
        // The check a[r][c] != 1 handles cells that were initially 1 but 
        // were decremented when one of their neighbors was processed.
        if (visited[r][c] || a[r][c] != 1) continue;

        visited[r][c] = true;
        result.push_back({r, c});

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // If neighbor is valid and not yet part of the sequence
            if (nr >= 1 && nr <= n && nc >= 1 && nc <= m && !visited[nr][nc]) {
                // When we process (r, c) as the 'last' of the remaining,
                // we are removing one neighbor that was planted after its neighbors.
                a[nr][nc]--;
                if (a[nr][nc] == 1) {
                    q.push({nr, nc});
                }
            }
        }
    }

    // Check if we found a sequence containing all cells
    if (result.size() == (size_t)(n * m)) {
        // The result was built in reverse order (from last to first), 
        // so print it in reverse.
        for (int i = (int)result.size() - 1; i >= 0; --i) {
            cout << result[i].first << " " << result[i].second << "\n";
        }
    } else {
        cout << "No solution" << endl;
    }

    return 0;
}

