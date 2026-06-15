#include <iostream>

using namespace std;

/**
 * Problem: 175. Encoding
 * 
 * Let phi(W) be the encoding of a word W of length N.
 * 1. If |W| = 1, phi(W) = W.
 * 2. If |W| = N > 1, let K = floor(N/2).
 *    phi(W) = phi(w_N, w_{N-1}, ..., w_{K+1}) + phi(w_K, w_{K-1}, ..., w_1).
 * 
 * The goal is to find the 1-based position of the q-th character w_q in phi(W).
 * 
 * Analysis:
 * Let f(N, q) be the 1-based position of the q-th character of a word of length N.
 * The recursion:
 * - Base case: If N = 1, f(1, 1) = 1.
 * - Let K = N / 2 (floor division).
 * - The first part of phi(W) is phi(w_N, w_{N-1}, ..., w_{K+1}).
 *   This part has a length of N - K.
 *   The characters in this part are w_{K+1}, ..., w_N.
 *   If the character we are looking for (w_q) is one of these, then q > K.
 *   In the sequence (w_N, w_{N-1}, ..., w_{K+1}), the position of w_q is:
 *   w_N is at 1, w_{N-1} is at 2, ..., w_q is at (N - q + 1).
 *   So f(N, q) = f(N - K, N - q + 1) for q > K.
 * - The second part of phi(W) is phi(w_K, w_{K-1}, ..., w_1).
 *   This part has a length of K.
 *   The characters in this part are w_1, ..., w_K.
 *   If the character we are looking for (w_q) is one of these, then q <= K.
 *   In the sequence (w_K, w_{K-1}, ..., w_1), the position of w_q is:
 *   w_K is at 1, w_{K-1} is at 2, ..., w_q is at (K - q + 1).
 *   Since this part starts after the first part, the position is:
 *   f(N, q) = (N - K) + f(K, K - q + 1) for q <= K.
 * 
 * Complexity:
 * - Time: O(log N), as the length N reduces by approximately half at each recursive step.
 * - Space: O(log N) for the recursion stack.
 */

typedef long long ll;

// Recursive function to calculate the position of character w_q in encoded phi(W) of length N
ll find_position(ll n, ll q) {
    if (n == 1) {
        return 1;
    }
    
    ll k = n / 2;
    if (q > k) {
        // The character w_q is in the first part of phi(W): phi(w_N, w_{N-1}, ..., w_{K+1})
        // Length of this part is N - K.
        // Index of w_q in the sequence (w_N, w_{N-1}, ..., w_{K+1}) is (N - q + 1).
        return find_position(n - k, n - q + 1);
    } else {
        // The character w_q is in the second part of phi(W): phi(w_K, w_{K-1}, ..., w_1)
        // Length of the first part is N - K.
        // Index of w_q in the sequence (w_K, w_{K-1}, ..., w_1) is (K - q + 1).
        return (n - k) + find_position(k, k - q + 1);
    }
}

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, q;
    if (cin >> n >> q) {
        cout << find_position(n, q) << endl;
    }

    return 0;
}

