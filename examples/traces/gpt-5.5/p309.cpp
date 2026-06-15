#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

vector<Point> pts;

bool coverable(vector<int> v, int k, long long d) {
    if (v.empty()) return true;

    long long minx = LLONG_MAX, maxx = LLONG_MIN;
    long long miny = LLONG_MAX, maxy = LLONG_MIN;

    for (int id : v) {
        minx = min(minx, pts[id].x);
        maxx = max(maxx, pts[id].x);
        miny = min(miny, pts[id].y);
        maxy = max(maxy, pts[id].y);
    }

    if (maxx - minx <= d && maxy - miny <= d) return true;
    if (k == 1) return false;

    struct Square {
        long long lx, rx, ly, ry;
    };

    Square sq[4] = {
        {minx, minx + d, miny, miny + d},
        {minx, minx + d, maxy - d, maxy},
        {maxx - d, maxx, miny, miny + d},
        {maxx - d, maxx, maxy - d, maxy}
    };

    for (int s = 0; s < 4; s++) {
        vector<int> rem;
        rem.reserve(v.size());

        for (int id : v) {
            long long x = pts[id].x;
            long long y = pts[id].y;

            if (sq[s].lx <= x && x <= sq[s].rx &&
                sq[s].ly <= y && y <= sq[s].ry) {
                continue;
            }

            rem.push_back(id);
        }

        if (rem.size() == v.size()) continue;

        if (coverable(rem, k - 1, d)) return true;
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    pts.resize(n);

    long long minx = LLONG_MAX, maxx = LLONG_MIN;
    long long miny = LLONG_MAX, maxy = LLONG_MIN;

    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
        minx = min(minx, pts[i].x);
        maxx = max(maxx, pts[i].x);
        miny = min(miny, pts[i].y);
        maxy = max(maxy, pts[i].y);
    }

    vector<int> all(n);
    iota(all.begin(), all.end(), 0);

    long long low = 0;
    long long high = max(maxx - minx, maxy - miny);

    while (low < high) {
        long long mid = (low + high) / 2;

        if (coverable(all, 3, mid)) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    cout << low << '\n';

    return 0;
}
