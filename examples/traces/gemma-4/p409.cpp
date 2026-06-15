#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * Problem Analysis:
 * We need to create an N^2 x N^2 flag where each row, column, and N x N block
 * contains exactly K blue cells ('*').
 * 
 * Construction Strategy:
 * Let the flag be divided into N x N blocks. Each block (i, j) where 0 <= i, j < N
 * contains cells (i*N + r, j*N + c) for 0 <= r, c < N.
 * 
 * We can pick K distinct pairs of (r, c) from the N x N possible pairs.
 * Let S = {(r_1, c_1), (r_2, c_2), ..., (r_K, c_K)} be the set of these pairs.
 * 
 * For each block (i, j), we pick the K cells:
 * cell(i, j, k) = (i*N + (r_k + j) % N, j*N + (c_k + i) % N) for k = 1 to K.
 * 
 * Let's verify this satisfies the conditions:
 * 1. Each N x N block (i, j) has exactly K blue cells by construction.
 * 2. Each row R = i*N + r has exactly K blue cells.
 *    For a fixed i and r, we need to find (j, k) such that:
 *    (r_k + j) % N = r.
 *    For each k, there's exactly one j in [0, N-1] that satisfies this: j = (r - r_k + N) % N.
 *    The corresponding column C for that (i, j, k) will be:
 *    C = j*N + (c_k + i) % N.
 *    Since j < N and (c_k + i) % N < N, this is a valid column index in [0, N^2-1].
 *    Thus, each k corresponds to exactly one cell in row R, totaling K blue cells.
 * 3. Each column C = j*N + c has exactly K blue cells.
 *    For a fixed j and c, we need to find (i, k) such that:
 *    (c_k + i) % N = c.
 *    For each k, there's exactly one i in [0, N-1] that satisfies this: i = (c - c_k + N) % N.
 *    The corresponding row R for that (i, j, k) will be:
 *    R = i*N + (r_k + j) % N.
 *    Since i < N and (r_k + j) % N < N, this is a valid row index in [0, N^2-1].
 *    Thus, each k corresponds to exactly one cell in column C, totaling K blue cells.
 * 
 * Time Complexity: O(N^4) which is at most 20^4 = 160,000 operations.
 * Space Complexity: O(N^4) for the grid, which is at most 400*400 = 160,000 chars.
 */

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    int M = N * N;
    // Initialize the flag with red cells ('.')
    vector<string> grid(M, string(M, '.'));

    // Create a set S of K distinct (r, c) pairs from the N x N grid
    vector<pair<int, int>> S;
    for (int r = 0; r < N && S.size() < (size_t)K; ++r) {
        for (int c = 0; c < N && S.size() < (size_t)K; ++c) {
            S.push_back({r, c});
        }
    }

    // If K is greater than N^2, no solution is possible (but constraints say K <= N^2)
    if (S.size() < (size_t)K) {
        cout << "NO SOLUTION" << endl;
        return 0;
    }

    // Apply our construction for each block (i, j) and each pair in S
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (const auto& p : S) {
                int r_k = p.first;
                int c_k = p.second;
                int R = i * N + (r_k + j) % N;
                int C = j * N + (c_k + i) % N;
                grid[R][C] = '*';
            }
        }
    }

    // Output the final flag grid
    for (int i = 0; i < M; ++i) {
        cout << grid[i] << "\n";
    }

    return 0;
}

