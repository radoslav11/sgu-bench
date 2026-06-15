#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod = 1000000007;

ll modpow(ll base, ll exp, ll modulus) {
    base %= modulus;
    ll res = 1;
    while (exp > 0) {
        if (exp & 1) 
            res = (res * base) % modulus;
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return res;
}

int main() {
    string a, b, c;
    getline(cin, a);
    getline(cin, b);
    getline(cin, c);

    // Remove carriage returns if present (for online judges, usually not needed but safe)
    if (!a.empty() && a.back() == '\r') a.pop_back();
    if (!b.empty() && b.back() == '\r') b.pop_back();
    if (!c.empty() && c.back() == '\r') c.pop_back();

    int n = a.size();

    int I_count = 0, J_count = 0, K_count = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i] && a[i] != c[i]) {
            I_count++;
        } else if (a[i] != b[i] && a[i] == c[i]) {
            J_count++;
        } else if (a[i] == b[i] && a[i] != c[i]) {
            K_count++;
        }
    }

    // Check parity condition
    if ((I_count % 2) != (J_count % 2) || (I_count % 2) != (K_count % 2)) {
        cout << 0 << endl;
        return 0;
    }

    // Precompute factorials and inverse factorials up to n
    vector<ll> fact(n + 1), inv_fact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % mod;
    }
    
    inv_fact[n] = modpow(fact[n], mod - 2, mod);
    for (int i = n; i > 0; i--) {
        inv_fact[i - 1] = inv_fact[i] * i % mod;
    }

    // Binomial coefficient function
    auto C = [&](int nn, int kk) -> ll {
        if (kk < 0 || kk > nn) return 0;
        return fact[nn] * inv_fact[kk] % mod * inv_fact[nn - kk] % mod;
    };

    ll ans = 0;
    int s_min = max({-I_count, -J_count, -K_count});
    int s_max = min({I_count, J_count, K_count});

    // Iterate over possible s values (step by 2 to maintain parity)
    for (int s = s_min; s <= s_max; s += 2) {
        int i1 = (I_count - s) / 2;
        int j1 = (J_count + s) / 2;
        int k1 = (K_count + s) / 2;
        
        ll term = C(I_count, i1);
        term = term * C(J_count, j1) % mod;
        term = term * C(K_count, k1) % mod;
        ans = (ans + term) % mod;
    }

    cout << ans << endl;
    return 0;
}
