#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;
    vector<vector<int>> A(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> A[i][j];
        }
    }

    // Precompute column minimums
    vector<int> col_min(M, INT_MAX);
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            col_min[j] = min(col_min[j], A[i][j]);
        }
    }

    // Precompute suffix minimums for columns (from j to M-1)
    vector<int> suff_col(M);
    suff_col[M-1] = col_min[M-1];
    for (int j = M-2; j >= 0; j--) {
        suff_col[j] = min(col_min[j], suff_col[j+1]);
    }

    // Precompute col_suf: for each column j, col_suf[j][i] = min value from row i to N-1 in column j
    vector<vector<int>> col_suf(M, vector<int>(N));
    for (int j = 0; j < M; j++) {
        col_suf[j][N-1] = A[N-1][j];
        for (int i = N-2; i >= 0; i--) {
            col_suf[j][i] = min(A[i][j], col_suf[j][i+1]);
        }
    }

    // Build matrix B
    vector<vector<int>> B(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = M-1; j >= 0; j--) {
            if (i == 0) {
                B[i][j] = suff_col[j];
            } else {
                int candidate = col_suf[j][i];
                if (j + 1 < M) {
                    candidate = min(candidate, B[i-1][j+1]);
                }
                B[i][j] = candidate;
            }
        }
    }

    // Output
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (j > 0) {
                cout << " ";
            }
            cout << B[i][j];
        }
        cout << '\n';
    }

    return 0;
}
