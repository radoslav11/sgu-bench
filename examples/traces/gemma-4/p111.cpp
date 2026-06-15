#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * BigInt structure to handle large integer arithmetic needed for the 
 * digit-by-digit square root algorithm.
 * Digits are stored in a vector where digits[0] is the least significant digit.
 */
struct BigInt {
    vector<int> digits;

    BigInt() {}
    BigInt(long long v) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % 10);
            v /= 10;
        }
    }

    void multiply(int d) {
        if (d == 0) {
            digits.assign(1, 0);
            return;
        }
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            int val = digits[i] * d + carry;
            digits[i] = val % 10;
            carry = val / 10;
        }
        while (carry) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    void add(int d) {
        int carry = d;
        for (size_t i = 0; i < digits.size() && carry; ++i) {
            int val = digits[i] + carry;
            digits[i] = val % 10;
            carry = val / 10;
        }
        while (carry) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    void multiplyBy10() {
        if (digits.size() == 1 && digits[0] == 0) return;
        digits.insert(digits.begin(), 0);
    }

    void multiplyBy100() {
        if (digits.size() == 1 && digits[0] == 0) return;
        digits.insert(digits.begin(), 0);
        digits.insert(digits.begin(), 0);
    }

    void subtract(const BigInt& other) {
        int borrow = 0;
        for (size_t i = 0; i < other.digits.size() || borrow; ++i) {
            int sub = (i < other.digits.size() ? other.digits[i] : 0) + borrow;
            if (digits[i] < sub) {
                digits[i] = digits[i] + 10 - sub;
                borrow = 1;
            } else {
                digits[i] = digits[i] - sub;
                borrow = 0;
            }
        }
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    bool lessThanOrEqual(const BigInt& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i];
        }
        return true;
    }

    void print() const {
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            cout << digits[i];
        }
        cout << endl;
    }
};

/**
 * The digit-by-digit square root method (similar to long division)
 * complexity is O(D^2) where D is the number of digits.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    // Pad the input string to ensure it has an even number of digits.
    if (s.length() % 2 != 0) {
        s = "0" + s;
    }

    BigInt P(0); // Current root being built.
    BigInt R(0); // Current remainder.

    for (int i = 0; i < (int)s.length(); i += 2) {
        int D = (s[i] - '0') * 10 + (s[i+1] - '0');
        
        // Append the current pair of digits to the remainder.
        R.multiplyBy100();
        R.add(D);

        // Pre-calculate 20 * P to save time in the inner loop.
        BigInt P20 = P;
        P20.multiply(20);

        int best_d = 0;
        BigInt best_term(0);
        
        // Try digits 1-9 to see which one works for the next digit of the square root.
        for (int d = 1; d <= 9; ++d) {
            BigInt temp = P20;
            temp.add(d);
            temp.multiply(d);
            if (temp.lessThanOrEqual(R)) {
                best_d = d;
                best_term = temp;
            } else {
                break;
            }
        }

        // Update the remainder and the current root.
        R.subtract(best_term);
        P.multiplyBy10();
        P.add(best_d);
    }

    P.print();

    return 0;
}

