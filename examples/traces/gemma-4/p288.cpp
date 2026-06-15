#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem: Best Tournament Schedule (Round-robin tournament)
 * Goal: For N players, find the minimum number of tours so that every pair plays exactly once,
 * and no player plays more than once in the same tour.
 * 
 * Strategy:
 * - If N is even, the minimum number of tours is N-1. This is a known result from graph theory (edge coloring of K_N).
 *   We use the standard "circle method" or "polygon method" for scheduling round-robin tournaments.
 * - If N is odd, the minimum number of tours is N.
 *   A simple construction is using (i + j) mod N as the tour index for the match between player i and player j.
 * 
 * Complexity:
 * - Time: O(N^2) which is approximately 4*10^6 for N=2005, well within 1 second.
 * - Space: O(N^2) to store the resulting N x N matrix.
 */

int main() {
    // Optimize standard input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    // Special case N = 1: No matches to be played.
    if (N == 1) {
        cout << 0 << "\n";
        cout << 0 << "\n";
        return 0;
    }

    // Using a 2D vector to store the tour number for each pair (i, j)
    // matrix[i][j] is the tour in which player i and player j compete.
    // For i == j, matrix[i][j] is 0.
    vector<vector<int>> matrix(N, vector<int>(N, 0));

    if (N % 2 == 0) {
        // Case 1: N is even. Minimum number of tours is T = N - 1.
        int T = N - 1;
        // The circle method for even N:
        // Fix one player (index N-1) and rotate the other N-1 players.
        for (int t = 0; t < N - 1; ++t) {
            // In tour t+1 (t is 0-indexed):
            // Player N-1 plays player t.
            matrix[t][N - 1] = matrix[N - 1][t] = t + 1;
            
            // Other matches in the same tour are formed by pairing (t-i, t+i) mod (N-1).
            // This ensures every player is used exactly once per tour.
            for (int i = 1; i < N / 2; ++i) {
                int u = (t - i + (N - 1)) % (N - 1);
                int v = (t + i) % (N - 1);
                matrix[u][v] = matrix[v][u] = t + 1;
            }
        }
        cout << T << "\n";
    } else {
        // Case 2: N is odd. Minimum number of tours is T = N.
        int T = N;
        // In an odd N round-robin, one player must sit out each tour.
        // Using (i + j) mod N as the tour index guarantees N tours, each with (N-1)/2 matches.
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    // Tour numbers are 1-indexed for the output.
                    matrix[i][j] = (i + j) % N + 1;
                }
            }
        }
        cout << T << "\n";
    }

    // Output the schedule matrix.
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << matrix[i][j] << (j == N - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}

