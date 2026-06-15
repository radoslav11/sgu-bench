#include <iostream>

using namespace std;

/**
 * Problem: Little Rooks
 * Given an n x n chessboard, find the number of ways to place k rooks
 * such that no two rooks attack each other. Two rooks attack each other 
 * if they are in the same row or the same column.
 * 
 * Constraints:
 * 1 <= n <= 10
 * 0 <= k <= n^2
 * Time limit: 0.25 sec
 * 
 * Logic:
 * For rooks to not attack each other, each rook must occupy a unique row 
 * and a unique column. This implies:
 * 1. If k > n, it is impossible to place k rooks without at least two 
 *    sharing a row or a column. In this case, the answer is 0.
 * 2. If k <= n, we can follow these steps:
 *    a. Select k rows out of n: C(n, k) ways.
 *    b. Select k columns out of n: C(n, k) ways.
 *    c. Within the chosen k rows and k columns (forming a k x k subgrid),
 *       place the k rooks such that each of the k rows and k columns 
 *       contains exactly one rook. This is equivalent to placing rooks 
 *       in a permutation of k elements, which results in k! ways.
 *    Total ways = C(n, k) * C(n, k) * k!
 * 
 * Complexity:
 * Time: O(n), which is very efficient for n <= 10.
 * Space: O(1).
 */

/**
 * Calculates the combination C(n, k) using the iterative formula:
 * C(n, k) = C(n, k-1) * (n-k+1) / k.
 */
long long combinations(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n / 2) k = n - k; // Use the symmetry property C(n, k) = C(n, n-k)
    
    long long res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i;
    }
    return res;
}

/**
 * Calculates the factorial k!.
 */
long long factorial(int k) {
    long long res = 1;
    for (int i = 1; i <= k; ++i) {
        res *= i;
    }
    return res;
}

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    // Read n and k from standard input
    if (!(cin >> n >> k)) {
        return 0;
    }

    // If k exceeds n, it's impossible to place rooks without overlap in row or column
    if (k > n) {
        cout << 0 << endl;
    } else {
        // C(n, k) ways to pick k rows
        long long comb_n_k = combinations(n, k);
        // k! ways to arrange rooks in chosen rows/cols
        long long fact_k = factorial(k);
        
        // The final result: (C(n, k) * C(n, k)) * k!
        // For n=10, the maximum possible value is C(10, 8)^2 * 8! = 81,648,000,
        // which fits easily in a long long (and even a standard 32-bit int).
        long long result = comb_n_k * comb_n_k * fact_k;
        
        cout << result << endl;
    }

    return 0;
}

