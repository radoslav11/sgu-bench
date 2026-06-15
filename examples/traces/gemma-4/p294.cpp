#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * Problem Analysis:
 * The number of distinct circular strings of length n with 2 possible characters is given by 
 * Burnside's Lemma. For the cyclic group of order n acting on the set of 2^n strings,
 * the number of distinct orbits is:
 * N = (1/n) * sum_{d|n} (phi(n/d) * 2^d)
 * Alternatively, N = (1/n) * sum_{d|n} (phi(d) * 2^(n/d))
 * 
 * Given n <= 200,000, 2^n can be very large (up to ~60,206 decimal digits).
 * We need a BigInt implementation to compute the sum and divide by n.
 * 
 * The sum can be computed by iterating through all divisors of n, calculating 2^d,
 * multiplying by phi(n/d), and summing them up. To avoid repeated large multiplications,
 * we calculate 2^d by incrementally multiplying 2^current_d by 2 until the next divisor.
 */

const int BASE = 1000000000;

struct BigInt {
    vector<int> digits;

    // Initialize with a small integer
    BigInt(long long v = 0) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % BASE);
            v /= BASE;
        }
    }

    // Multiply the current BigInt by 2
    void multiply2() {
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            long long val = (long long)digits[i] * 2 + carry;
            digits[i] = (int)(val % BASE);
            carry = (int)(val / BASE);
        }
        if (carry) digits.push_back(carry);
    }

    // Multiply the current BigInt by a small integer v
    void multiply(int v) {
        if (v == 1) return;
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            long long val = (long long)digits[i] * v + carry;
            digits[i] = (int)(val % BASE);
            carry = (int)(val / BASE);
        }
        while (carry) {
            digits.push_back(carry % BASE);
            carry /= BASE;
        }
    }

    // Add another BigInt to this one
    void add(const BigInt& other) {
        int carry = 0;
        size_t max_size = max(digits.size(), other.digits.size());
        for (size_t i = 0; i < max_size || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            long long val = (long long)digits[i] + (i < other.digits.size() ? other.digits[i] : 0) + carry;
            digits[i] = (int)(val % BASE);
            carry = (int)(val / BASE);
        }
    }

    // Divide the current BigInt by a small integer v
    void divide(int v) {
        long long rem = 0;
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            long long cur = digits[i] + rem * BASE;
            digits[i] = (int)(cur / v);
            rem = cur % v;
        }
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }

    // Print the BigInt in decimal
    void print() {
        if (digits.empty()) {
            cout << 0 << endl;
            return;
        }
        cout << digits.back();
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << digits[i];
        }
        cout << endl;
    }
};

const int MAXN = 200005;
int phi[MAXN];

// Precompute Euler's totient function up to MAXN
void sieve_phi(int n) {
    for (int i = 0; i <= n; ++i) phi[i] = i;
    for (int i = 2; i <= n; ++i) {
        if (phi[i] == i) {
            for (int j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }
}

int main() {
    // Optimization for standard input/output
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    sieve_phi(n);

    vector<int> divisors;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i * i != n) divisors.push_back(n / i);
        }
    }
    sort(divisors.begin(), divisors.end());

    BigInt totalSum(0);
    BigInt current_2_power(1);
    int current_d = 0;

    for (int d : divisors) {
        // Compute 2^d incrementally to avoid expensive exponentiation
        while (current_d < d) {
            current_2_power.multiply2();
            current_d++;
        }
        // Add phi(n/d) * 2^d to totalSum
        BigInt term = current_2_power;
        term.multiply(phi[n / d]);
        totalSum.add(term);
    }

    // Final result is totalSum / n
    totalSum.divide(n);
    totalSum.print();

    return 0;
}

