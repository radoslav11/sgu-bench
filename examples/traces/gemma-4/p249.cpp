#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

/**
 * The problem asks to arrange numbers from 0 to 2^(N+M)-1 in a 2^N x 2^M matrix
 * such that adjacent cells (including cyclic adjacency) differ by exactly one bit.
 * 
 * Let R = 2^N and C = 2^M. We can use the property of the Gray code, G(x) = x ^ (x >> 1).
 * In a Gray code sequence, G(x) ^ G(x+1) is always a power of 2.
 * Furthermore, the cyclic property G(2^k - 1) ^ G(0) is also a power of 2 (specifically 2^(k-1)).
 * 
 * We can construct the matrix element at (i, j) as:
 * A[i][j] = (gray(j) << N) | gray(i)
 * where i is the row index (0 to R-1) and j is the column index (0 to C-1).
 * 
 * Verification:
 * 1. Row adjacency (i, j) and (i, j+1):
 *    A[i][j] ^ A[i][j+1] = ((gray(j) << N) | gray(i)) ^ ((gray(j+1) << N) | gray(i))
 *                        = (gray(j) ^ gray(j+1)) << N.
 *    Since gray(j) ^ gray(j+1) is a power of 2, this is also a power of 2.
 * 2. Cyclic row adjacency (i, C-1) and (i, 0):
 *    A[i][C-1] ^ A[i][0] = (gray(C-1) << N | gray(i)) ^ (gray(0) << N | gray(i))
 *                        = (gray(C-1) ^ gray(0)) << N.
 *    Since gray(C-1) ^ gray(0) is 2^(M-1), this is 2^(M-1+N), a power of 2.
 * 3. Column adjacency (i, j) and (i+1, j):
 *    A[i][j] ^ A[i+1][j] = (gray(j) << N | gray(i)) ^ (gray(j) << N | gray(i+1))
 *                        = gray(i) ^ gray(i+1).
 *    Since gray(i) ^ gray(i+1) is a power of 2, this is fine.
 * 4. Cyclic column adjacency (R-1, j) and (0, j):
 *    A[R-1][j] ^ A[0][j] = (gray(j) << N | gray(R-1)) ^ (gray(j) << N | gray(0))
 *                        = gray(R-1) ^ gray(0).
 *    Since gray(R-1) ^ gray(0) is 2^(N-1), this is a power of 2.
 * 
 * The values A[i][j] will be a permutation of 0 to 2^(N+M)-1 because gray(i) 
 * and gray(j) are permutations of 0 to 2^N-1 and 0 to 2^M-1 respectively.
 * 
 * Constraints: N+M <= 20, so 2^(N+M) <= 2^20 = 1,048,576. 
 * 0.5s time limit requires fast output.
 */

// Function to compute the standard binary-to-Gray code
inline int gray(int n) {
    return n ^ (n >> 1);
}

int main() {
    int N, M;
    // Read N and M from standard input
    if (scanf("%d %d", &N, &M) != 2) {
        return 0;
    }

    int R = 1 << N;
    int C = 1 << M;

    // Precompute Gray codes to optimize the loop
    vector<int> gray_R(R);
    vector<int> gray_C(C);
    for (int i = 0; i < R; ++i) {
        gray_R[i] = gray(i);
    }
    for (int j = 0; j < C; ++j) {
        gray_C[j] = gray(j);
    }

    // Print the matrix row by row
    for (int i = 0; i < R; ++i) {
        int gi = gray_R[i];
        for (int j = 0; j < C; ++j) {
            // Constructing A[i][j] = (gray(j) << N) | gray(i)
            int val = (gray_C[j] << N) | gi;
            // Using printf for fast output
            if (j == C - 1) {
                printf("%d\n", val);
            } else {
                printf("%d ", val);
            }
        }
    }

    return 0;
}

