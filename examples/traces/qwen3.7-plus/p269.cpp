#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct BigInt {
    vector<int> digits;

    BigInt(long long v = 0) {
        digits.reserve(100);
        if (v == 0) {
            digits.push_back(0);
        } else {
            while (v > 0) {
                digits.push_back(v % 1000000000);
                v /= 1000000000;
            }
        }
    }

    BigInt(const BigInt& other) : digits(other.digits) {
        digits.reserve(100);
    }

    void add(const BigInt& other) {
        int carry = 0;
        size_t n = max(digits.size(), other.digits.size());
        for (size_t i = 0; i < n || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            long long sum = digits[i] + (i < other.digits.size() ? other.digits[i] : 0) + carry;
            digits[i] = sum % 1000000000;
            carry = sum / 1000000000;
        }
    }

    void mul(long long v) {
        if (v == 0) {
            digits = {0};
            return;
        }
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

    void print() const {
        if (digits.empty()) {
            cout << 0 << "\n";
            return;
        }
        cout << digits.back();
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            cout << setfill('0') << setw(9) << digits[i];
        }
        cout << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
    }

    sort(b.begin(), b.end());

    vector<BigInt> dp(k + 1, BigInt(0));
    dp[0] = BigInt(1);

    for (int i = 0; i < n; ++i) {
        long long L = b[i];
        for (int j = k; j >= 1; --j) {
            if (L - j + 1 > 0) {
                BigInt temp = dp[j - 1];
                temp.mul(L - j + 1);
                dp[j].add(temp);
            }
        }
    }

    dp[k].print();

    return 0;
}
