#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

cpp_int egcd(cpp_int a, cpp_int b, cpp_int &x, cpp_int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    cpp_int x1, y1;
    cpp_int g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

cpp_int mod_inverse(cpp_int a, cpp_int mod) {
    cpp_int x, y;
    egcd(a, mod, x, y);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

cpp_int ipow_int(int base, int exp) {
    cpp_int res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return res;
}

char digit_char(int d) {
    if (d < 10) return char('0' + d);
    return char('A' + d - 10);
}

string to_base(cpp_int x, int b, int n) {
    string s(n, '0');
    for (int i = n - 1; i >= 0; --i) {
        int d = (int)(x % b);
        s[i] = digit_char(d);
        x /= b;
    }
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, n;
    cin >> b >> n;

    int tmp = b;
    vector<pair<int, int>> factors;
    for (int p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            factors.push_back({p, cnt});
        }
    }
    if (tmp > 1) factors.push_back({tmp, 1});

    int r = (int)factors.size();

    vector<cpp_int> mods;
    cpp_int M = 1;

    for (auto [p, a] : factors) {
        cpp_int q = ipow_int(p, a * n);
        mods.push_back(q);
        M *= q;
    }

    vector<cpp_int> basis(r);

    for (int i = 0; i < r; ++i) {
        cpp_int mi = M / mods[i];
        cpp_int inv = mod_inverse(mi % mods[i], mods[i]);
        basis[i] = (mi * inv) % M;
    }

    cpp_int min_n_digit = 1;
    for (int i = 1; i < n; ++i) min_n_digit *= b;

    vector<string> answer;

    for (int mask = 0; mask < (1 << r); ++mask) {
        cpp_int x = 0;
        for (int i = 0; i < r; ++i) {
            if (mask & (1 << i)) {
                x += basis[i];
            }
        }
        x %= M;

        if (x >= min_n_digit) {
            answer.push_back(to_base(x, b, n));
        }
    }

    cout << answer.size() << '\n';
    for (const string &s : answer) {
        cout << s << '\n';
    }

    return 0;
}
