#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int n, m;
ll d;
vector<ll> p, q;

struct Level {
    int cnt;
    ll t;
};

vector<Level> buildLevels(const vector<ll>& a) {
    int sz = (int)a.size();
    vector<ll> events;
    for (int i = 0; i + 1 < sz; ++i) {
        ll g = a[i + 1] - a[i];
        ll t = g / 2 + 1;
        if (t >= 1 && t <= 1000000000LL) events.push_back(t);
    }

    sort(events.begin(), events.end());

    vector<Level> res;
    ll curT = 1;
    int cnt = 1;
    for (int i = 0; i + 1 < sz; ++i) {
        if (a[i + 1] - a[i] > 2) cnt++;
    }
    res.push_back({cnt, 1});

    int idx = 0;
    while (idx < (int)events.size()) {
        ll t = events[idx];
        int same = 0;
        while (idx < (int)events.size() && events[idx] == t) {
            idx++;
            same++;
        }
        cnt -= same;
        if (res.back().cnt != cnt) res.push_back({cnt, t});
    }

    sort(res.begin(), res.end(), [](const Level& x, const Level& y) {
        return x.cnt < y.cnt;
    });

    return res;
}

vector<pair<ll,ll>> makeIntervals(const vector<ll>& a, ll t) {
    vector<pair<ll,ll>> v;
    int sz = (int)a.size();
    int l = 0;
    for (int i = 1; i <= sz; ++i) {
        if (i == sz || a[i] - a[i - 1] > 2 * t) {
            v.push_back({a[l] - t, a[i - 1] + t});
            l = i;
        }
    }
    return v;
}

bool ok(ll t1, ll t2) {
    vector<pair<ll,ll>> A = makeIntervals(p, t1);
    vector<pair<ll,ll>> B = makeIntervals(q, t2);

    int i = 0, j = 0;
    while (i < (int)A.size() && j < (int)B.size()) {
        ll l = max(A[i].first, B[j].first);
        ll r = min(A[i].second, B[j].second);
        if (r - l > d) return false;

        if (A[i].second < B[j].second) i++;
        else j++;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> d;
    p.resize(n);
    q.resize(m);

    for (ll &x : p) cin >> x;
    for (ll &x : q) cin >> x;

    if (!ok(1, 1)) {
        cout << "No solution\n";
        return 0;
    }

    vector<Level> L1 = buildLevels(p);
    vector<Level> L2 = buildLevels(q);

    int best = INT_MAX;
    ll ans1 = -1, ans2 = -1;

    for (const auto& a : L1) {
        int lo = 0, hi = (int)L2.size() - 1, pos = -1;

        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (ok(a.t, L2[mid].t)) {
                pos = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }

        if (pos != -1) {
            int sum = a.cnt + L2[pos].cnt;
            if (sum < best) {
                best = sum;
                ans1 = a.t;
                ans2 = L2[pos].t;
            }
        }
    }

    if (ans1 == -1) {
        cout << "No solution\n";
    } else {
        cout << ans1 << ' ' << ans2 << '\n';
    }

    return 0;
}
