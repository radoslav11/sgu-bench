#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    static const int BASE = 1000000000;
    vector<int> d;

    BigInt(long long x = 0) {
        if (x == 0) d.push_back(0);
        while (x > 0) {
            d.push_back(x % BASE);
            x /= BASE;
        }
    }

    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
    }

    void multiply(int m) {
        long long carry = 0;
        for (int &x : d) {
            long long cur = 1LL * x * m + carry;
            x = cur % BASE;
            carry = cur / BASE;
        }
        while (carry) {
            d.push_back(carry % BASE);
            carry /= BASE;
        }
        trim();
    }

    static int compare(const BigInt& a, const BigInt& b) {
        if (a.d.size() != b.d.size())
            return a.d.size() < b.d.size() ? -1 : 1;
        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            if (a.d[i] != b.d[i])
                return a.d[i] < b.d[i] ? -1 : 1;
        }
        return 0;
    }

    static BigInt subtract(BigInt a, const BigInt& b) {
        long long borrow = 0;
        for (size_t i = 0; i < a.d.size(); ++i) {
            long long cur = 1LL * a.d[i] - borrow - (i < b.d.size() ? b.d[i] : 0);
            if (cur < 0) {
                cur += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            a.d[i] = cur;
        }
        a.trim();
        return a;
    }

    string toString() const {
        string s = to_string(d.back());
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            string part = to_string(d[i]);
            s += string(9 - part.length(), '0') + part;
        }
        return s;
    }
};

BigInt power(int base, int exp) {
    BigInt res(1);
    for (int i = 0; i < exp; ++i)
        res.multiply(base);
    return res;
}

int main() {
    int a, b;
    cin >> a >> b;

    BigInt x = power(a, b);
    BigInt y = power(b, a);

    int cmp = BigInt::compare(x, y);

    if (cmp == 0) {
        cout << 0 << '\n';
    } else if (cmp > 0) {
        cout << BigInt::subtract(x, y).toString() << '\n';
    } else {
        cout << "-" << BigInt::subtract(y, x).toString() << '\n';
    }

    return 0;
}
