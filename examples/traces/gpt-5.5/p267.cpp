#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

long long crossVec(long long ax, long long ay, long long bx, long long by) {
    return ax * by - ay * bx;
}

long long dotVec(long long ax, long long ay, long long bx, long long by) {
    return ax * bx + ay * by;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<pair<long long, bool>> pies;

    for (int i = 0; i < N; i++) {
        Point p[4], c1, c2;
        for (int j = 0; j < 4; j++) cin >> p[j].x >> p[j].y;
        cin >> c1.x >> c1.y >> c2.x >> c2.y;

        vector<pair<long long, int>> dist;
        for (int j = 1; j < 4; j++) {
            long long dx = p[j].x - p[0].x;
            long long dy = p[j].y - p[0].y;
            dist.push_back({dx * dx + dy * dy, j});
        }

        sort(dist.begin(), dist.end());

        Point a = p[dist[0].second];
        Point b = p[dist[1].second];

        long long v1x = a.x - p[0].x;
        long long v1y = a.y - p[0].y;
        long long v2x = b.x - p[0].x;
        long long v2y = b.y - p[0].y;

        long long area = llabs(crossVec(v1x, v1y, v2x, v2y));

        long long sx = 0, sy = 0;
        for (int j = 0; j < 4; j++) {
            sx += p[j].x;
            sy += p[j].y;
        }

        long long u1x = 4 * c1.x - sx;
        long long u1y = 4 * c1.y - sy;
        long long u2x = 4 * c2.x - sx;
        long long u2y = 4 * c2.y - sy;

        bool edible = true;

        if ((u1x == 0 && u1y == 0) || (u2x == 0 && u2y == 0)) {
            edible = false;
        } else {
            long long cr = crossVec(u1x, u1y, u2x, u2y);
            long long dt = dotVec(u1x, u1y, u2x, u2y);

            if (cr == 0 && dt > 0) {
                edible = false;
            }
        }

        pies.push_back({area, edible});
    }

    if (K == 0) {
        cout << fixed << setprecision(3) << 0.0 << ' ' << 0.0 << '\n';
        return 0;
    }

    sort(pies.begin(), pies.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    long long borderArea = pies[K - 1].first;

    int mandatory = 0;
    int need = 0;
    int equalTotal = 0;
    int equalEdible = 0;

    long double base = 0.0;

    for (auto [area, edible] : pies) {
        if (area > borderArea) {
            mandatory++;
            if (edible) base += (long double)area / 2.0;
        } else if (area == borderArea) {
            equalTotal++;
            if (edible) equalEdible++;
        }
    }

    need = K - mandatory;

    int equalBad = equalTotal - equalEdible;

    int pessEdible = max(0, need - equalBad);
    int optEdible = min(need, equalEdible);

    long double pessimist = base + (long double)pessEdible * borderArea / 2.0;
    long double optimist = base + (long double)optEdible * borderArea / 2.0;

    cout << fixed << setprecision(3) << (double)pessimist << ' ' << (double)optimist << '\n';

    return 0;
}
