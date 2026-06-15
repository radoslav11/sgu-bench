#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
using i64 = long long;

struct Interval {
    ull l, r;
};

static vector<Interval> intersectIntervals(const vector<Interval>& a, const vector<Interval>& b) {
    vector<Interval> res;
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        ull l = max(a[i].l, b[j].l);
        ull r = min(a[i].r, b[j].r);
        if (l <= r) res.push_back({l, r});
        if (a[i].r < b[j].r) ++i;
        else ++j;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<ull> a(n);
    ull mn = ULLONG_MAX;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        mn = min(mn, a[i]);
    }

    const int L = 61;

    vector<int> xr(L, 0);
    for (int b = 0; b < L; ++b) {
        int v = 0;
        for (ull x : a) v ^= int((x >> b) & 1ULL);
        xr[b] = v;
    }

    vector<int> ord(n), tmp;
    iota(ord.begin(), ord.end(), 0);
    tmp.reserve(n);

    auto rebuild_order = [&](int bit) {
        tmp.clear();
        for (int id : ord) {
            if (((a[id] >> bit) & 1ULL) == 0) tmp.push_back(id);
        }
        for (int id : ord) {
            if (((a[id] >> bit) & 1ULL) == 1) tmp.push_back(id);
        }
        ord.swap(tmp);
    };

    if (n % 2 == 1) {
        ull x = 0;

        for (int b = 0; b < L; ++b) {
            ull mask = (b == 64 ? ~0ULL : ((1ULL << b) - 1));
            ull pref = x & mask;

            int parityBorrow = 0;
            for (int id : ord) {
                ull r = a[id] & mask;
                if (r < pref) parityBorrow ^= 1;
                else break;
            }

            int bit = xr[b] ^ parityBorrow;
            if (bit) x |= (1ULL << b);

            rebuild_order(b);
        }

        ull ans = 0;
        if (x < mn) {
            ull xo = 0;
            for (ull v : a) xo ^= (v - x);
            if (xo == 0) ans = 1;
        }

        cout << ans << '\n';
        return 0;
    }

    vector<Interval> cur;
    cur.push_back({0, 0});

    for (int b = 0; b < L; ++b) {
        vector<Interval> good;

        ull mod = 1ULL << b;

        if (b == 0) {
            if (xr[b] == 0) good.push_back({0, 0});
        } else {
            ull mask = mod - 1;
            ull prev = 0;
            int parity = 0;

            int i = 0;
            while (i < n) {
                ull r = a[ord[i]] & mask;
                ull point = r + 1;

                if (prev <= point - 1 && parity == xr[b]) {
                    good.push_back({prev, point - 1});
                }

                int cntParity = 0;
                while (i < n && (a[ord[i]] & mask) == r) {
                    cntParity ^= 1;
                    ++i;
                }

                parity ^= cntParity;
                prev = point;
            }

            if (prev <= mod - 1 && parity == xr[b]) {
                good.push_back({prev, mod - 1});
            }
        }

        cur = intersectIntervals(cur, good);
        if (cur.empty()) {
            cout << 0 << '\n';
            return 0;
        }

        vector<Interval> nxt;
        nxt.reserve(cur.size() * 2);
        for (auto seg : cur) nxt.push_back(seg);
        for (auto seg : cur) nxt.push_back({seg.l + mod, seg.r + mod});
        cur.swap(nxt);

        sort(cur.begin(), cur.end(), [](const Interval& x, const Interval& y) {
            return x.l < y.l;
        });

        vector<Interval> merged;
        for (auto seg : cur) {
            if (merged.empty() || merged.back().r + 1 < seg.l) {
                merged.push_back(seg);
            } else {
                merged.back().r = max(merged.back().r, seg.r);
            }
        }
        cur.swap(merged);

        rebuild_order(b);
    }

    ull ans = 0;
    ull limitL = 0, limitR = mn - 1;

    for (auto seg : cur) {
        ull l = max(seg.l, limitL);
        ull r = min(seg.r, limitR);
        if (l <= r) ans += r - l + 1;
    }

    cout << ans << '\n';
    return 0;
}
