#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * There are n cities, two countries start with cities s and t.
 * Each day, Country 1 can pick a set of cities C such that no city in C is a neighbor of any city in its current set A.
 * Then Country 2 can pick a set of cities C' such that no city in C' is a neighbor of any city in its current set B.
 * Country 1 wants to maximize popul(A) - popul(B), and Country 2 wants to maximize popul(B) - popul(A).
 * 
 * Constraint: n <= 13.
 * State: (maskA, maskB, turn, skipped)
 * Since A and B are disjoint, we can represent the state with a base-3 index of length n.
 * Each city is either in A, B, or neither.
 * 
 * The number of states is 3^n * 2 (turn) * 2 (skipped).
 * For n = 13, 3^13 = 1,594,323.
 * Transitions: For each state, a country can pick any subset of its currently available cities.
 * The number of transitions is at most 4^n. For n = 13, 4^13 = 67,108,864.
 * This should fit within 0.75s in C++.
 */

int n, s, t;
int adj[13];
int popul[13];
int powers3[14];
int sum_powers3_A[1 << 13];
int sum_powers3_B[1 << 13];
int neigh[1 << 13];
int popul_mask[1 << 13];

// memo[idx][turn][skipped]
// idx: base-3 index for maskA and maskB
// turn: 0 for Country 1, 1 for Country 2
// skipped: 0 for false, 1 for true
int memo[1594323][2][2];
bool visited[1594323][2][2];

int solve(int idx, int maskA, int maskB, int turn, int skipped) {
    if (visited[idx][turn][skipped]) return memo[idx][turn][skipped];

    int res;
    if (turn == 0) { // Country 1's turn
        // S1 is the set of cities available for Country 1
        int S1 = (~(maskA | maskB)) & (~neigh[maskA]) & ((1 << n) - 1);
        bool can_move = false;
        res = -2000000000;

        // Try picking any non-empty subset C of S1
        for (int C = S1; C > 0; C = (C - 1) & S1) {
            can_move = true;
            res = max(res, solve(idx + sum_powers3_A[C], maskA | C, maskB, 1, 0));
        }

        if (!can_move) {
            // If no cities can be picked, check if game ends or turn is skipped
            if (skipped) {
                res = popul_mask[maskA] - popul_mask[maskB];
            } else {
                res = solve(idx, maskA, maskB, 1, 1);
            }
        } else {
            // Even if moves are available, Country 1 can still choose to skip (pick C = empty set)
            res = max(res, solve(idx, maskA, maskB, 1, 1));
        }
    } else { // Country 2's turn
        // S2 is the set of cities available for Country 2
        int S2 = (~(maskA | maskB)) & (~neigh[maskB]) & ((1 << n) - 1);
        bool can_move = false;
        res = 2000000000;

        // Try picking any non-empty subset C of S2
        for (int C = S2; C > 0; C = (C - 1) & S2) {
            can_move = true;
            res = min(res, solve(idx + sum_powers3_B[C], maskA, maskB | C, 0, 0));
        }

        if (!can_move) {
            if (skipped) {
                res = popul_mask[maskA] - popul_mask[maskB];
            } else {
                res = solve(idx, maskA, maskB, 0, 1);
            }
        } else {
            // Even if moves are available, Country 2 can still choose to skip
            res = min(res, solve(idx, maskA, maskB, 0, 1));
        }
    }

    visited[idx][turn][skipped] = true;
    return memo[idx][turn][skipped] = res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> s >> t)) return 0;
    --s; --t;
    for (int i = 0; i < n; ++i) {
        string row;
        cin >> row;
        adj[i] = 0;
        for (int j = 0; j < n; ++j) {
            if (row[j] == '1') adj[i] |= (1 << j);
        }
    }
    for (int i = 0; i < n; ++i) cin >> popul[i];

    // Precompute powers of 3 and helpful sums
    powers3[0] = 1;
    for (int i = 1; i <= n; ++i) powers3[i] = powers3[i - 1] * 3;

    for (int i = 0; i < (1 << n); ++i) {
        sum_powers3_A[i] = 0;
        sum_powers3_B[i] = 0;
        popul_mask[i] = 0;
        for (int j = 0; j < n; ++j) {
            if ((i >> j) & 1) {
                sum_powers3_A[i] += powers3[j];
                sum_powers3_B[i] += 2 * powers3[j];
                popul_mask[i] += popul[j];
            }
        }
    }

    // Precompute neighbor masks for any set of cities
    for (int i = 0; i < (1 << n); ++i) {
        neigh[i] = 0;
        for (int j = 0; j < n; ++j) {
            if ((i >> j) & 1) neigh[i] |= adj[j];
        }
    }

    // Initial index calculation: s in A, t in B
    int initial_idx = powers3[s] + 2 * powers3[t];
    cout << solve(initial_idx, 1 << s, 1 << t, 0, 0) << endl;

    return 0;
}

