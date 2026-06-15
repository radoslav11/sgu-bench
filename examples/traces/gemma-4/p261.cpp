#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef long long ll;

/**
 * Performs modular exponentiation: (base^exp) % mod
 * Uses __int128 to prevent potential overflow during intermediate multiplication.
 * Although P <= 10^9, so (P-1)^2 is approx 10^18, which fits in long long,
 * __int128 is a safer choice for larger P or intermediate steps.
 */
ll pow_mod(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp /= 2;
    }
    return res;
}

/**
 * Computes the greatest common divisor of a and b.
 */
ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

/**
 * Extended Euclidean Algorithm to solve ax + by = gcd(a, b).
 * Returns gcd(a, b) and sets x and y.
 */
ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

/**
 * Finds a primitive root modulo P.
 * Steps: factorize (P-1) and check the property g^((P-1)/q) != 1 mod P for each prime factor q of (P-1).
 */
ll find_primitive_root(ll p) {
    if (p == 2) return 1;
    vector<ll> factors;
    ll temp = p - 1;
    for (ll i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            factors.push_back(i);
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) factors.push_back(temp);

    for (ll g = 2; g < p; ++g) {
        bool ok = true;
        for (ll f : factors) {
            if (pow_mod(g, (p - 1) / f, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

/**
 * Baby-step Giant-step algorithm to solve the discrete logarithm problem:
 * Find b such that g^b = a mod p.
 * Time Complexity: O(sqrt(P)).
 */
ll bsgs(ll g, ll a, ll p) {
    if (a == 1) return 0;
    ll m = ceil(sqrt(p - 1));
    unordered_map<ll, int> baby_steps;
    ll cur = 1;
    for (int j = 0; j < m; ++j) {
        if (baby_steps.find(cur) == baby_steps.end())
            baby_steps[cur] = j;
        cur = (__int128)cur * g % p;
    }
    // Giant step calculation: A * (g^-m)^i mod p
    ll inv_gm = pow_mod(pow_mod(g, m, p), p - 2, p);
    cur = a;
    for (int i = 0; i <= m; ++i) {
        if (baby_steps.count(cur)) {
            return (ll)i * m + baby_steps[cur];
        }
        cur = (__int128)cur * inv_gm % p;
    }
    return -1;
}

int main() {
    // Optimize IO
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll p, k, a;
    if (!(cin >> p >> k >> a)) return 0;

    // Special case: if A = 0, the only root is x = 0
    if (a == 0) {
        cout << 1 << endl << 0 << endl;
        return 0;
    }

    // Finding primitive root g and its discrete logarithm b such that g^b = A mod P
    ll g = find_primitive_root(p);
    ll b = bsgs(g, a, p);

    // If no discrete logarithm found, no solution exists for x^k = a mod p
    if (b == -1) {
        cout << 0 << endl << endl;
        return 0;
    }

    /**
     * Solve for y in the congruence: y * K = b mod (P - 1)
     * Let M = P - 1. The congruence is yK ≡ b (mod M).
     * Let d = gcd(K, M). Solutions exist if and only if d divides b.
     */
    ll m = p - 1;
    ll x_gcd, z_gcd;
    ll d = extended_gcd(k, m, x_gcd, z_gcd);

    if (b % d != 0) {
        cout << 0 << endl << endl;
        return 0;
    }

    // One particular solution for y using the extended Euclidean algorithm
    ll y0 = (__int128)x_gcd * (b / d) % (m / d);
    if (y0 < 0) y0 += (m / d);

    /**
     * There are exactly d solutions for y modulo M:
     * y_i = (y0 + i * (M/d)) mod M for i = 0, 1, ..., d-1.
     * The corresponding roots are x_i = g^(y_i) mod P.
     */
    vector<ll> roots;
    ll step = m / d;
    for (int i = 0; i < d; ++i) {
        ll y = (y0 + (__int128)i * step) % m;
        roots.push_back(pow_mod(g, y, p));
    }

    // Final sorting and output formatting
    sort(roots.begin(), roots.end());
    cout << roots.size() << "\n";
    for (int i = 0; i < (int)roots.size(); ++i) {
        cout << roots[i] << (i == (int)roots.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

