#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem: 181. X-Sequence
 * The sequence is defined as:
 * x0 = A
 * xi = (alpha * xi-1^2 + beta * xi-1 + gamma) mod M, for i >= 1.
 * 
 * Given the constraints:
 * A <= 10000
 * alpha, beta, gamma <= 100
 * M <= 1000
 * k <= 10^9
 * 
 * Since the sequence elements x_i for i >= 1 are all in the range [0, M-1],
 * and M is small (up to 1000), the sequence must enter a cycle quickly.
 * We can detect this cycle by keeping track of the index at which each value 
 * first appears.
 */

int main() {
    // Speed up I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long A, alpha, beta, gamma, M, k;
    if (!(cin >> A >> alpha >> beta >> gamma >> M >> k)) return 0;

    // Case k = 0: the result is simply A.
    if (k == 0) {
        cout << A << endl;
        return 0;
    }

    // seq stores the sequence elements x_0, x_1, ..., x_i.
    vector<long long> seq;
    seq.push_back(A);

    // visited[v] stores the index i where the value v first appeared in the sequence.
    // This helps in detecting the start of a cycle.
    // Since M <= 1000, the values x_i for i >= 1 are in [0, M-1].
    vector<int> visited(M, -1);

    // If A is in the range [0, M-1], we can mark it as appearing at index 0.
    // This allows for cycles that include the initial value x_0.
    if (A < M) {
        visited[A] = 0;
    }

    // Iterate to find x_k, either by calculating it directly or by using cycle detection.
    for (long long i = 1; i <= k; ++i) {
        long long prev_x = seq.back();
        
        // Intermediate calculation can reach ~10^10, so use long long.
        // The formula is xi = (alpha * xi-1^2 + beta * xi-1 + gamma) mod M.
        long long next_x = (alpha * prev_x * prev_x + beta * prev_x + gamma) % M;

        // If next_x has been seen before, we've detected a cycle.
        if (visited[next_x] != -1) {
            int start_idx = visited[next_x];
            int cycle_len = (int)i - start_idx;
            
            // Calculate the index within the sequence that corresponds to x_k.
            // Since k >= i, the value x_k is part of the repeating pattern.
            long long target_idx_long = start_idx + (k - start_idx) % (long long)cycle_len;
            int target_idx = (int)target_idx_long;
            
            cout << seq[target_idx] << endl;
            return 0;
        }

        // Mark this value's first appearance and add to the sequence.
        visited[next_x] = (int)i;
        seq.push_back(next_x);
    }

    // If no cycle was detected (only possible for very small k), print x_k.
    // At this point, seq.back() is x_k.
    cout << seq.back() << endl;

    return 0;
}

