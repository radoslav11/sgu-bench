#include <iostream>
#include <vector>

using namespace std;

/**
 * The problem asks for the number of "extremal permutations" of integers 1 to n.
 * An extremal permutation is a zigzag (alternating) permutation.
 * The number of such permutations is related to the Euler zigzag numbers E_n.
 * For n >= 2, the total number of alternating permutations is 2 * E_n.
 * For n = 1, the number is 1.
 * The Euler zigzag numbers E_n satisfy the recurrence:
 * 2 * E_{n+1} = sum_{k=0}^n (binom(n, k) * E_k * E_{n-k})
 * 
 * Given n up to 10^6 (estimated) and m up to 10^9, and a time limit of 0.5s,
 * if n is large, an O(n log n) solution using NTT and CRT would be required.
 * However, if n is small (e.g., n <= 5000), an O(n^2) solution is more plausible.
 * Since the time limit is very strict, an O(n^2) approach with symmetry 
 * to reduce complexity is used.
 */

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, m;
    if (!(cin >> n >> m)) return 0;

    // Base cases for small n
    if (n == 1) {
        cout << 1 % m << endl;
        return 0;
    }
    if (n == 2) {
        cout << 2 % m << endl;
        return 0;
    }

    /**
     * To compute E_{i+1} modulo M (where M = 2m), we can use the recurrence
     * 2 * E_{i+1} = sum_{k=0}^i (binom(i, k) * E_k * E_{i-k}).
     * To handle the division by 2, we compute the sum modulo 2M (where 2M = 4m).
     * Let S be the sum modulo 4m. Then E_{i+1} % (2m) = (S / 2).
     */
    long long M = 2 * m;
    long long MOD = 4 * m;

    // E[i] will store the i-th Euler number modulo M.
    // Using a vector for E is safe up to n=10^6 given the 256MB limit.
    // However, O(n^2) is only feasible for n up to ~5000-8000.
    vector<long long> E(n + 1);
    // C[k] will store the current binomial coefficient binom(i, k) modulo MOD.
    vector<long long> C(n + 1, 0);

    E[0] = 1 % M;
    E[1] = 1 % M;
    C[0] = 1;

    for (int i = 1; i < n; ++i) {
        // Update binomial coefficients from binom(i-1, k) to binom(i, k)
        for (int k = i; k >= 1; --k) {
            C[k] = (C[k] + C[k - 1]) % MOD;
        }

        __int128 S = 0; // Sum modulo 4m
        if (i % 2 != 0) {
            // Case i is odd: binom(i, k) * E_k * E_{i-k} is symmetric and
            // k and i-k are never equal because i is odd.
            for (int k = 0; k <= i / 2; ++k) {
                __int128 term = (__int128)C[k] * E[k] % MOD * E[i - k] % MOD;
                S = (S + 2 * term) % MOD;
            }
        } else {
            // Case i is even: binom(i, k) * E_k * E_{i-k} is symmetric,
            // and there's a middle term where k = i-k = i/2.
            for (int k = 0; k < i / 2; ++k) {
                __int128 term = (__int128)C[k] * E[k] % MOD * E[i - k] % MOD;
                S = (S + 2 * term) % MOD;
            }
            __int128 term_mid = (__int128)C[i / 2] * E[i / 2] % MOD * E[i / 2] % MOD;
            S = (S + term_mid) % MOD;
        }
        // E[i+1] % M = (S % 4m) / 2
        E[i + 1] = (long long)(S / 2);
    }

    // The total number of extremal permutations is 2 * E_n (for n >= 2).
    // We have E[n] modulo 2m.
    // Result: (2 * E[n]) % m = (2 * (E[n] % 2m)) % m.
    long long ans = (2 * E[n]) % m;
    cout << ans << endl;

    return 0;
}

