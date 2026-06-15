#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

/**
 * Problem: Find a Hamiltonian path for a knight on an N x N chessboard.
 * N can be up to 250.
 * We use Warnsdorff's rule, which is an efficient heuristic for the Knight's Tour.
 * The rule selects the next cell by choosing the unvisited neighbor with the
 * fewest onward unvisited moves. To break ties, we choose the cell furthest
 * from the center of the board.
 */

int N;
int grid[255][255];
int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};

/**
 * countNeighbors: Calculates the number of unvisited valid knight move destinations
 * from cell (r, c).
 */
inline int countNeighbors(int r, int c) {
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int nr = r + dx[i];
        int nc = c + dy[i];
        if (nr >= 0 && nr < N && nc >= 0 && nc < N && grid[nr][nc] == 0) {
            count++;
        }
    }
    return count;
}

/**
 * solve: Tries to find a Hamiltonian path using Warnsdorff's rule, 
 * starting from (startR, startC).
 */
bool solve(int startR, int startC) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            grid[i][j] = 0;

    int r = startR, c = startC;
    grid[r][c] = 1;
    for (int step = 2; step <= N * N; ++step) {
        int minDeg = 9;
        pair<int, int> candidates[8];
        int candCount = 0;

        // Find all unvisited neighbors and their degrees
        for (int i = 0; i < 8; ++i) {
            int nr = r + dx[i];
            int nc = c + dy[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && grid[nr][nc] == 0) {
                int deg = countNeighbors(nr, nc);
                if (deg < minDeg) {
                    minDeg = deg;
                    candCount = 0;
                    candidates[candCount++] = {nr, nc};
                } else if (deg == minDeg) {
                    candidates[candCount++] = {nr, nc};
                }
            }
        }

        // If no valid neighbors, the heuristic failed
        if (candCount == 0) return false;

        // Tie-breaking: Choose the candidate furthest from the center
        int mid2 = N - 1;
        int maxDistSq = -1;
        int bestIdx = 0;
        for (int i = 0; i < candCount; ++i) {
            int dr = 2 * candidates[i].first - mid2;
            int dc = 2 * candidates[i].second - mid2;
            int distSq = dr * dr + dc * dc;
            if (distSq > maxDistSq) {
                maxDistSq = distSq;
                bestIdx = i;
            }
        }

        r = candidates[bestIdx].first;
        c = candidates[bestIdx].second;
        grid[r][c] = step;
    }
    return true;
}

int main() {
    // Speed up I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    // Trivial solution for N=1
    if (N == 1) {
        cout << "There is solution:\n1\n";
        return 0;
    }

    // Known impossible cases for N=2, 3, 4
    if (N < 5) {
        cout << "No solution.\n";
        return 0;
    }

    // Attempt to solve starting from board corners
    int corners[4][2] = {{0, 0}, {0, N - 1}, {N - 1, 0}, {N - 1, N - 1}};
    for (int i = 0; i < 4; ++i) {
        if (solve(corners[i][0], corners[i][1])) {
            cout << "There is solution:\n";
            for (int row = 0; row < N; ++row) {
                for (int col = 0; col < N; ++col) {
                    cout << grid[row][col] << (col == N - 1 ? "" : " ");
                }
                cout << "\n";
            }
            return 0;
        }
    }

    // If corner starts fail, try various random starting positions
    srand(time(0));
    for (int attempt = 0; attempt < 300; ++attempt) {
        int r = rand() % N;
        int c = rand() % N;
        if (solve(r, c)) {
            cout << "There is solution:\n";
            for (int row = 0; row < N; ++row) {
                for (int col = 0; col < N; ++col) {
                    cout << grid[row][col] << (col == N - 1 ? "" : " ");
                }
                cout << "\n";
            }
            return 0;
        }
    }

    // If no solution is found, output No solution.
    cout << "No solution.\n";
    return 0;
}

