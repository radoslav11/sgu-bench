#include <iostream>
#include <vector>

/**
 * Problem Analysis:
 * The digital root (DR) of a positive integer n is its value modulo 9, 
 * but with the result being 9 if n % 9 == 0. 
 * For n = 0, the digital root is 0.
 * 
 * The expression given is S = A1*A2*...*AN + A1*A2*...*AN-1 + ... + A1*A2 + A1.
 * Let S_i = A1 * A2 * ... * Ai. Then S = S1 + S2 + ... + SN.
 * 
 * Since digital root properties hold for both addition and multiplication:
 * DR(S) is equivalent to finding S mod 9 and applying the rule:
 * - If S % 9 == 0 and S > 0, DR(S) = 9.
 * - If S % 9 != 0, DR(S) = S % 9.
 * - If S == 0, DR(S) = 0.
 * 
 * Given A_i are non-negative, S = 0 if and only if A1 = 0. 
 * If A1 > 0, then S > 0.
 * 
 * Complexity:
 * Time complexity: O(K * N), where K is the number of test cases and N is the size of array A.
 * Space complexity: O(N) to store the input array.
 */

using namespace std;

void solve() {
    int N;
    if (!(cin >> N)) return;
    
    vector<long long> A(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
    }

    // If A1 is 0, then every term in the sum A1 + A1*A2 + ... + A1*A2*...*AN will be 0.
    // Therefore S = 0, and its digital root is 0.
    if (A[0] == 0) {
        cout << 0 << endl;
        return;
    }

    // Calculate S modulo 9.
    // We compute the sum of the modular products: S_mod_9 = (sum_{i=1}^{N} (product_{j=1}^{i} A_j)) mod 9.
    long long S_mod_9 = 0;
    long long current_prod_mod_9 = 1;
    for (int i = 0; i < N; ++i) {
        // Update current product modulo 9: (current_prod * A[i]) % 9
        current_prod_mod_9 = (current_prod_mod_9 * (A[i] % 9)) % 9;
        // Accumulate current product into the sum modulo 9
        S_mod_9 = (S_mod_9 + current_prod_mod_9) % 9;
    }

    // Handle the special case where S % 9 == 0.
    // Since we already checked A[0] == 0, we know S > 0.
    // For a positive integer S, if S % 9 == 0, the digital root is 9.
    if (S_mod_9 == 0) {
        cout << 9 << endl;
    } else {
        cout << S_mod_9 << endl;
    }
}

int main() {
    // Speed up I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    if (!(cin >> K)) return 0;
    while (K--) {
        solve();
    }
    return 0;
}

