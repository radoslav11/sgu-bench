// Independent oracle for p374: sum of coefficients of (ax+b)^k equals
// (a+b)^k evaluated at x=1; computed with a decimal big integer since
// 200^20 overflows 64-bit (and even 128-bit) integers.
#include <bits/stdc++.h>
using namespace std;

int main() {
    long long a, b, k;
    cin >> a >> b >> k;

    long long base = a + b;
    vector<int> digits = {1};
    for (int step = 0; step < k; step++) {
        long long carry = 0;
        for (size_t i = 0; i < digits.size(); i++) {
            long long cur = (long long)digits[i] * base + carry;
            digits[i] = cur % 10;
            carry = cur / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    string out;
    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        out += char('0' + digits[i]);
    }
    cout << out << endl;
    return 0;
}
