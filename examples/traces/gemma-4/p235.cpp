#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * Problem Analysis:
 * The White Queen can move along rows, columns, and diagonals.
 * The queen can land on any cell ('W', 'B', or '.') but cannot pass through any pieces ('W', 'B', or 'Q').
 * We need to find the number of different cells the queen can occupy after exactly M moves.
 * 
 * Constraints:
 * N: 2 <= N <= 300
 * M: 0 <= M <= 50
 * Time complexity should be around O(M * N^2) to be efficient.
 * 
 * DP Approach:
 * Let S_m be the set of cells reachable in exactly m moves.
 * S_0 = {initial position of the Queen}.
 * S_m = {cells reachable from some cell in S_{m-1} in one move}.
 * 
 * To compute S_m in O(N^2) time:
 * For each move m, we iterate over each cell (r, c) in S_{m-1} and expand in all 8 directions.
 * To optimize, we use a direction-specific visited array 'visited_dir[8][N][N]' to ensure that
 * we don't re-traverse the same direction path within the same move. This ensures each direction
 * for each cell is processed only once, making the complexity O(M * 8 * N^2).
 */

int N, M;
string board[300];
int visited[300][300];
int visited_dir[8][300][300];
int visit_token = 0;

// Directions: 8 standard chess queen directions
int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

int main() {
    // Speed up standard I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Input N and M
    if (!(cin >> N >> M)) return 0;

    int start_r = -1, start_c = -1;
    for (int i = 0; i < N; ++i) {
        cin >> board[i];
        for (int j = 0; j < N; ++j) {
            // Identify the initial position of the White Queen
            if (board[i][j] == 'Q') {
                start_r = i;
                start_c = j;
            }
        }
    }

    // current_cells will store the set of cells the queen can be in after m moves
    vector<pair<int, int>> current_cells;
    current_cells.push_back({start_r, start_c});

    // Perform M moves
    for (int m = 1; m <= M; ++m) {
        vector<pair<int, int>> next_cells;
        visit_token++; // Increment token for the current move's visited arrays

        for (auto& cell : current_cells) {
            int r = cell.first;
            int c = cell.second;
            for (int d = 0; d < 8; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                
                // Traverse the direction until the boundary or a piece is hit
                while (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    // If this cell was already reached by the same direction in the current move, skip it
                    if (visited_dir[d][nr][nc] == visit_token) break;
                    visited_dir[d][nr][nc] = visit_token;
                    
                    // If this cell hasn't been added to the set for the current move, add it
                    if (visited[nr][nc] != visit_token) {
                        visited[nr][nc] = visit_token;
                        next_cells.push_back({nr, nc});
                    }
                    
                    // If the queen hits a piece, she can land on it but cannot go further
                    if (board[nr][nc] != '.') break;
                    
                    nr += dr[d];
                    nc += dc[d];
                }
            }
        }
        // Move to the next set of cells for the next step
        current_cells = next_cells;
    }

    // The answer is the number of unique cells in the set after M moves
    cout << current_cells.size() << endl;

    return 0;
}

