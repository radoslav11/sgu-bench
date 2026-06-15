#include <bits/stdc++.h>
using namespace std;

struct Segment {
    long long x1, y1, x2, y2;
    char col;
    bool vertical() const { return x1 == x2; }
};

long long cross(long long ax, long long ay, long long bx, long long by, long long cx, long long cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

int sgn(long long v) {
    if (v < 0) return -1;
    if (v > 0) return 1;
    return 0;
}

bool onSegment(long long ax, long long ay, long long bx, long long by, long long px, long long py) {
    return cross(ax, ay, bx, by, px, py) == 0 &&
           min(ax, bx) <= px && px <= max(ax, bx) &&
           min(ay, by) <= py && py <= max(ay, by);
}

bool intersect(const Segment& a, const Segment& b) {
    long long c1 = cross(a.x1, a.y1, a.x2, a.y2, b.x1, b.y1);
    long long c2 = cross(a.x1, a.y1, a.x2, a.y2, b.x2, b.y2);
    long long c3 = cross(b.x1, b.y1, b.x2, b.y2, a.x1, a.y1);
    long long c4 = cross(b.x1, b.y1, b.x2, b.y2, a.x2, a.y2);

    if (sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0) return true;
    if (c1 == 0 && onSegment(a.x1, a.y1, a.x2, a.y2, b.x1, b.y1)) return true;
    if (c2 == 0 && onSegment(a.x1, a.y1, a.x2, a.y2, b.x2, b.y2)) return true;
    if (c3 == 0 && onSegment(b.x1, b.y1, b.x2, b.y2, a.x1, a.y1)) return true;
    if (c4 == 0 && onSegment(b.x1, b.y1, b.x2, b.y2, a.x2, a.y2)) return true;
    return false;
}

double intersectionX(const Segment& a, const Segment& b) {
    double x1 = a.x1, y1 = a.y1, x2 = a.x2, y2 = a.y2;
    double x3 = b.x1, y3 = b.y1, x4 = b.x2, y4 = b.y2;

    double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    double p1 = x1 * y2 - y1 * x2;
    double p2 = x3 * y4 - y3 * x4;

    return (p1 * (x3 - x4) - (x1 - x2) * p2) / d;
}

double valueAt(const Segment& s, double x) {
    return s.y1 + (double)(s.y2 - s.y1) * (x - s.x1) / (double)(s.x2 - s.x1);
}

int colorId(char c) {
    if (c == 'R') return 0;
    if (c == 'G') return 1;
    return 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<Segment> seg(N);
    vector<double> xs;

    for (int i = 0; i < N; ++i) {
        cin >> seg[i].x1 >> seg[i].y1 >> seg[i].x2 >> seg[i].y2 >> seg[i].col;
        xs.push_back(seg[i].x1);
        xs.push_back(seg[i].x2);
    }

    for (int i = 0; i < N; ++i) {
        if (seg[i].vertical()) continue;

        for (int j = i + 1; j < N; ++j) {
            if (seg[j].vertical()) continue;
            if (!intersect(seg[i], seg[j])) continue;

            long long denom = (seg[i].x1 - seg[i].x2) * (seg[j].y1 - seg[j].y2) -
                              (seg[i].y1 - seg[i].y2) * (seg[j].x1 - seg[j].x2);

            if (denom != 0) {
                xs.push_back(intersectionX(seg[i], seg[j]));
            }
        }
    }

    sort(xs.begin(), xs.end());

    vector<double> ux;
    const double EPS = 1e-9;

    for (double x : xs) {
        if (ux.empty() || fabs(x - ux.back()) > EPS) {
            ux.push_back(x);
        }
    }

    double ans[3] = {0.0, 0.0, 0.0};

    for (int i = 0; i + 1 < (int)ux.size(); ++i) {
        double l = ux[i], r = ux[i + 1];
        if (r - l < 1e-8) continue;

        double mid = (l + r) / 2.0;

        double bestY = 1e100;
        int bestColor = -1;

        for (const Segment& s : seg) {
            if (s.vertical()) continue;

            double mn = min(s.x1, s.x2);
            double mx = max(s.x1, s.x2);

            if (mid > mn + EPS && mid < mx - EPS) {
                double y = valueAt(s, mid);
                if (y < bestY) {
                    bestY = y;
                    bestColor = colorId(s.col);
                }
            }
        }

        if (bestColor != -1) {
            ans[bestColor] += r - l;
        }
    }

    cout << fixed << setprecision(2);
    cout << "R " << ans[0] << '\n';
    cout << "G " << ans[1] << '\n';
    cout << "B " << ans[2] << '\n';

    return 0;
}
