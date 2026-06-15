#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

/**
 * BigInt structure to handle the potentially large value of 2^(m-k) - 1.
 * Given m <= 100, 2^100 - 1 can have up to 31 digits.
 */
struct BigInt {
    vector<int> digits;
    BigInt(long long n = 0) {
        if (n == 0) digits.push_back(0);
        while (n > 0) {
            digits.push_back(n % 10);
            n /= 10;
        }
    }

    // Multiplies the current BigInt by 2.
    void multiplyBy2() {
        int carry = 0;
        for (int i = 0; i < (int)digits.size(); ++i) {
            int val = digits[i] * 2 + carry;
            digits[i] = val % 10;
            carry = val / 10;
        }
        if (carry) digits.push_back(carry);
    }

    // Subtracts 1 from the current BigInt.
    void subtractOne() {
        int i = 0;
        while (i < (int)digits.size() && digits[i] == 0) {
            digits[i] = 9;
            i++;
        }
        if (i < (int)digits.size()) {
            digits[i]--;
        }
        // Remove leading zeros.
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    // Prints the BigInt.
    void print() {
        if (digits.empty()) {
            cout << 0 << endl;
            return;
        }
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            cout << digits[i];
        }
        cout << endl;
    }
};

int main() {
    // Optimizing standard I/O for performance.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t, m;
    if (!(cin >> t >> m)) return 0;
    vector<long long> b(m);
    for (int i = 0; i < m; ++i) cin >> b[i];

    // Precompute the first t primes.
    // Since t <= 100, the 100th prime is 541. A sieve up to 1000 is sufficient.
    vector<int> primes;
    vector<bool> is_prime(1000, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p < 1000; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
            if ((int)primes.size() == t) break;
            for (int i = p * p; i < 1000; i += p)
                is_prime[i] = false;
        }
    }

    // Each number b[i] is mapped to a binary vector representing the parity of its prime factor exponents.
    // Using bitset for efficient matrix operations in Gaussian elimination.
    vector<bitset<100>> matrix(m);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < t; ++j) {
            int count = 0;
            while (b[i] % primes[j] == 0) {
                count++;
                b[i] /= primes[j];
            }
            if (count % 2 != 0) {
                matrix[i].set(j);
            }
        }
    }

    // Find the rank of the m x t matrix over GF(2) using Gaussian elimination.
    int rank = 0;
    for (int j = 0; j < t && rank < m; ++j) {
        int pivot = -1;
        for (int i = rank; i < m; ++i) {
            if (matrix[i].test(j)) {
                pivot = i;
                break;
            }
        }
        if (pivot != -1) {
            swap(matrix[rank], matrix[pivot]);
            for (int i = rank + 1; i < m; ++i) {
                if (matrix[i].test(j)) {
                    matrix[i] ^= matrix[rank];
                }
            }
            rank++;
        }
    }

    // The number of subsets whose product is a perfect square is 2^(m-rank).
    // Subtracting 1 to exclude the empty subset.
    BigInt res(1);
    for (int i = 0; i < m - rank; ++i) {
        res.multiplyBy2();
    }
    res.subtractOne();
    res.print();

    return 0;
}

