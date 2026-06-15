#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * We have N coins in a row, and M operations are performed on them.
 * Each operation selects a K-length window and applies a transformation X, D times.
 * Transformation X:
 *   1. Cyclic left shift the window of K coins.
 *   2. Scan the window C[1]...C[K-1]: if C[i]=1 and A[i]=1, flip C[K].
 * A is a (K-1) length binary vector. We are given L pairs of (before-transformation, after-transformation) 
 * to reconstruct A. Then, given the final row of coins, reconstruct the initial row.
 * 
 * Solution:
 * 1. Represent the transformation X as a KxK matrix over GF(2).
 * 2. Solve the system of L equations to find A.
 * 3. Compute the inverse of the transformation X, and apply the inverse operations in reverse order.
 *    Since M is small and D can be large, we use matrix exponentiation.
 */

typedef vector<bitset<50>> Matrix;

Matrix multiply(const Matrix& A, const Matrix& B, int K) {
    Matrix C(K);
    for (int i = 0; i < K; ++i) {
        for (int k = 0; k < K; ++k) {
            if (A[i][k]) {
                C[i] ^= B[k];
            }
        }
    }
    return C;
}

Matrix power(Matrix A, int p, int K) {
    Matrix res(K);
    for (int i = 0; i < K; ++i) res[i][i] = 1;
    while (p > 0) {
        if (p & 1) res = multiply(res, A, K);
        A = multiply(A, A, K);
        p >>= 1;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K, L;
    if (!(cin >> N >> M >> K >> L)) return 0;

    vector<pair<int, int>> ops(M);
    for (int i = 0; i < M; ++i) {
        cin >> ops[i].first >> ops[i].second;
    }

    vector<bitset<51>> equations(L);
    for (int i = 0; i < L; ++i) {
        string b_str, a_str;
        cin >> b_str >> a_str;
        // The equation: sum_{j=0}^{K-2} (b[j+1] * A[j]) = a[K-1] ^ b[0]
        // b_str and a_str are 0-indexed in the code.
        for (int j = 0; j < K - 1; ++j) {
            if (b_str[j + 1] == '1') {
                equations[i].set(j);
            }
        }
        if ((a_str[K - 1] == '1') ^ (b_str[0] == '1')) {
            equations[i].set(K - 1); // Target column (last column)
        }
    }

    // Gaussian elimination to solve for A[0]...A[K-2]
    int row = 0;
    vector<int> pivot_row(K - 1, -1);
    for (int j = 0; j < K - 1 && row < L; ++j) {
        int pivot = -1;
        for (int i = row; i < L; ++i) {
            if (equations[i][j]) {
                pivot = i;
                break;
            }
        }
        if (pivot != -1) {
            swap(equations[row], equations[pivot]);
            pivot_row[j] = row;
            for (int i = 0; i < L; ++i) {
                if (i != row && equations[i][j]) {
                    equations[i] ^= equations[row];
                }
            }
            row++;
        }
    }

    vector<int> A(K - 1);
    for (int j = 0; j < K - 1; ++j) {
        if (pivot_row[j] != -1) {
            A[j] = equations[pivot_row[j]][K - 1];
        } else {
            A[j] = 0; // Should not happen based on the guarantee
        }
    }

    // Construct M_X inverse
    Matrix MX_inv(K);
    // Row 0: A[0], A[1], ..., A[K-2], 1
    for (int j = 0; j < K - 1; ++j) if (A[j]) MX_inv[0].set(j);
    MX_inv[0].set(K - 1);
    // Row i: 1 at col i-1 for i=1 to K-1
    for (int i = 1; i < K; ++i) MX_inv[i].set(i - 1);

    string final_row_str;
    cin >> final_row_str;
    vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = final_row_str[i] - '0';

    // Apply operations in reverse
    for (int i = M - 1; i >= 0; --i) {
        int S = ops[i].first;
        int D = ops[i].second;
        Matrix MX_inv_D = power(MX_inv, D, K);

        vector<int> window(K);
        for (int j = 0; j < K; ++j) window[j] = v[S + j - 1];

        vector<int> window_new(K, 0);
        for (int r = 0; r < K; ++r) {
            int sum = 0;
            for (int c = 0; c < K; ++c) {
                if (MX_inv_D[r][c] && window[c]) sum ^= 1;
            }
            window_new[r] = sum;
        }

        for (int j = 0; j < K; ++j) v[S + j - 1] = window_new[j];
    }

    for (int i = 0; i < N; ++i) {
        cout << v[i];
    }
    cout << endl;

    return 0;
}

