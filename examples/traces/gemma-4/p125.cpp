#include <iostream>
#include <vector>

using namespace std;

/*
 * Problem: Shtirlits
 * The goal is to find an N x N matrix A of soldiers (0 <= A[i, j] <= 9)
 * such that for each cell (i, j), B[i, j] is the count of its neighbors
 * with a strictly larger army size.
 * 
 * Approach:
 * Since the number of possible army sizes is small (0-9) and the number of 
 * cells is expected to be small (N is up to 3, though it might be slightly more),
 * we use backtracking with pruning.
 * 
 * Pruning:
 * For each cell (i, j) that is already filled, we check if its required B[i, j] 
 * value is still achievable. Specifically, we let:
 *   cnt_greater = number of filled neighbors with A[ni, nj] > A[i, j]
 *   cnt_unfilled = number of neighbors not yet filled
 * The condition is: cnt_greater <= B[i, j] <= cnt_greater + cnt_unfilled.
 * 
 * This condition is checked for the current cell (r, c) and its neighbors
 * every time a cell is filled.
 */

int N;
int B[15][15], A[15][15];
bool filled[15][15];
bool solved = false;
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

struct Neighbor {
    int r, c;
};
Neighbor adj[15][15][4];
int adj_size[15][15];

/**
 * is_valid checks whether the current configuration of A satisfies
 * the B constraints for the cell (r, c) and all its neighbors.
 */
inline bool is_valid(int r, int c) {
    // Check (r, c) itself
    int cnt_greater = 0;
    int cnt_unfilled = 0;
    for (int d = 0; d < adj_size[r][c]; ++d) {
        int ni = adj[r][c][d].r;
        int nj = adj[r][c][d].c;
        if (filled[ni][nj]) {
            if (A[ni][nj] > A[r][c]) cnt_greater++;
        } else {
            cnt_unfilled++;
        }
    }
    if (cnt_greater > B[r][c]) return false;
    if (cnt_greater + cnt_unfilled < B[r][c]) return false;

    // Check neighbors of (r, c) that are already filled
    for (int d = 0; d < 4; ++d) {
        int ni = r + dr[d];
        int nj = c + dc[d];
        if (ni >= 0 && ni < N && nj >= 0 && nj < N && filled[ni][nj]) {
            int c_g = 0;
            int c_u = 0;
            for (int k = 0; k < adj_size[ni][nj]; ++k) {
                int nni = adj[ni][nj][k].r;
                int nnj = adj[ni][nj][k].c;
                if (filled[nni][nnj]) {
                    if (A[nni][nnj] > A[ni][nj]) c_g++;
                } else {
                    c_u++;
                }
            }
            if (c_g > B[ni][nj]) return false;
            if (c_g + c_u < B[ni][nj]) return false;
        }
    }
    return true;
}

/**
 * backtrack performs a depth-first search through all possible army sizes.
 * idx: the current cell index in row-major order (0 to N*N - 1).
 */
void backtrack(int idx) {
    if (idx == N * N) {
        solved = true;
        return;
    }
    int r = idx / N;
    int c = idx % N;

    for (int val = 0; val <= 9; ++val) {
        A[r][c] = val;
        filled[r][c] = true;
        if (is_valid(r, c)) {
            backtrack(idx + 1);
            if (solved) return;
        }
        filled[r][c] = false;
    }
}

int main() {
    // Speed up input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!(cin >> B[i][j])) break;
        }
    }

    // Pre-calculate neighbor lists for each cell
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            adj_size[i][j] = 0;
            for (int d = 0; d < 4; ++d) {
                int ni = i + dr[d];
                int nj = j + dc[d];
                if (ni >= 0 && ni < N && nj >= 0 && nj < N) {
                    adj[i][j][adj_size[i][j]++] = {ni, nj};
                }
            }
        }
    }

    // Initialize filled array to false
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            filled[i][j] = false;
        }
    }

    backtrack(0);

    if (solved) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << A[i][j] << (j == N - 1 ? "" : " ");
            }
            cout << "\n";
        }
    } else {
        cout << "NO SOLUTION" << endl;
    }

    return 0;
}

