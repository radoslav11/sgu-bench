#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// Fast modular exponentiation: computes base^exp mod m in O(log exp).
int64 modexp(int64 base, int64 exp, int64 m) {
    int64 res = 1 % m;
    base %= m;
    while (exp > 0) {
        if (exp & 1) res = (__int128)res * base % m;
        base = (__int128)base * base % m;
        exp >>= 1;
    }
    return res;
}

// Extended GCD: returns (g, x, y) such that a*x + b*y = g = gcd(a,b).
tuple<int64,int64,int64> extgcd(int64 a, int64 b) {
    if (b == 0) return {a, 1, 0};
    int64 g, x1, y1;
    tie(g, x1, y1) = extgcd(b, a % b);
    int64 x = y1;
    int64 y = x1 - (a / b) * y1;
    return {g, x, y};
}

// Modular inverse of a mod m, if gcd(a,m)==1.
int64 invmod(int64 a, int64 m) {
    int64 g, x, y;
    tie(g, x, y) = extgcd(a, m);
    if (g != 1) return -1;
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Factor n into its distinct prime divisors.
vector<int64> factorize(int64 n) {
    vector<int64> fac;
    for (int64 p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            fac.push_back(p);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) fac.push_back(n);
    return fac;
}

// Find a primitive root modulo prime p.
int64 find_primitive(int64 p) {
    if (p == 2) return 1;
    int64 phi = p - 1;
    auto primes = factorize(phi);
    // Check if g is generator: for all prime factors q of phi, g^(phi/q) != 1
    for (int64 g = 2; g < p; ++g) {
        bool ok = true;
        for (auto q : primes) {
            if (modexp(g, phi / q, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

// Baby-step Giant-step discrete log: find x s.t. a^x ≡ b (mod p), returns x or -1.
int64 discrete_log(int64 a, int64 b, int64 p) {
    a %= p; b %= p;
    if (b == 1) return 0;
    int64 m = (int64)ceil(sqrt(p - 1));
    unordered_map<int64,int64> baby;
    baby.reserve(m+1);
    // Baby steps: store a^j
    int64 cur = 1;
    for (int64 j = 0; j < m; ++j) {
        if (!baby.count(cur))
            baby[cur] = j;
        cur = (__int128)cur * a % p;
    }
    // Compute factor a^{-m}
    int64 factor = modexp(a, p-1-m, p);
    int64 giant = b;
    for (int64 i = 0; i <= m; ++i) {
        if (baby.count(giant)) {
            return i*m + baby[giant];
        }
        giant = (__int128)giant * factor % p;
    }
    return -1;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int64 P, K, A;
    cin >> P >> K >> A;

    // Special case A == 0
    if (A == 0) {
        cout << 1 << "\n" << 0 << "\n";
        return 0;
    }

    // Compute d = gcd(K, P-1) and test solvability
    int64 d = gcd(K, P-1LL);
    // If A^((P-1)/d) != 1, no solutions
    if (modexp(A, (P-1)/d, P) != 1) {
        cout << 0 << "\n";
        return 0;
    }

    // Find primitive root g
    int64 g = find_primitive(P);

    // Compute discrete log T: g^T ≡ A (mod P)
    int64 T = discrete_log(g, A, P);
    // T must exist and be divisible by d
    if (T < 0 || T % d != 0) {
        cout << 0 << "\n";
        return 0;
    }

    // Reduce the linear congruence K*y ≡ T (mod P-1)
    int64 Kp = K / d;
    int64 Mp = (P-1) / d;
    int64 Tp = T / d;

    // Inverse of K' mod M'
    int64 invKp = invmod(Kp, Mp);

    // Particular solution y0
    int64 y0 = ( (__int128)Tp * invKp ) % Mp;
    if (y0 < 0) y0 += Mp;

    // Generate all d solutions: y = y0 + i*Mp
    vector<int64> roots;
    for (int64 i = 0; i < d; ++i) {
        int64 exp = y0 + i*Mp;
        exp %= (P-1);
        int64 x = modexp(g, exp, P);
        roots.push_back(x);
    }

    sort(roots.begin(), roots.end());
    roots.erase(unique(roots.begin(), roots.end()), roots.end());

    // Output
    cout << roots.size() << "\n";
    for (auto x : roots) cout << x << " ";
    cout << "\n";
    return 0;
}
