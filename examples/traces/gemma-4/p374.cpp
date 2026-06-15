#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * We need to find the sum of coefficients of the polynomial Q(x) = (ax + b)^k.
 * Let Q(x) = c_m*x^m + c_{m-1}*x^{m-1} + ... + c_1*x + c_0.
 * The sum of the coefficients is Q(1) = c_m + c_{m-1} + ... + c_0.
 * For Q(x) = (ax + b)^k, Q(1) = (a(1) + b)^k = (a + b)^k.
 *
 * Constraints:
 * 1 <= a, b <= 100, 1 <= k <= 20.
 * The maximum value of (a + b)^k is (100 + 100)^20 = 200^20.
 * 200^20 = (2 * 10^2)^20 = 2^20 * 10^40 = 1,048,576 * 10^40 approx 1.048 * 10^46.
 * This value is larger than a 64-bit integer (approx 9 * 10^18) and even a 128-bit integer (approx 3.4 * 10^38).
 * Therefore, we need to use a BigInt implementation to compute the result.
 */

struct BigInt {
    vector<long long> d;
    static const long long BASE = 1000000000; // Using base 10^9 for efficiency

    // Initialize BigInt with a long long value
    BigInt(long long v = 0) {
        if (v == 0) {
            d.push_back(0);
        } else {
            while (v > 0) {
                d.push_back(v % BASE);
                v /= BASE;
            }
        }
    }

    // Multiply the current BigInt by a long long integer
    void multiply(long long v) {
        if (v == 0) {
            d.clear();
            d.push_back(0);
            return;
        }
        long long carry = 0;
        for (size_t i = 0; i < d.size() || carry; ++i) {
            if (i == d.size()) d.push_back(0);
            // d[i] is up to 10^9, v is up to 200, carry is up to 200.
            // Their product and sum will fit in long long.
            long long cur = d[i] * v + carry;
            d[i] = cur % BASE;
            carry = cur / BASE;
        }
        // Clean up leading zeros if any (though multiply here won't create them)
        while (d.size() > 1 && d.back() == 0) d.pop_back();
    }

    // Print the BigInt in base-10 representation
    void print() const {
        if (d.empty()) {
            cout << 0 << endl;
            return;
        }
        // Print the most significant digit first
        cout << d.back();
        // Print subsequent digits with leading zeros to match the BASE
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << d[i];
        }
        cout << endl;
    }
};

int main() {
    // Improve I/O speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long a, b, k;
    if (!(cin >> a >> b >> k)) return 0;

    // The result is simply (a + b)^k
    long long base_val = a + b;
    BigInt result(1);
    for (int i = 0; i < k; ++i) {
        result.multiply(base_val);
    }

    // Output the calculated sum of coefficients
    result.print();

    return 0;
}

