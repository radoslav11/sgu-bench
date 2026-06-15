#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

/**
 * Problem Breakdown:
 * We have N digits and an array of real numbers A[i] where the fractional part
 * is an infinite decimal with digits: D[(i + 0K) % N], D[(i + 1K) % N], ...
 * 
 * Let g = gcd(K, N), then K' = K % N.
 * The sequence of indices (i + jK) % N repeats with period L = N/g.
 * The sequence of digits for a fixed i is periodic with period L.
 * 
 * For a fixed remainder r = i % g, the L sequences for different i are 
 * cyclic shifts of a base sequence C_r of length L.
 * The elements of C_r are C_r[j] = D[r + (j * m * g) % N] where m = (K' / g) % L.
 * 
 * To find the maximum A[i], we need to find the lexicographically largest cyclic shift
 * among all possible C_r sequences.
 * Once found, the first N digits are simply this sequence repeated g times.
 */

// Efficiently find the starting index of the lexicographically largest cyclic shift
// using an O(L) algorithm.
int max_cyclic_shift(const string& s) {
    int n = s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        int idx_i = (i + k >= n) ? i + k - n : i + k;
        int idx_j = (j + k >= n) ? j + k - n : j + k;
        char a = s[idx_i];
        char b = s[idx_j];
        if (a == b) {
            k++;
        } else {
            if (a < b) i += k + 1;
            else j += k + 1;
            if (i == j) j++;
            k = 0;
        }
    }
    return (i < n) ? i : j;
}

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long K;
    if (!(cin >> N >> K)) return 0;

    string D;
    if (!(cin >> D)) return 0;

    // Calculate necessary parameters
    int K_mod_N = K % N;
    
    // Standard GCD algorithm
    auto get_gcd = [](int a, int b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    };

    int g = get_gcd(K_mod_N, N);
    int L = N / g;
    int m = (L == 1) ? 0 : (int)((K_mod_N / g) % L);

    string best_C = "";

    // Iterate through each group of indices with the same remainder modulo g
    for (int r = 0; r < g; ++r) {
        string E_r(L, ' ');
        for (int j = 0; j < L; ++j) {
            E_r[j] = D[r + j * g];
        }

        string C_r(L, ' ');
        for (int j = 0; j < L; ++j) {
            // The j-th digit in the sequence is E_r[(j*m) % L]
            C_r[j] = E_r[(1LL * j * m) % L];
        }

        // Find the largest cyclic shift of C_r
        int start_idx = max_cyclic_shift(C_r);
        
        // Construct the maximized cyclic shift
        string current_best_shift = "";
        current_best_shift.reserve(L);
        for (int k = 0; k < L; ++k) {
            current_best_shift += C_r[(start_idx + k) % L];
        }

        // Track the absolute maximum across all r
        if (best_C == "" || current_best_shift > best_C) {
            best_C = current_best_shift;
        }
    }

    // The output is the best sequence of L digits repeated g times (to reach N total digits)
    string result = "";
    result.reserve(N);
    for (int i = 0; i < g; ++i) {
        result += best_C;
    }
    cout << result << endl;

    return 0;
}

