#include <iostream>

/**
 * Problem: 163. Wise King
 * The king wants to choose a subset of his sons such that the sum of their
 * mental potentials raised to a fixed positive power p is maximized.
 * Each son's potential a_i is in the range [-3, 3].
 * The exponent p is in the range [1, 3].
 * If all possible subset sums are negative, the king can choose to send
 * no sons, resulting in a sum of 0.
 *
 * Approach:
 * 1. Read n (number of sons), p (exponent), and the potentials a_i.
 * 2. For each potential a_i, calculate v_i = a_i^p.
 * 3. The maximum subset sum is the sum of all positive v_i values.
 * 4. If no v_i is positive, the sum is 0.
 * 5. Constraints: n <= 100, p <= 3, a_i in [-3, 3].
 *    Maximum possible sum is 100 * 3^3 = 2700, which fits in a standard int.
 */

using namespace std;

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // Read the number of sons. If input fails, terminate.
    if (!(cin >> n)) {
        return 0;
    }

    int p;
    // Read the exponent p. If input fails, terminate.
    if (!(cin >> p)) {
        return 0;
    }

    // Use long long for the sum to prevent any possibility of overflow, 
    // though int is sufficient for the maximum possible sum (2700).
    long long max_sum = 0;

    for (int i = 0; i < n; ++i) {
        int a;
        // Read each son's potential.
        if (!(cin >> a)) {
            break;
        }

        // Calculate a^p manually to avoid precision issues with floating point functions.
        long long val = 1;
        for (int j = 0; j < p; ++j) {
            val *= a;
        }

        // If a^p is positive, include it in the maximum sum.
        // If a^p is zero or negative, adding it wouldn't increase the sum.
        if (val > 0) {
            max_sum += val;
        }
    }

    // Output the result.
    cout << max_sum << endl;

    return 0;
}

