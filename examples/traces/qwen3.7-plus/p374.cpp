#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct BigInt {
    vector<long long> digits;
    BigInt() {
        digits.push_back(0);
    }
    BigInt(long long v) {
        if (v == 0) digits.push_back(0);
        else {
            while (v > 0) {
                digits.push_back(v % 1000000000);
                v /= 1000000000;
            }
        }
    }
    void multiply(int v) {
        long long carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            long long cur = digits[i] * v + carry;
            digits[i] = cur % 1000000000;
            carry = cur / 1000000000;
        }
        while (carry > 0) {
            digits.push_back(carry % 1000000000);
            carry /= 1000000000;
        }
    }
    void print() {
        if (digits.empty()) {
            cout << 0 << "\n";
            return;
        }
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            if (i == (int)digits.size() - 1) {
                cout << digits[i];
            } else {
                cout << setfill('0') << setw(9) << digits[i];
            }
        }
        cout << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    long long a, b, k;
    if (cin >> a >> b >> k) {
        long long base = a + b;
        BigInt ans(1);
        for (int i = 0; i < k; ++i) {
            ans.multiply(base);
        }
        ans.print();
    }
    return 0;
}
