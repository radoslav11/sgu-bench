#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct Pt {
    int64 x, y;
};

int64 vx[6] = {1, 0, -1, -1, 0, 1};
int64 vy[6] = {0, 1, 1, 0, -1, -1};

int64 det3(i128 a1,i128 b1,i128 c1,
           i128 a2,i128 b2,i128 c2,
           i128 a3,i128 b3,i128 c3) {
    i128 v = a1*(b2*c3-b3*c2) - b1*(a2*c3-a3*c2) + c1*(a2*b3-a3*b2);
    if (v > (i128)LLONG_MAX) return LLONG_MAX;
    if (v < (i128)LLONG_MIN) return LLONG_MIN;
    return (int64)v;
}

bool feasibleSum(const array<int64,6>& cap, int64 tx, int64 ty, int64 S) {
    int ids[6] = {0,1,2,3,4,5};

    for (int maskFree = 0; maskFree < (1<<6); maskFree++) {
        if (__builtin_popcount((unsigned)maskFree) != 3) continue;

        vector<int> freev, boundv;
        for (int i = 0; i < 6; i++) {
            if (maskFree & (1<<i)) freev.push_back(i);
            else boundv.push_back(i);
        }

        for (int bm = 0; bm < 8; bm++) {
            i128 rx = tx, ry = ty, rs = S;

            for (int k = 0; k < 3; k++) {
                int id = boundv[k];
                int64 val = (bm & (1<<k)) ? cap[id] : 0;
                rx -= (i128)val * vx[id];
                ry -= (i128)val * vy[id];
                rs -= val;
            }

            int a = freev[0], b = freev[1], c = freev[2];

            i128 D =
                (i128)vx[a] * ((i128)vy[b] - vy[c]) -
                (i128)vx[b] * ((i128)vy[a] - vy[c]) +
                (i128)vx[c] * ((i128)vy[a] - vy[b]);

            if (D == 0) continue;

            i128 Da =
                rx * ((i128)vy[b] - vy[c]) -
                (i128)vx[b] * (ry - (i128)vy[c] * rs) +
                (i128)vx[c] * (ry - (i128)vy[b] * rs);

            i128 Db =
                (i128)vx[a] * (ry - (i128)vy[c] * rs) -
                rx * ((i128)vy[a] - vy[c]) +
                (i128)vx[c] * ((i128)vy[a] * rs - ry);

            i128 Dc =
                (i128)vx[a] * ((i128)vy[b] * rs - ry) -
                (i128)vx[b] * ((i128)vy[a] * rs - ry) +
                rx * ((i128)vy[a] - vy[b]);

            if (Da % D || Db % D || Dc % D) continue;

            i128 va = Da / D, vb = Db / D, vc = Dc / D;

            if (va < 0 || vb < 0 || vc < 0) continue;
            if (va > cap[a] || vb > cap[b] || vc > cap[c]) continue;

            return true;
        }
    }

    return false;
}

int64 hexDist(int64 x, int64 y) {
    return max({llabs(x), llabs(y), llabs(x + y)});
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 x, y;
    int d2, d1;
    int64 b;

    cin >> x >> y >> d2 >> d1 >> b;

    int r = (d2 - d1 + 6) % 6;

    array<int,6> beta{};
    for (int s = 0; s <= r; s++) {
        beta[(d1 + s) % 6]++;
    }

    auto calc = [&](int64 t) -> int64 {
        array<int64,6> q{}, low{}, cap{};
        int64 lx = 0, ly = 0;
        int64 lowerSum = r + 6 * t;

        for (int i = 0; i < 6; i++) {
            q[i] = t + beta[i];
            low[i] = q[i] - (i == d1 ? 1 : 0);
            cap[i] = b * q[i] - low[i];

            lx += low[i] * vx[i];
            ly += low[i] * vy[i];
        }

        int64 rx = x - lx;
        int64 ry = y - ly;

        int64 totalCap = 0;
        for (int i = 0; i < 6; i++) totalCap += cap[i];

        int64 lo = hexDist(rx, ry), hi = totalCap;
        if (lo > hi) return (int64)4e18;

        if (!feasibleSum(cap, rx, ry, hi)) return (int64)4e18;

        while (lo < hi) {
            int64 mid = (lo + hi) / 2;
            if (feasibleSum(cap, rx, ry, mid)) hi = mid;
            else lo = mid + 1;
        }

        return lowerSum + lo;
    };

    int64 baseH = hexDist(x, y);
    int64 hi = baseH / max<int64>(1, b - 1) + 100;
    hi += llabs(x) + llabs(y) < 1000 ? 1000 : 0;

    int64 lo = 0;
    while (hi - lo > 10) {
        int64 m1 = lo + (hi - lo) / 3;
        int64 m2 = hi - (hi - lo) / 3;

        if (calc(m1) <= calc(m2)) hi = m2 - 1;
        else lo = m1 + 1;
    }

    int64 ans = (int64)4e18;
    for (int64 t = max<int64>(0, lo - 20); t <= hi + 20; t++) {
        ans = min(ans, calc(t));
    }

    cout << ans << '\n';
    return 0;
}
