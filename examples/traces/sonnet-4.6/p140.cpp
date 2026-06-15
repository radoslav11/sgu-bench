#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1; y = x1 - (a / b) * y1;
    return g;
}
int main() {
    int N, P, B;
    cin >> N >> P >> B;
    vector<ll> A(N);
    for (int i = 0; i < N; i++) { cin >> A[i]; A[i] %= P; }
    vector<ll> X(N, 0);
    ll g = P;
    for (int i = 0; i < N; i++) {
        ll s, t;
        ll ng = extgcd(g, A[i], s, t);
        for (int j = 0; j < N; j++) X[j] = ((s % P) * X[j]) % P;
        X[i] = (X[i] + t % P + P) % P;
        g = ng;
    }
    if (B % g != 0) { cout << "NO\n"; return 0; }
    ll scale = (B / g) % P;
    cout << "YES\n";
    for (int i = 0; i < N; i++) {
        ll val = (X[i] * scale % P + P) % P;
        cout << val;
        if (i < N-1) cout << " ";
    }
    cout << "\n";
}
