#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Pt {
    ll x, y;
};

ll N;
Pt B, W, Z;
ll D;

ll cheb(Pt a, Pt b) {
    return max(llabs(a.x - b.x), llabs(a.y - b.y));
}

bool interval_ok(ll lo, ll hi, ll fixedCoord, bool horizontal, Pt other, ll L, ll U) {
    if (lo > hi) return false;

    ll mn, mx;
    if (horizontal) {
        ll dy = llabs(fixedCoord - other.y);
        ll dxmin = 0;
        if (other.x < lo) dxmin = lo - other.x;
        else if (other.x > hi) dxmin = other.x - hi;

        mn = max(dxmin, dy);
        mx = max({llabs(lo - other.x), llabs(hi - other.x), dy});
    } else {
        ll dx = llabs(fixedCoord - other.x);
        ll dymin = 0;
        if (other.y < lo) dymin = lo - other.y;
        else if (other.y > hi) dymin = other.y - hi;

        mn = max(dx, dymin);
        mx = max({llabs(lo - other.y), llabs(hi - other.y), dx});
    }

    return max(mn, L) <= min(mx, U);
}

bool check_horizontal(ll y, ll x1, ll x2, Pt other, ll k, ll L, ll U) {
    x1 = max(x1, 1LL);
    x2 = min(x2, N);
    if (x1 > x2 || y < 1 || y > N) return false;

    if (y < Z.y - k || y > Z.y + k) return false;

    vector<pair<ll,ll>> parts;

    ll inL = max(x1, Z.x - k);
    ll inR = min(x2, Z.x + k);

    if (llabs(y - Z.y) == k) {
        if (inL <= inR) parts.push_back({inL, inR});
    } else {
        if (Z.x - k >= x1 && Z.x - k <= x2) parts.push_back({Z.x - k, Z.x - k});
        if (k != 0 && Z.x + k >= x1 && Z.x + k <= x2) parts.push_back({Z.x + k, Z.x + k});
    }

    for (auto [l, r] : parts) {
        if (interval_ok(l, r, y, true, other, L, U)) return true;
    }
    return false;
}

bool check_vertical(ll x, ll y1, ll y2, Pt other, ll k, ll L, ll U) {
    y1 = max(y1, 1LL);
    y2 = min(y2, N);
    if (y1 > y2 || x < 1 || x > N) return false;

    if (x < Z.x - k || x > Z.x + k) return false;

    vector<pair<ll,ll>> parts;

    ll inL = max(y1, Z.y - k);
    ll inR = min(y2, Z.y + k);

    if (llabs(x - Z.x) == k) {
        if (inL <= inR) parts.push_back({inL, inR});
    } else {
        if (Z.y - k >= y1 && Z.y - k <= y2) parts.push_back({Z.y - k, Z.y - k});
        if (k != 0 && Z.y + k >= y1 && Z.y + k <= y2) parts.push_back({Z.y + k, Z.y + k});
    }

    for (auto [l, r] : parts) {
        if (interval_ok(l, r, x, false, other, L, U)) return true;
    }
    return false;
}

bool can_intercept_king(Pt start, Pt other, ll k) {
    ll r = D - 2 * k;
    if (r < 1) return false;

    ll L = llabs(k - r);
    ll U = k + r;

    ll x1 = start.x - k;
    ll x2 = start.x + k;
    ll y1 = start.y - k;
    ll y2 = start.y + k;

    if (check_horizontal(y1, x1, x2, other, k, L, U)) return true;
    if (check_horizontal(y2, x1, x2, other, k, L, U)) return true;
    if (check_vertical(x1, y1, y2, other, k, L, U)) return true;
    if (check_vertical(x2, y1, y2, other, k, L, U)) return true;

    return false;
}

bool can(ll k) {
    return can_intercept_king(B, W, k) || can_intercept_king(W, B, k);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    cin >> B.x >> B.y;
    cin >> W.x >> W.y;
    cin >> Z.x >> Z.y;

    D = cheb(B, W);

    ll meet_moves = D - 1;

    for (ll k = 1; 2 * k < meet_moves; ++k) {
        if (can(k)) {
            cout << "YES\n" << k << '\n';
            return 0;
        }
    }

    cout << "NO\n" << meet_moves << '\n';
    return 0;
}
