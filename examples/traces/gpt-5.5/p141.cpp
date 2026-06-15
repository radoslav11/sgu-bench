#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll floor_div(ll a, ll b) {
    // b > 0
    ll q = a / b, r = a % b;
    if (r < 0) q--;
    return q;
}

ll ceil_div(ll a, ll b) {
    // b > 0
    return -floor_div(-a, b);
}

ll egcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll x1, x2, P, K;
    cin >> x1 >> x2 >> P >> K;

    ll g = std::gcd(x1, x2);

    if (P % g != 0) {
        cout << "NO\n";
        return 0;
    }

    ll A = x1 / g;
    ll B = x2 / g;
    ll C = P / g;

    ll u, v;
    egcd(A, B, u, v);

    ll a0 = u * C;
    ll b0 = v * C;

    auto value = [&](ll t) -> ll {
        ll a = a0 + B * t;
        ll b = b0 - A * t;
        return llabs(a) + llabs(b);
    };

    // Since |a| + |b| <= K implies both |a| <= K and |b| <= K.
    ll L = max(ceil_div(-K - a0, B), ceil_div(b0 - K, A));
    ll R = min(floor_div(K - a0, B), floor_div(b0 + K, A));

    if (L > R) {
        cout << "NO\n";
        return 0;
    }

    vector<ll> cand;
    cand.push_back(L);
    cand.push_back(R);

    ll r1 = floor_div(-a0, B);
    cand.push_back(r1);
    cand.push_back(r1 + 1);

    ll r2 = floor_div(b0, A);
    cand.push_back(r2);
    cand.push_back(r2 + 1);

    ll bestT = L;
    ll bestVal = value(L);

    for (ll t : cand) {
        if (t < L || t > R) continue;
        ll cur = value(t);
        if (cur < bestVal) {
            bestVal = cur;
            bestT = t;
        }
    }

    if (bestVal > K) {
        cout << "NO\n";
        return 0;
    }

    // Find full interval of t where value(t) <= K.
    ll lo = L, hi = bestT;
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (value(mid) <= K) hi = mid;
        else lo = mid + 1;
    }
    ll feasibleL = lo;

    lo = bestT;
    hi = R;
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;
        if (value(mid) <= K) lo = mid;
        else hi = mid - 1;
    }
    ll feasibleR = lo;

    ll chosenT = feasibleL;
    bool found = false;

    for (ll t = feasibleL; t <= feasibleR && t <= feasibleL + 2; t++) {
        if ((value(t) % 2 + 2) % 2 == K % 2) {
            chosenT = t;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "NO\n";
        return 0;
    }

    ll a = a0 + B * chosenT;
    ll b = b0 - A * chosenT;

    ll minimal = llabs(a) + llabs(b);
    ll extra = K - minimal;

    if (extra < 0 || extra % 2 != 0) {
        cout << "NO\n";
        return 0;
    }

    ll total1 = llabs(a) + extra;
    ll total2 = llabs(b);

    ll P1 = (total1 + a) / 2;
    ll N1 = (total1 - a) / 2;
    ll P2 = (total2 + b) / 2;
    ll N2 = (total2 - b) / 2;

    cout << "YES\n";
    cout << P1 << ' ' << N1 << ' ' << P2 << ' ' << N2 << '\n';

    return 0;
}
