#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll smallestPrimeFactor(ll n) {
    if (n % 2 == 0) return 2;
    for (ll i = 3; i * i <= n; i += 2)
        if (n % i == 0) return i;
    return n; // prime
}

int main() {
    ll A, B;
    cin >> A >> B;
    vector<ll> seq;
    seq.push_back(A);
    ll cur = A;
    if (cur % 2 != 0) {
        ll p = smallestPrimeFactor(cur);
        if (p == cur) { cout << "Impossible\n"; return 0; }
        cur += p; seq.push_back(cur);
    }
    // cur is even, add 2s toward B
    if (B % 2 == 0) {
        while (cur < B) { cur += 2; seq.push_back(cur); }
    } else {
        // find odd prime factor p of B, go to B-p then +p
        ll p = smallestPrimeFactor(B);
        if (p == B) { cout << "Impossible\n"; return 0; }
        ll target = B - p;
        if (target < cur) { cout << "Impossible\n"; return 0; }
        while (cur < target) { cur += 2; seq.push_back(cur); }
        cur += p; seq.push_back(cur);
    }
    if (cur != B) { cout << "Impossible\n"; return 0; }
    for (ll x : seq) cout << x << "\n";
}
