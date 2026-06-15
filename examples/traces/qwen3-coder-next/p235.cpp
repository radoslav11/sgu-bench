#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<string> board(N);
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }

    // Find the initial position of the Queen
    int qx = -1, qy = -1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'Q') {
                qx = i;
                qy = j;
                break;
            }
        }
        if (qx != -1) break;
    }

    // Directions: 8 possible directions for Queen movement
    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // We'll use BFS with state (row, col, moves_done)
    // But since M is up to 50 and board size is up to 300x300,
    // we can use DP or BFS level by level.

    // visited[r][c] = the minimum number of moves to reach (r,c)
    // But we need to know if we can reach (r,c) in exactly M moves
    // So we track all positions reachable in exactly k moves for k=0..M

    // Use two 2D boolean arrays for current and next levels
    vector<vector<bool>> current(N, vector<bool>(N, false));
    vector<vector<bool>> next(N, vector<bool>(N, false));

    // Initialize: at move 0, queen is at starting position
    current[qx][qy] = true;

    // If M == 0, answer is 1
    for (int move = 1; move <= M; move++) {
        // Reset next level
        for (int i = 0; i < N; i++) {
            fill(next[i].begin(), next[i].end(), false);
        }

        // For each cell in current level, try all 8 directions
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (!current[r][c]) continue;

                // Try each of 8 directions
                for (int d = 0; d < 8; d++) {
                    int nr = r + dx[d];
                    int nc = c + dy[d];

                    // Move in this direction until blocked
                    while (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                        // If it's a black piece ('B'), the queen can capture it and stop there
                        // If it's a white piece ('W'), the queen cannot move through or land on it
                        // If it's '.' or 'Q', but 'Q' shouldn't be in path
                        if (board[nr][nc] == 'B') {
                            // Queen can move here and capture
                            next[nr][nc] = true;
                            break;
                        } else if (board[nr][nc] == 'W') {
                            // Cannot move through or land on a white piece
                            break;
                        }
                        // If '.' or 'Q' (though 'Q' shouldn't be here), can move here and continue
                        next[nr][nc] = true;
                        nr += dx[d];
                        nc += dy[d];
                    }
                }
            }
        }

        // Swap for next iteration
        current.swap(next);
    }

    // Count positions reachable in exactly M moves
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (current[i][j]) {
                count++;
            }
        }
    }

    cout << count << endl;

    return 0;
}
