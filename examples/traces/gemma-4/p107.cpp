#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

typedef long long ll;
typedef __int128_t int128;

// Standard function to compute (base^exp) % mod
ll power(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (ll)((int128)res * base % mod);
        base = (ll)((int128)base * base % mod);
        exp /= 2;
    }
    return res;
}

// Extended Euclidean Algorithm to find modular inverse
ll modInverse(ll a, ll m) {
    ll m0 = m, t, q;
    ll x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    ll a = 987654321;
    ll mod2 = 1 << 9;
    ll mod5 = 1;
    for (int i = 0; i < 9; ++i) mod5 *= 5;

    // Lifting solutions modulo 2^9
    vector<ll> sol2 = {1, 3, 5, 7};
    ll cur_m2 = 8;
    for (int k = 3; k < 9; ++k) {
        vector<ll> next_sol2;
        for (ll x : sol2) {
            int128 diff = (int128)a - (int128)x * x;
            // Find smallest non-negative diff modulo 2^{k+1}
            ll m_next = 1LL << (k + 1);
            diff %= m_next;
            if (diff < 0) diff += m_next;
            
            ll t = (ll)((diff / (1LL << k)) % 2);
            next_sol2.push_back(x + t * (1LL << (k - 1)));
        }
        sol2 = next_sol2;
    }

    // Lifting solutions modulo 5^9
    vector<ll> sol5 = {1, 4};
    ll cur_m5 = 5;
    for (int k = 1; k < 9; ++k) {
        vector<ll> next_sol5;
        for (ll x : sol5) {
            int128 target = a;
            int128 current_x = x;
            int128 current_m = cur_m5;
            int128 needed = (target - current_x * current_x) / current_m;
            ll inv2x = modInverse((2 * x) % 5, 5);
            ll t = (ll)((needed % 5 * inv2x) % 5);
            next_sol5.push_back(x + t * cur_m5);
        }
        sol5 = next_sol5;
        cur_m5 *= 5;
    }

    // Combine using Chinese Remainder Theorem to get all 8 solutions modulo 10^9
    vector<ll> sol10;
    ll L2 = mod2;
    ll L5 = mod5;
    ll invL2_L5 = modInverse(L2 % L5, L5);
    for (ll r2 : sol2) {
        for (ll r5 : sol5) {
            // k * L2 = (r5 - r2) mod L5
            ll diff = (r5 - r2) % L5;
            if (diff < 0) diff += L5;
            ll k = (ll)((int128)diff * invL2_L5 % L5);
            sol10.push_back(r2 + k * L2);
        }
    }
    sort(sol10.begin(), sol10.end());

    if (N >= 10) {
        // For N >= 10, the number of N-digit numbers is always 72 * 10^(N-10)
        // This corresponds to 8 * 9 * 10^(N-10)
        cout << "72";
        for (int i = 0; i < N - 10; ++i) cout << "0";
        cout << endl;
    } else {
        // For N < 10, we count how many solutions r_i are in [10^(N-1), 10^N - 1]
        ll low = 1;
        for (int i = 0; i < N - 1; ++i) low *= 10;
        ll high = low * 10 - 1;

        int count = 0;
        for (ll r : sol10) {
            if (r >= low && r <= high) {
                count++;
            }
        }
        cout << count << endl;
    }

    return 0;
}

