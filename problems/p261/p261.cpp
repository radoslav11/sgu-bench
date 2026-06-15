#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int64_t p, k, a;

void read() { cin >> p >> k >> a; }

int64_t mul_mod(int64_t x, int64_t y, int64_t mod) {
    return (int64_t)((__int128)x * y % mod);
}

int64_t ext_gcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int64_t x1, y1;
    int64_t g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int64_t inv_mod(int64_t a, int64_t m) {
    int64_t x, y;
    int64_t g = ext_gcd(((a % m) + m) % m, m, x, y);
    if(g != 1) {
        return -1;
    }
    return ((x % m) + m) % m;
}

int64_t mod_exp(int64_t base, int64_t exp, int64_t mod) {
    int64_t result = 1;
    base %= mod;
    if(base < 0) {
        base += mod;
    }

    while(exp > 0) {
        if(exp & 1) {
            result = mul_mod(result, base, mod);
        }
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

vector<int64_t> factorize(int64_t n) {
    vector<int64_t> factors;
    for(int64_t i = 2; i * i <= n; i++) {
        if(n % i == 0) {
            factors.push_back(i);
            while(n % i == 0) {
                n /= i;
            }
        }
    }
    if(n > 1) {
        factors.push_back(n);
    }
    return factors;
}

int64_t find_primitive_root(int64_t mod) {
    if(mod == 2) {
        return 1;
    }

    int64_t phi = mod - 1;
    vector<int64_t> prime_factors = factorize(phi);
    for(int64_t g = 2; g < 200000; g++) {
        bool ok = true;
        for(int64_t f: prime_factors) {
            if(mod_exp(g, phi / f, mod) == 1) {
                ok = false;
                break;
            }
        }
        if(ok) {
            return g;
        }
    }
    return -1;
}

int64_t baby_step_giant_step(int64_t base, int64_t target, int64_t mod) {
    if(target % mod == 1) {
        return 0;
    }

    base %= mod;
    target %= mod;

    int64_t m = (int64_t)sqrtl((long double)(mod - 1)) + 2;

    unordered_map<int64_t, int64_t> baby;
    int64_t cur = 1;
    for(int64_t j = 0; j < m; j++) {
        if(!baby.count(cur)) {
            baby[cur] = j;
        }
        cur = mul_mod(cur, base, mod);
    }

    int64_t a_inv_m = mod_exp(base, (mod - 1) - m, mod);

    int64_t giant = target;
    for(int64_t i = 0; i < m; i++) {
        auto it = baby.find(giant);
        if(it != baby.end()) {
            return i * m + it->second;
        }
        giant = mul_mod(giant, a_inv_m, mod);
    }
    return -1;
}

void solve() {
    // Solve x^K = A (mod P) for prime P.
    //
    // - If A == 0 the only root is 0.
    //
    // - Let d = gcd(K, P-1). A solution exists iff A^((P-1)/d) == 1 (mod P).
    //
    // - Take a primitive root alpha of P (found by checking candidates against
    //   the prime factors of P-1). Solve the discrete log T with alpha^T == A
    //   via baby-step giant-step, so x = alpha^L means we need K*L == T
    //   (mod P-1).
    //
    // - That linear congruence is solvable iff d | T. Reduce to K'*L == T'
    //   (mod M) with T'=T/d, M=(P-1)/d, K'=K/d, giving L0 = T'*inv(K') (mod M).
    //   The d distinct roots are alpha^(L0 + i*M) for i in [0, d), which we
    //   sort and deduplicate.

    if(a == 0) {
        cout << 1 << "\n";
        cout << 0 << "\n";
        return;
    }

    int64_t gx, gy;
    int64_t d = ext_gcd(k, p - 1, gx, gy);
    if(mod_exp(a, (p - 1) / d, p) != 1) {
        cout << 0 << "\n";
        return;
    }

    int64_t alpha = find_primitive_root(p);
    if(alpha == -1) {
        cout << 0 << "\n";
        return;
    }

    int64_t t = baby_step_giant_step(alpha, a, p);
    if(t == -1 || t % d != 0) {
        cout << 0 << "\n";
        return;
    }

    int64_t t_prime = t / d;
    int64_t m = (p - 1) / d;
    int64_t k_prime = ((k / d) % m + m) % m;

    int64_t inv_k_prime = (m == 1) ? 0 : inv_mod(k_prime, m);
    if(inv_k_prime == -1) {
        cout << 0 << "\n";
        return;
    }

    int64_t l0 = (m == 1) ? 0 : mul_mod(t_prime % m, inv_k_prime, m);

    set<int64_t> roots;
    for(int64_t i = 0; i < d; i++) {
        int64_t exponent = (l0 + i * m) % (p - 1);
        roots.insert(mod_exp(alpha, exponent, p));
    }

    cout << roots.size() << "\n";
    bool first = true;
    for(int64_t x: roots) {
        if(!first) {
            cout << ' ';
        }
        cout << x;
        first = false;
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
