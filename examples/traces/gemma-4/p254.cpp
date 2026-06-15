#include <cstdio>

/**
 * Problem: 254. Strange Random
 * The problem asks to simulate the deletion of integers from a circle of N numbers.
 * We start from 1 and move in a certain direction (clockwise initially).
 * Each step:
 * 1. Erase the Q-th integer in the current direction.
 * 2. Move to the next integer clockwise from the erased integer.
 * 3. If the next integer is odd, set direction to clockwise; if even, to counter-clockwise.
 * 4. Repeat until no integers are left.
 * 
 * Constraints: N <= 2,000,000; Q <= 10.
 * Time limit: 0.25 sec. (This is very tight for N=2*10^6)
 * Memory limit: 9216 KB (Very tight for N=2*10^6)
 * 
 * A Fenwick tree is used to efficiently find and erase elements.
 * Since N is up to 2*10^6, we can't use two large arrays (like values and tree).
 * However, because the numbers are 1 to N, the value of the k-th element is simply
 * the index of the k-th active element in the original sequence.
 * A Fenwick tree stores the presence of elements (1 if active, 0 if erased).
 * Binary lifting on the Fenwick tree allows finding the k-th active element in O(log N).
 */

using namespace std;

// Max N = 2,000,000. The next power of 2 is 2^21 = 2,097,152.
// To keep the tree size within memory limits (9216 KB), we'll use a size that fits.
// 2,097,153 * 4 bytes = 8,388,612 bytes (approx 8192 KB).
const int MAXN = 2097153;
int tree[MAXN];
int N_global;

// Standard Fenwick tree update
void update(int idx, int val, int n) {
    for (; idx <= n; idx += idx & -idx) {
        tree[idx] += val;
    }
}

// Find the k-th element using binary lifting on the Fenwick tree in O(log N)
int find_kth(int k, int n) {
    int idx = 0;
    for (int i = 1 << 21; i > 0; i >>= 1) {
        if (idx + i <= n && tree[idx + i] < k) {
            idx += i;
            k -= tree[idx];
        }
    }
    return idx + 1;
}

int main() {
    int N, Q;
    // Using scanf for faster I/O
    if (scanf("%d %d", &N, &Q) != 2) return 0;
    N_global = N;

    // Initialize the Fenwick tree with all elements present
    for (int i = 1; i <= N; ++i) {
        update(i, 1, N);
    }

    int current_idx = 0; // 0-indexed position among current active elements
    int M = N; // Current number of active elements
    bool clockwise = true; // true for CW, false for CCW
    int last_erased = 0;

    while (M > 0) {
        int target_idx;
        if (clockwise) {
            // Q-th element starting from current_idx clockwise
            target_idx = (current_idx + Q - 1) % M;
        } else {
            // Q-th element starting from current_idx counter-clockwise
            // The count goes: current_idx, current_idx-1, ...
            // To handle negative results of (current_idx - (Q-1)), we use modulo M
            target_idx = (current_idx - (Q - 1) % M + M) % M;
        }

        // V is the value of the target integer (the original index)
        int V = find_kth(target_idx + 1, N);
        last_erased = V;

        if (M > 1) {
            // Find the next integer clockwise to decide the next direction
            // The next clockwise integer's index in the current set of M elements
            int next_clockwise_idx = (target_idx + 1) % M;
            int V_next = find_kth(next_clockwise_idx + 1, N);

            // Erase the element V
            update(V, -1, N);
            M--;

            // The element that was at target_idx+1 (or 0) moves to target_idx (mod M-1)
            current_idx = target_idx % M;
            
            // Determine direction based on V_next's parity
            clockwise = (V_next % 2 != 0);
        } else {
            // Only one element left, erase it and stop
            update(V, -1, N);
            M--;
        }
    }

    printf("%d\n", last_erased);

    return 0;
}

