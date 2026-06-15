#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll mulmod(ll a, ll b, ll m) { return (__int128)a * b % m; }

ll power(ll a, ll e, ll m) {
    ll r = 1; a %= m;
    for (; e > 0; e >>= 1) { if (e & 1) r = mulmod(r, a, m); a = mulmod(a, a, m); }
    return r;
}

ll gcd(ll a, ll b) { return b ? gcd(b, a%b) : a; }
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) { x=1; y=0; return a; }
    ll x1, y1; ll g = extgcd(b, a%b, x1, y1);
    x = y1; y = x1 - (a/b)*y1; return g;
}

ll bsgs(ll g, ll h, ll mod) {
    // g^x = h mod mod, x in [0, mod-1]
    ll n = (ll)ceil(sqrt((double)mod)) + 1;
    unordered_map<ll,ll> table;
    ll gn = power(g, n, mod);
    ll cur = h;
    for (ll j = 0; j <= n; j++) {
        table[cur] = j;
        cur = mulmod(cur, g, mod);
    }
    cur = 1;
    for (ll i = 1; i <= n+1; i++) {
        cur = mulmod(cur, gn, mod);
        if (table.count(cur)) {
            ll ans = i*n - table[cur];
            if (ans >= 0) return ans % (mod-1);
        }
    }
    return -1;
}

int main(){
    ll P, K, A;
    cin >> P >> K >> A;
    if (A == 0) {
        cout << 1 << "\n" << 0 << "\n";
        return 0;
    }
    // Find primitive root
    ll phi = P - 1;
    vector<ll> factors;
    ll tmp = phi;
    for (ll i = 2; i*i <= tmp; i++) if (tmp%i==0) { factors.push_back(i); while(tmp%i==0) tmp/=i; }
    if (tmp > 1) factors.push_back(tmp);
    ll g = 2;
    while (true) {
        bool ok = true;
        for (ll f : factors) if (power(g, phi/f, P) == 1) { ok=false; break; }
        if (ok) break;
        g++;
    }
    // Discrete log: g^t = A mod P
    ll t = bsgs(g, A, P);
    // Solve m*K ≡ t (mod phi)
    ll d = gcd(K, phi);
    if (t % d != 0) { cout << 0 << "\n\n"; return 0; }
    ll K2 = K/d, phi2 = phi/d, t2 = t/d;
    ll inv, dummy;
    extgcd(K2, phi2, inv, dummy);
    inv = ((inv % phi2) + phi2) % phi2;
    ll m0 = mulmod(inv, t2, phi2);
    vector<ll> roots;
    for (ll i = 0; i < d; i++) {
        ll m = (m0 + i*phi2) % phi;
        roots.push_back(power(g, m, P));
    }
    sort(roots.begin(), roots.end());
    roots.erase(unique(roots.begin(), roots.end()), roots.end());
    cout << roots.size() << "\n";
    for (ll r : roots) cout << r << " ";
    cout << "\n";
}
