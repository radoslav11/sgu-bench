#include <bits/stdc++.h>
using namespace std;

struct HSeg {
    long long y, l, r;
};

struct VSeg {
    long long x, b, t;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<HSeg> hs;
    vector<VSeg> vs;

    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        if (y1 == y2) {
            if (x1 > x2) swap(x1, x2);
            hs.push_back({y1, x1, x2});
        } else {
            if (y1 > y2) swap(y1, y2);
            vs.push_back({x1, y1, y2});
        }
    }

    long long ans = 0;

    int H = (int)hs.size();
    int V = (int)vs.size();

    for (int i = 0; i < H; ++i) {
        for (int j = i + 1; j < H; ++j) {
            if (hs[i].y == hs[j].y) continue;

            long long yLow = min(hs[i].y, hs[j].y);
            long long yHigh = max(hs[i].y, hs[j].y);

            long long xLeft = max(hs[i].l, hs[j].l);
            long long xRight = min(hs[i].r, hs[j].r);

            if (xLeft > xRight) continue;

            long long cnt = 0;

            for (int k = 0; k < V; ++k) {
                if (vs[k].x >= xLeft && vs[k].x <= xRight &&
                    vs[k].b <= yLow && vs[k].t >= yHigh) {
                    ++cnt;
                }
            }

            ans += cnt * (cnt - 1) / 2;
        }
    }

    cout << ans << '\n';

    return 0;
}
