#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

long long modpow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b, c;
    cin >> a >> b >> c;

    int n = (int)a.size();

    int same = 0;
    int p = 0, q = 0, r = 0;
    // p: b == c != a
    // q: a == c != b
    // r: a == b != c

    for (int i = 0; i < n; ++i) {
        if (a[i] == b[i] && b[i] == c[i]) {
            same++;
        } else if (b[i] == c[i]) {
            p++;
        } else if (a[i] == c[i]) {
            q++;
        } else {
            r++;
        }
    }

    vector<long long> fact(n + 1), invfact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % MOD;

    invfact[n] = modpow(fact[n], MOD - 2);
    for (int i = n; i >= 1; --i) invfact[i - 1] = invfact[i] * i % MOD;

    auto C = [&](int nn, int kk) -> long long {
        if (kk < 0 || kk > nn) return 0;
        return fact[nn] * invfact[kk] % MOD * invfact[nn - kk] % MOD;
    };

    auto ways = [&](int m, int sum) -> long long {
        if (abs(sum) > m) return 0;
        if ((m + sum) & 1) return 0;
        return C(m, (m + sum) / 2);
    };

    long long ans = 0;

    if ((p & 1) == (q & 1) && (q & 1) == (r & 1)) {
        int lim = min({p, q, r});
        int parity = p & 1;

        for (int s = -lim; s <= lim; ++s) {
            int curParity = ((s % 2) + 2) % 2;
            if (curParity != parity) continue;

            long long add = ways(p, s);
            add = add * ways(q, s) % MOD;
            add = add * ways(r, s) % MOD;
            ans = (ans + add) % MOD;
        }
    }

    ans = ans * modpow(2, same) % MOD;
    cout << ans << '\n';

    return 0;
}
