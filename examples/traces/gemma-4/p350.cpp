#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * Problem Analysis:
 * Let A = {A_1, A_2, ..., A_n} be the set of n integers.
 * The given set B contains all pairwise XORs A_i ^ A_j for 1 <= i < j <= n.
 * The size of B is M = n(n-1)/2.
 * Since no subset of A with size >= 2 has XOR sum zero, it implies that the
 * elements are "almost" linearly independent. Specifically, if we pick 
 * A_1 = 0, then {A_2, A_3, ..., A_n} must be linearly independent over GF(2).
 * 
 * If we assume A_1 = 0, then the set B is exactly {A_i | i = 2...n} union
 * {A_i ^ A_j | 2 <= i < j <= n}.
 * This means that the elements A_2, ..., A_n are n-1 elements in B such that
 * all their pairwise XORs are also in B.
 * 
 * Since n is small (n <= 14 for M <= 100), we can use backtracking to find 
 * these n-1 elements from B. We prune the search by ensuring each new element
 * added to our candidate set X has its XOR with every current element in X 
 * also present in B.
 */

int M, N;
vector<long long> B;
vector<long long> X;

/**
 * Checks if the given elements are linearly independent over GF(2)
 * using Gaussian elimination (basis method).
 */
bool is_linearly_independent(const vector<long long>& elements) {
    vector<long long> basis;
    for (long long x : elements) {
        for (long long b : basis) {
            x = min(x, x ^ b);
        }
        if (x > 0) {
            basis.push_back(x);
            // Keep basis elements in descending order to simplify the bitwise XOR reduction
            sort(basis.rbegin(), basis.rend());
        } else {
            return false;
        }
    }
    return true;
}

/**
 * Backtracking function to find a subset of B of size n-1 such that 
 * all pairwise XORs are in B.
 * 
 * @param start_idx: The index in B to start searching from.
 * @param count: The number of elements currently in X.
 * @return true if a valid subset is found, false otherwise.
 */
bool backtrack(int start_idx, int count) {
    // Base case: we have found n-1 elements
    if (count == N - 1) {
        // Check if all pairwise XORs (X_i ^ X_j) are in B
        for (int i = 0; i < count; ++i) {
            for (int j = i + 1; j < count; ++j) {
                if (!binary_search(B.begin(), B.end(), X[i] ^ X[j])) return false;
            }
        }
        // The condition for the set A={0, X_2, ..., X_n} is that X_2...X_n are linearly independent
        return is_linearly_independent(X);
    }

    for (int i = start_idx; i < M; ++i) {
        long long x = B[i];
        bool ok = true;
        // Pruning: check if (x ^ existing_element) is in B for all existing elements
        for (int j = 0; j < count; ++j) {
            if (!binary_search(B.begin(), B.end(), x ^ X[j])) {
                ok = false;
                break;
            }
        }
        if (ok) {
            X.push_back(x);
            if (backtrack(i + 1, count + 1)) return true;
            X.pop_back();
        }
    }
    return false;
}

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> M)) return 0;
    B.resize(M);
    for (int i = 0; i < M; ++i) {
        cin >> B[i];
    }
    // Sorting B is necessary for binary_search
    sort(B.begin(), B.end());

    // Solving M = n(n-1)/2 for n: n^2 - n - 2M = 0
    // n = (1 + sqrt(1 + 8M)) / 2
    N = (int)((1.0 + sqrt(1.0 + 8.0 * M)) / 2.0 + 0.5);

    // Corner case: n = 2
    if (N == 2) {
        // If n=2, B = {A_1 ^ A_2}. Since we can pick any solution, 
        // we pick A_1 = 0 and A_2 = B[0].
        cout << 0 << " " << B[0] << endl;
        return 0;
    }

    // General case: Try all possible starting elements in B for X
    for (int i = 0; i < M; ++i) {
        X.clear();
        X.push_back(B[i]);
        if (backtrack(i + 1, 1)) {
            // If found, output the resulting set A = {0, X_1, ..., X_{n-1}}
            cout << 0;
            for (long long x : X) cout << " " << x;
            cout << endl;
            return 0;
        }
    }

    return 0;
}

