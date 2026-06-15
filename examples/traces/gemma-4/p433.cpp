#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

/**
 * Problem: 433. Japhshan and Ramshut
 * The goal is to tile an N x M rectangle using bricks of given lengths.
 * A brick of length L can be 1 x L or L x 1.
 * Constraints: N + M <= 20, K <= 5, brick length <= 1000.
 * Max cells in a grid: 10 x 10 = 100.
 * 
 * Approach:
 * 1. Use backtracking (DFS) to find a valid tiling of the N x M grid.
 * 2. Each brick used in the tiling is assigned a unique ID.
 * 3. To satisfy the condition "two adjacent letters should be the same if 
 *    they are related to the same brick, and different otherwise", we treat 
 *    each brick as a node in a graph. Two nodes are connected if their 
 *    corresponding bricks are adjacent in the grid.
 * 4. Solve this graph coloring problem with lowercase Latin letters.
 */

int N, M, K;
int lengths[5];
int grid[20][20];
int brick_count = 0;
set<int> brick_adj[101];
char brick_color[101];

// Backtracking to tile the grid
bool solve(int r, int c) {
    if (r == N) return true;
    
    int nr = (c == M - 1) ? r + 1 : r;
    int nc = (c == M - 1) ? 0 : c + 1;
    
    // If the current cell is already filled by a previously placed brick, skip to the next cell.
    if (grid[r][c] != -1) return solve(nr, nc);

    // Try each brick type for the current empty cell.
    for (int k = 0; k < K; ++k) {
        int L = lengths[k];
        
        // Try horizontal placement: (r, c) to (r, c + L - 1)
        if (c + L <= M) {
            bool ok = true;
            for (int i = 0; i < L; ++i) {
                if (grid[r][c + i] != -1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                int id = brick_count++;
                for (int i = 0; i < L; ++i) {
                    grid[r][c + i] = id;
                }
                if (solve(nr, nc)) return true;
                // Backtrack
                for (int i = 0; i < L; ++i) grid[r][c + i] = -1;
                brick_count--;
            }
        }
        
        // Try vertical placement: (r, c) to (r + L - 1, c)
        // If L=1, the horizontal placement already covers this case.
        if (L > 1 && r + L <= N) {
            bool ok = true;
            for (int i = 0; i < L; ++i) {
                if (grid[r + i][c] != -1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                int id = brick_count++;
                for (int i = 0; i < L; ++i) {
                    grid[r + i][c] = id;
                }
                if (solve(nr, nc)) return true;
                // Backtrack
                for (int i = 0; i < L; ++i) grid[r + i][c] = -1;
                brick_count--;
            }
        }
    }
    return false;
}

// Backtracking to color the bricks such that adjacent bricks have different colors
bool color_dfs(int id) {
    if (id == brick_count) return true;
    
    for (char col = 'a'; col <= 'z'; ++col) {
        bool ok = true;
        for (int neighbor : brick_adj[id]) {
            if (neighbor < id && brick_color[neighbor] == col) {
                ok = false;
                break;
            }
        }
        if (ok) {
            brick_color[id] = col;
            if (color_dfs(id + 1)) return true;
        }
    }
    return false;
}

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M >> K)) return 0;
    for (int i = 0; i < K; ++i) cin >> lengths[i];
    
    // Initialize grid with -1 indicating empty cells
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            grid[i][j] = -1;
        }
    }

    // Try to find a tiling
    if (solve(0, 0)) {
        // Build the adjacency graph of bricks
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < M; ++c) {
                int id1 = grid[r][c];
                // Check right and down to find neighboring bricks (avoid duplicate checks)
                int dr[] = {0, 1};
                int dc[] = {1, 0};
                for (int i = 0; i < 2; ++i) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if (nr < N && nc < M) {
                        int id2 = grid[nr][nc];
                        if (id1 != id2) {
                            brick_adj[id1].insert(id2);
                            brick_adj[id2].insert(id1);
                        }
                    }
                }
            }
        }

        // Color the bricks graph
        if (color_dfs(0)) {
            cout << "YES\n";
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < M; ++c) {
                    cout << brick_color[grid[r][c]];
                }
                cout << "\n";
            }
        } else {
            // Theoretically, for a planar graph (like bricks in a grid), 
            // 4 colors are always enough.
            cout << "NO\n";
        }
    } else {
        cout << "NO\n";
    }

    return 0;
}

