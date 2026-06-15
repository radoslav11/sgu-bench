#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Find a^b - b^a for given natural numbers a and b (1 <= a, b <= 100).
 * Since a^b can be as large as 100^100 = 10^200, we need a BigInt implementation.
 * The constraints and the sample input (a=2, b=3 => -1) indicate the goal is a^b - b^a.
 */

struct BigInt {
    vector<int> d;

    // Constructor for a long long number
    BigInt(long long n = 0) {
        if (n == 0) {
            d.push_back(0);
        } else {
            if (n < 0) n = -n; // Not used in the current logic but good for robustness
            while (n > 0) {
                d.push_back(n % 10);
                n /= 10;
            }
        }
    }

    // Multiply this BigInt by an integer a
    void multiply(int a) {
        int carry = 0;
        for (size_t i = 0; i < d.size(); i++) {
            long long val = (long long)d[i] * a + carry;
            d[i] = (int)(val % 10);
            carry = (int)(val / 10);
        }
        while (carry) {
            d.push_back(carry % 10);
            carry /= 10;
        }
    }

    // Compare two BigInts
    bool operator<(const BigInt& other) const {
        if (d.size() != other.d.size()) return d.size() < other.d.size();
        for (int i = (int)d.size() - 1; i >= 0; i--) {
            if (d[i] != other.d[i]) return d[i] < other.d[i];
        }
        return false;
    }

    bool operator>(const BigInt& other) const { return other < *this; }
    bool operator==(const BigInt& other) const { return !(*this < other) && !(other < *this); }

    // Subtract another BigInt from this one. Assumes *this >= other.
    BigInt operator-(const BigInt& other) const {
        BigInt res = *this;
        int borrow = 0;
        int n = (int)res.d.size();
        int m = (int)other.d.size();
        for (int i = 0; i < m || borrow; i++) {
            int sub = (i < m ? other.d[i] : 0) + borrow;
            if (res.d[i] < sub) {
                res.d[i] = res.d[i] + 10 - sub;
                borrow = 1;
            } else {
                res.d[i] -= sub;
                borrow = 0;
            }
        }
        // Remove trailing zeros to normalize representation
        while (res.d.size() > 1 && res.d.back() == 0) {
            res.d.pop_back();
        }
        return res;
    }

    // Print the BigInt
    void print() const {
        for (int i = (int)d.size() - 1; i >= 0; i--) {
            cout << d[i];
        }
        cout << endl;
    }
};

/**
 * Computes a^b for integers a and b using BigInt multiplication.
 * Since b <= 100, simple multiplication is efficient enough.
 */
BigInt power(int a, int b) {
    BigInt res(1);
    for (int i = 0; i < b; i++) {
        res.multiply(a);
    }
    return res;
}

int main() {
    // Set up fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int a, b;
    if (!(cin >> a >> b)) return 0;

    // Compute a^b and b^a
    BigInt A = power(a, b);
    BigInt B = power(b, a);

    // The result of a^b - b^a
    if (A == B) {
        cout << 0 << endl;
    } else if (A > B) {
        (A - B).print();
    } else {
        cout << "-";
        (B - A).print();
    }

    return 0;
}

