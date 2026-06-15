#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * The number of variants of evolution for a troop of N droids is the number of ways
 * to arrange N droids in rows with the same number of droids in each row.
 * This is equivalent to finding the number of divisors of N, denoted as d(N).
 * Given K, we want the smallest N such that d(N) = K.
 *
 * If N = p1^a1 * p2^a2 * ... * pm^am, then d(N) = (a1 + 1)(a2 + 1)...(am + 1).
 * To minimize N, larger exponents should correspond to smaller prime numbers.
 * Therefore, a1 >= a2 >= ... >= am for the smallest N.
 *
 * Constraints:
 * K <= 10^5.
 * N can potentially exceed 2^64-1 (e.g., if K is a large prime). 
 * However, the requirement to output 0 if no such N exists suggests N must fit
 * within a 64-bit unsigned integer.
 *
 * Approach:
 * We use backtracking to decompose K into a product of factors (a_i + 1), 
 * ensuring the non-increasing exponent property to find the minimal N efficiently.
 * We use __int128 for intermediate calculations to avoid overflow and correctly 
 * compare potential values of N against the 64-bit limit.
 */

typedef __int128_t int128;

int K_val;
int128 min_N = ((int128)1 << 127); // Large initial value for minimization
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
vector<int> divisors;

/**
 * dfs: Backtracking to explore possible factorizations of K.
 * @param prime_idx Index of the current prime being used.
 * @param remaining_K The remaining part of K to factorize.
 * @param current_N The current value of N being constructed.
 * @param last_a The exponent used for the previous prime (to ensure a_i <= last_a).
 */
void dfs(int prime_idx, int remaining_K, int128 current_N, int last_a) {
    if (remaining_K == 1) {
        if (current_N < min_N) min_N = current_N;
        return;
    }
    if (prime_idx >= 25) return; // Safety limit for the number of primes used

    for (int x : divisors) {
        // Each x is a factor (a_i + 1) in the product that forms K.
        if (x > 1 && x <= remaining_K && remaining_K % x == 0) {
            int a = x - 1;
            // To minimize N, we maintain a1 >= a2 >= a3...
            if (a <= last_a) {
                int128 next_N = current_N;
                bool overflow = false;
                for (int i = 0; i < a; ++i) {
                    next_N *= (int128)primes[prime_idx];
                    if (next_N >= min_N) {
                        overflow = true;
                        break;
                    }
                }
                if (!overflow) {
                    dfs(prime_idx + 1, remaining_K / x, next_N, a);
                }
            }
        }
    }
}

int main() {
    // Faster I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> K_val)) return 0;

    // Corner case: K=1, the smallest N with 1 divisor is 1.
    if (K_val == 1) {
        cout << 1 << endl;
        return 0;
    }

    // Pre-calculate divisors of K for efficient searching during backtracking.
    for (int i = 1; i <= K_val; ++i) {
        if (K_val % i == 0) {
            divisors.push_back(i);
        }
    }
    // Sort divisors to ensure consistent DFS exploration.
    sort(divisors.begin(), divisors.end());

    // Start DFS from the first prime (2) with the initial value of N = 1.
    // The exponent of the first prime can be at most K-1.
    dfs(0, K_val, 1, K_val);

    // Check if the smallest N found fits into a 64-bit unsigned integer.
    // The problem implies that if N > 2^64-1, we should output 0.
    const unsigned long long max_u64 = 0xFFFFFFFFFFFFFFFFULL;
    if (min_N > (int128)max_u64) {
        cout << 0 << endl;
    } else {
        unsigned long long res = (unsigned long long)min_N;
        cout << res << endl;
    }

    return 0;
}

