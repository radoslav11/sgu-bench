#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

/**
 * Problem: Jumping Joe
 * Goal: Find P1, N1, P2, N2 such that:
 * (P1 - N1)*x1 + (P2 - N2)*x2 = P
 * P1 + N1 + P2 + N2 = K
 * 
 * Let A = P1 - N1 and B = P2 - N2.
 * The equations become:
 * A*x1 + B*x2 = P
 * P1 + N1 + P2 + N2 = K
 * 
 * Let n1 = P1 + N1 and n2 = P2 + N2.
 * Then n1 + n2 = K.
 * For P1, N1 to be non-negative integers, we need:
 * n1 >= |A| and n1 % 2 == |A| % 2 (which is the same as n1 % 2 == A % 2).
 * Similarly, n2 >= |B| and n2 % 2 == B % 2.
 * 
 * Combining these, we need:
 * 1) A*x1 + B*x2 = P
 * 2) K >= |A| + |B|
 * 3) K % 2 == (A + B) % 2 (since n1 + n2 = K and n1 + n2 = A + B mod 2)
 * 
 * We can solve A*x1 + B*x2 = P using the Extended Euclidean Algorithm.
 * The general solution for A, B is:
 * A(t) = A0 + t * (x2/g)
 * B(t) = B0 - t * (x1/g)
 * where g = gcd(x1, x2).
 * 
 * We want to find t that minimizes f(t) = |A(t)| + |B(t)| and satisfies 
 * (A(t) + B(t) - K) % 2 == 0 and f(t) <= K.
 */

typedef long long ll;

ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll x1, x2, P, K;
    if (!(cin >> x1 >> x2 >> P >> K)) return 0;

    ll A0_base, B0_base;
    ll g = extgcd(x1, x2, A0_base, B0_base);

    // If P is not divisible by g, no solution exists.
    if (P % g != 0) {
        cout << "NO" << endl;
        return 0;
    }

    // Initial solution for A*x1 + B*x2 = P
    ll A0 = A0_base * (P / g);
    ll B0 = B0_base * (P / g);

    ll step1 = x2 / g;
    ll step2 = x1 / g;

    // A(t) = A0 + t * step1
    // B(t) = B0 - t * step2
    // f(t) = |A(t)| + |B(t)| is convex.
    // Minimum values of f(t) occur around t where A(t) ~ 0 or B(t) ~ 0.
    // t1 = -A0 / step1, t2 = B0 / step2.
    
    vector<ll> candidates;
    candidates.push_back(-A0 / step1);
    candidates.push_back(B0 / step2);

    vector<ll> check_t;
    for (ll c : candidates) {
        for (ll i = -2; i <= 2; ++i) {
            check_t.push_back(c + i);
        }
    }

    ll best_t = -2e18; // Placeholder
    bool found = false;

    // Check candidates to find if any t satisfies f(t) <= K and parity.
    // We sort them to potentially find a solution closer to the minimum f(t) if needed,
    // but the problem asks for *any* solution.
    for (ll t : check_t) {
        ll cur_A = A0 + t * step1;
        ll cur_B = B0 - t * step2;
        ll cur_f = abs(cur_A) + abs(cur_B);

        if (cur_f <= K && (abs(cur_A + cur_B - K) % 2 == 0)) {
            // Found a suitable t.
            // We need n1, n2 such that n1 + n2 = K, n1 >= |A|, n2 >= |B|, 
            // n1 % 2 == A % 2, n2 % 2 == B % 2.
            // One such solution: n1 = K - |B|, n2 = |B|.
            // Since K >= |A| + |B|, K - |B| >= |A|.
            // Since K = A + B (mod 2), K - |B| = A + B - |B| = A (mod 2).
            
            ll A = cur_A;
            ll B = cur_B;
            ll n1 = K - abs(B);
            ll n2 = abs(B);

            ll P1 = (n1 + A) / 2;
            ll N1 = (n1 - A) / 2;
            ll P2 = (n2 + B) / 2;
            ll N2 = (n2 - B) / 2;

            cout << "YES" << endl;
            cout << P1 << " " << N1 << " " << P2 << " " << N2 << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "NO" << endl;
    }

    return 0;
}

