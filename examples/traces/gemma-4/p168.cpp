#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * Given an N*M matrix A, find matrix B such that
 * B[i,j] = min { A[x,y] : y >= j AND x+y >= i+j }.
 * 
 * Constraints:
 * N, M <= 1000, A[i,j] <= 32000 by absolute value.
 * Time limit: 0.5 sec, Memory limit: 16000 KB.
 * 
 * Approach:
 * Let s = i+j. Then B[i,j] = min { A[x,y] : x+y >= s AND y >= j }.
 * For a fixed y, the condition x+y >= s is equivalent to x >= max(0, s-y).
 * So B[i,j] = min { ColSuffixMin[max(0, s-y)][y] : y >= j AND max(0, s-y) < N }.
 * Let F[s][j] = min { ColSuffixMin[max(0, s-y)][y] : y >= j }.
 * 
 * We can compute ColSuffixMin[x][y] = min_{k=x}^{N-1} A[k][y] in O(NM) time.
 * Then for each possible sum s (0 to N+M-2), we compute F[s][j] for all j (0 to M-1) 
 * in O(M) using a suffix minimum calculation:
 * F[s][j] = min(ColSuffixMin[max(0, s-j)][j], F[s][j+1]).
 * Note that we handle the condition max(0, s-y) < N by treating ColSuffixMin as INF 
 * if s-y >= N.
 * 
 * Final B[i,j] = F[i+j][j]. 
 * Total complexity: O(NM). Memory complexity: O(NM).
 */

const int INF = 1e9;
int A[1000][1000];
int B[1000][1000];
int current_F[1000];

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // Read matrix A
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> A[i][j];
        }
    }

    // Compute ColSuffixMin in-place in matrix A to save memory.
    // A[i][j] will now store the minimum of column j from row i down to N-1.
    for (int j = 0; j < M; ++j) {
        for (int i = N - 2; i >= 0; --i) {
            if (A[i + 1][j] < A[i][j]) {
                A[i][j] = A[i + 1][j];
            }
        }
    }

    // Compute B[i,j] using the calculated ColSuffixMin and suffix minima on diagonals.
    for (int s = 0; s < N + M - 1; ++s) {
        // Calculate the suffix minimums for the current diagonal-like constraint s.
        // current_F[j] will store F[s][j] = min { A[max(0, s-y)][y] : y >= j and s-y < N }
        
        // Base case for suffix minimum: the last column in the range.
        int last_y = M - 1;
        int last_x = s - last_y;
        if (last_x < N) {
            current_F[last_y] = A[max(0, last_x)][last_y];
        } else {
            current_F[last_y] = INF;
        }

        // Suffix minimums for j from M-2 down to 0.
        for (int j = M - 2; j >= 0; --j) {
            int x_val = s - j;
            int val = (x_val < N ? A[max(0, x_val)][j] : INF);
            current_F[j] = min(val, current_F[j + 1]);
        }

        // Now populate B[i][j] for all i, j where i+j = s.
        // The constraints on i are: 0 <= i < N AND 0 <= s-i < M.
        // Which implies i <= s and i > s-M.
        int start_i = max(0, s - M + 1);
        int end_i = min(s, N - 1);
        for (int i = start_i; i <= end_i; ++i) {
            int j = s - i;
            B[i][j] = current_F[j];
        }
    }

    // Output matrix B.
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << B[i][j] << (j == M - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}

