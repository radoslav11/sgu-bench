#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<int> A(N * M);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> A[i * M + j];
        }
    }

    // Precompute suffix minimums for each column
    // A[i][j] will store min(A[x][j]) for x >= i
    for (int j = 0; j < M; ++j) {
        for (int i = N - 2; i >= 0; --i) {
            A[i * M + j] = min(A[i * M + j], A[(i + 1) * M + j]);
        }
    }

    // Compute the result matrix B
    // B[i][j] = min( A[i][j] (which is now the suffix min), B[i-1][j+1] )
    vector<int> B(N * M, INF);
    for (int i = 0; i < N; ++i) {
        for (int j = M - 1; j >= 0; --j) {
            int val = A[i * M + j];
            if (i > 0 && j + 1 < M) {
                val = min(val, B[(i - 1) * M + (j + 1)]);
            }
            B[i * M + j] = val;
        }
    }

    // Output the result
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << B[i * M + j] << (j == M - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
