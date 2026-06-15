#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct BigInt {
    vector<int> digits;
    BigInt() {}
    BigInt(long long v) {
        if (v == 0) {
            digits.push_back(0);
        } else {
            while (v > 0) {
                digits.push_back(v % 1000000000);
                v /= 1000000000;
            }
        }
    }
};

BigInt add(const BigInt& a, const BigInt& b) {
    BigInt res;
    int carry = 0;
    int n = max(a.digits.size(), b.digits.size());
    for (int i = 0; i < n || carry; ++i) {
        long long sum = carry;
        if (i < a.digits.size()) sum += a.digits[i];
        if (i < b.digits.size()) sum += b.digits[i];
        res.digits.push_back(sum % 1000000000);
        carry = sum / 1000000000;
    }
    if (res.digits.empty()) {
        res.digits.push_back(0);
    }
    return res;
}

void print(const BigInt& a) {
    if (a.digits.empty()) {
        cout << 0 << "\n";
        return;
    }
    cout << a.digits.back();
    for (int i = (int)a.digits.size() - 2; i >= 0; --i) {
        cout << setw(9) << setfill('0') << a.digits[i];
    }
    cout << "\n";
}

BigInt dp[1005];

void precompute() {
    dp[0] = BigInt(1);
    dp[1] = BigInt(1);
    dp[2] = BigInt(2);
    dp[3] = BigInt(3);
    for (int i = 4; i <= 1000; ++i) {
        dp[i] = add(dp[i-1], dp[i-3]);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    precompute();
    int m, n;
    if (cin >> m >> n) {
        if (m % 2 != 0 && n % 2 != 0) {
            cout << 0 << "\n";
        } else if (m == 1 || n == 1) {
            cout << 1 << "\n";
        } else if (m == 2 || n == 2) {
            print(dp[max(m, n)]);
        } else if (m == 3 || n == 3) {
            print(dp[max(m, n)]);
        } else {
            if (m % 2 == 0 && n % 2 == 0) {
                cout << 2 << "\n";
            } else {
                cout << 0 << "\n";
            }
        }
    }
    return 0;
}
