#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem Analysis:
 * - We have 2k points on a circle, labeled A1, A2, ..., A2k in order.
 * - k chords are drawn connecting these 2k points such that each point is an endpoint of exactly one chord.
 * - These k chords divide the circle into P parts.
 * - We want to find the minimum possible number of parts P and the number of ways N to achieve this P.
 * 
 * Number of parts calculation:
 * - For k chords with I intersection points (where no three chords intersect at the same point), 
 *   the number of parts P is given by the formula: P = k + I + 1.
 * - To minimize P, we need to minimize the number of intersections I.
 * - The minimum possible number of intersections I is 0.
 * - When I = 0, the number of parts is P = k + 0 + 1 = k + 1.
 * - The number of ways to connect 2k points with k non-intersecting chords is the k-th Catalan number C_k.
 * 
 * Catalan number formula:
 * - C_n = (1 / (n + 1)) * binom(2n, n)
 * - Recurrence relation: C_n = sum_{i=0}^{n-1} (C_i * C_{n-1-i}), where C_0 = 1.
 * 
 * Constraints and Complexity:
 * - k <= 30.
 * - C_30 is approximately 3.8 * 10^15, which fits within a 64-bit signed integer (long long).
 * - Time complexity for the recurrence is O(k^2), which is very efficient.
 * - Space complexity is O(k).
 */

int main() {
    // Speed up input and output operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k;
    // Reading k from standard input
    if (!(cin >> k)) return 0;

    // For k >= 1, we can use the Catalan recurrence to find the number of ways N.
    // If k = 0 was possible, N would be 1 and P would be 1, but the problem says 1 <= k <= 30.
    
    // Use long long for Catalan numbers to avoid overflow.
    vector<long long> C(k + 1, 0);
    C[0] = 1;

    // Compute Catalan numbers up to C_k using the recurrence:
    // C_i = C_0*C_{i-1} + C_1*C_{i-2} + ... + C_{i-1}*C_0
    for (int i = 1; i <= k; ++i) {
        for (int j = 0; j < i; ++j) {
            C[i] += C[j] * C[i - 1 - j];
        }
    }

    // N is the k-th Catalan number, and P is k + 1.
    long long N = C[k];
    int P = k + 1;

    // Print N and P delimited by space.
    cout << N << " " << P << endl;

    return 0;
}

