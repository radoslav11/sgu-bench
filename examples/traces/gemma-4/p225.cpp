#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/**
 * Problem: Little Knights
 * We need to find the number of ways to place k knights on an n x n chessboard
 * so that no two knights are in an attacking position.
 * 
 * Constraints: n <= 10, k <= n^2.
 * 
 * Since n is small (up to 10), the problem can be solved using dynamic programming
 * with a "broken profile" mask. The state needs to track the occupancy of all cells
 * that could attack future cells. For a knight's move, the relevant cells are
 * in the previous two rows. Specifically, a knight at (r, c) attacks:
 * (r-2, c-1), (r-2, c+1), (r-1, c-2), (r-1, c+2).
 * 
 * As we iterate cell by cell through the board, a mask that tracks the last 
 * 2*n+1 cells is sufficient.
 */

int N, K;
unordered_map<long long, long long> memo;

// solve(idx, k_left, mask)
// idx: current cell index (0 to N*N - 1)
// k_left: number of knights remaining to be placed
// mask: occupancy of the previous 2*N + 1 cells
long long solve(int idx, int k_left, int mask) {
    // Base cases
    if (k_left == 0) return 1;
    if (idx == N * N) return 0;
    if (N * N - idx < k_left) return 0;

    // Memoization state: combine idx, k_left, and mask into a single long long
    // idx: 7 bits (up to 100), k_left: 7 bits (up to 100), mask: 21 bits (up to 2^21)
    long long state = ((long long)idx << 28) | ((long long)k_left << 21) | mask;
    auto it = memo.find(state);
    if (it != memo.end()) return it->second;

    long long res = 0;

    // Option 1: Do not place a knight at current cell 'idx'
    // The new mask is (mask << 1) restricted to 2*N + 1 bits
    res += solve(idx + 1, k_left, (mask << 1) & ((1 << (2 * N + 1)) - 1));

    // Option 2: Place a knight at current cell 'idx'
    int r = idx / N;
    int c = idx % N;
    bool can_place = true;

    /**
     * We use a bitmask where bit j represents the cell at (idx - (j + 1)).
     * The knight's attack rules in terms of relative offsets:
     * (r-1, c-2)  => idx - (N + 2)   => mask bit (N + 1)
     * (r-1, c+2)  => idx - (N - 2)   => mask bit (N - 3)
     * (r-2, c-1)  => idx - (2N + 1)  => mask bit (2N)
     * (r-2, c+1)  => idx - (2N - 1)  => mask bit (2N - 2)
     */

    if (c >= 2 && (mask & (1 << (N + 1)))) can_place = false;
    if (can_place && c + 2 < N && (mask & (1 << (N - 3)))) can_place = false;
    if (can_place && r >= 2 && c >= 1 && (mask & (1 << (2 * N)))) can_place = false;
    if (can_place && r >= 2 && c + 1 < N && (mask & (1 << (2 * N - 2)))) can_place = false;

    if (can_place) {
        // Place the knight and update mask with bit 0 set to 1
        res += solve(idx + 1, k_left - 1, ((mask << 1) | 1) & ((1 << (2 * N + 1)) - 1));
    }

    return memo[state] = res;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> K)) return 0;

    // Corner case: k larger than total cells
    if (K > N * N) {
        cout << 0 << endl;
        return 0;
    }

    // Corner case: k is 0
    if (K == 0) {
        cout << 1 << endl;
        return 0;
    }

    // Start recursion
    cout << solve(0, K, 0) << endl;

    return 0;
}

