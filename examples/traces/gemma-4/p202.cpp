#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdio>
#include <string>

using namespace std;

// Use __int128 to prevent overflow for T(N, M) calculations.
// While M >= 4 makes T(N, M) much smaller than 2^64-1,
// intermediate steps or M=3 case could potentially involve large numbers.
typedef __int128_t int128;

int128 T[66][66];
int best_k[66][66];
stack<int> pegs[66];

/**
 * Moves a disk from source to destination peg.
 * Prints the move in the required format based on whether the target peg is empty.
 */
void move_disk(int disk, int src, int dest) {
    if (pegs[dest].empty()) {
        printf("move %d from %d to %d\n", disk, src, dest);
    } else {
        printf("move %d from %d to %d atop %d\n", disk, src, dest, pegs[dest].top());
    }
    pegs[src].pop();
    pegs[dest].push(disk);
}

/**
 * The Frame-Stewart algorithm for the multi-peg Towers of Hanoi.
 * @param n Number of disks to move.
 * @param m Number of available pegs.
 * @param src Source peg index.
 * @param dest Target peg index.
 * @param aux List of available auxiliary peg indices.
 */
void solve(int n, int m, int src, int dest, vector<int> aux) {
    if (n == 0) return;
    if (n == 1) {
        int disk = pegs[src].top();
        move_disk(disk, src, dest);
        return;
    }
    if (m == 3) {
        // Base case for 3 pegs: standard recursive Hanoi
        int a = aux[0];
        solve(n - 1, 3, src, a, {dest});
        int disk = pegs[src].top();
        move_disk(disk, src, dest);
        solve(n - 1, 3, a, dest, {src});
        return;
    }

    // Multi-peg Frame-Stewart recursion:
    // 1. Move k disks to an auxiliary peg using m pegs.
    // 2. Move the remaining n-k disks to the destination using m-1 pegs.
    // 3. Move the k disks from the auxiliary peg to the destination using m pegs.
    int k = best_k[n][m];

    // Step 1: Target is aux[0]. Available pegs: src, dest, aux[0], and other aux.
    vector<int> aux1;
    for (int i = 1; i < (int)aux.size(); ++i) aux1.push_back(aux[i]);
    aux1.push_back(dest);
    solve(k, m, src, aux[0], aux1);

    // Step 2: Target is dest. Available pegs: src, dest, and aux[1...].
    vector<int> aux2;
    for (int i = 1; i < (int)aux.size(); ++i) aux2.push_back(aux[i]);
    solve(n - k, m - 1, src, dest, aux2);

    // Step 3: Target is dest. Available pegs: src, dest, aux[0], and other aux.
    vector<int> aux3;
    for (int i = 1; i < (int)aux.size(); ++i) aux3.push_back(aux[i]);
    aux3.push_back(src);
    solve(k, m, aux[0], dest, aux3);
}

/**
 * Helper to print __int128 as a decimal string.
 */
void print_int128(int128 n) {
    if (n == 0) {
        printf("0\n");
        return;
    }
    string s = "";
    while (n > 0) {
        s += (char)('0' + (long long)(n % 10));
        n /= 10;
    }
    reverse(s.begin(), s.end());
    printf("%s\n", s.c_str());
}

int main() {
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) return 0;

    // Precompute the optimal number of moves T[n][m] and choice of k using DP.
    for (int n = 1; n <= 64; ++n) {
        T[n][3] = ((int128)1 << n) - 1;
    }
    for (int m = 4; m <= 65; ++m) T[0][m] = 0;

    for (int m = 4; m <= 65; ++m) {
        for (int n = 1; n <= 64; ++n) {
            if (n == 1) {
                T[n][m] = 1;
                continue;
            }
            T[n][m] = -1;
            for (int k = 1; k < n; ++k) {
                int128 val = (int128)2 * T[k][m] + T[n - k][m - 1];
                if (T[n][m] == -1 || val < T[n][m]) {
                    T[n][m] = val;
                    best_k[n][m] = k;
                }
            }
        }
    }

    // Print total number of moves.
    print_int128(T[N][M]);

    // Initialize pegs with disks.
    for (int i = N; i >= 1; --i) {
        pegs[1].push(i);
    }

    // Set up initial auxiliary pegs.
    vector<int> initial_aux;
    for (int i = 2; i <= M - 1; ++i) {
        initial_aux.push_back(i);
    }

    // Run the recursive solution and print moves.
    solve(N, M, 1, M, initial_aux);

    return 0;
}

