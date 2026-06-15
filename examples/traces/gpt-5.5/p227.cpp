#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);
const double TWO_PI = 2.0 * PI;

struct Point {
    double x, y;
};

struct Arc {
    Point a, b, c;
    Point o;
    double r;
    double s, e; // covered counter-clockwise from s to e
};

double dist2(Point p, Point q) {
    double dx = p.x - q.x, dy = p.y - q.y;
    return dx * dx + dy * dy;
}

double dist(Point p, Point q) {
    return sqrt(dist2(p, q));
}

double normAng(double a) {
    while (a < 0) a += TWO_PI;
    while (a >= TWO_PI) a -= TWO_PI;
    return a;
}

double angleOf(Point o, Point p) {
    return normAng(atan2(p.y - o.y, p.x - o.x));
}

double ccwDelta(double a, double b) {
    double d = b - a;
    if (d < 0) d += TWO_PI;
    return d;
}

bool betweenCCW(double s, double e, double x) {
    return ccwDelta(s, x) <= ccwDelta(s, e) + 1e-8;
}

Point pointOnCircle(Point o, double r, double ang) {
    return {o.x + r * cos(ang), o.y + r * sin(ang)};
}

bool containsAngle(const Arc& arc, double ang) {
    return betweenCCW(arc.s, arc.e, normAng(ang));
}

bool containsPoint(const Arc& arc, Point p) {
    if (fabs(dist(p, arc.o) - arc.r) > 1e-6) return false;
    return containsAngle(arc, angleOf(arc.o, p));
}

Arc makeArc(Point a, Point b, Point c) {
    Arc arc;
    arc.a = a;
    arc.b = b;
    arc.c = c;

    double x1 = a.x, y1 = a.y;
    double x2 = b.x, y2 = b.y;
    double x3 = c.x, y3 = c.y;

    double d = 2.0 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

    double ux = ((x1 * x1 + y1 * y1) * (y2 - y3) +
                 (x2 * x2 + y2 * y2) * (y3 - y1) +
                 (x3 * x3 + y3 * y3) * (y1 - y2)) / d;

    double uy = ((x1 * x1 + y1 * y1) * (x3 - x2) +
                 (x2 * x2 + y2 * y2) * (x1 - x3) +
                 (x3 * x3 + y3 * y3) * (x2 - x1)) / d;

    arc.o = {ux, uy};
    arc.r = dist(arc.o, a);

    double aa = angleOf(arc.o, a);
    double bb = angleOf(arc.o, b);
    double cc = angleOf(arc.o, c);

    if (betweenCCW(aa, bb, cc)) {
        arc.s = aa;
        arc.e = bb;
    } else {
        arc.s = bb;
        arc.e = aa;
    }

    return arc;
}

vector<pair<double, double>> intervals(const Arc& arc) {
    vector<pair<double, double>> res;
    if (arc.s <= arc.e) {
        res.push_back({arc.s, arc.e});
    } else {
        res.push_back({arc.s, TWO_PI});
        res.push_back({0.0, arc.e});
    }
    return res;
}

bool sameCircle(const Arc& a, const Arc& b) {
    return dist(a.o, b.o) < 1e-7 && fabs(a.r - b.r) < 1e-7;
}

bool positiveOverlap(const Arc& a, const Arc& b) {
    auto ia = intervals(a);
    auto ib = intervals(b);

    for (auto x : ia) {
        for (auto y : ib) {
            double l = max(x.first, y.first);
            double r = min(x.second, y.second);
            if (r - l > 1e-8) return true;
        }
    }
    return false;
}

void addPoint(vector<Point>& pts, Point p) {
    for (Point q : pts) {
        if (dist(p, q) < 1e-5) return;
    }
    pts.push_back(p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<Arc> arcs;
    for (int i = 0; i < N; ++i) {
        Point a, b, c;
        cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y;
        arcs.push_back(makeArc(a, b, c));
    }

    vector<Point> ans;

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            const Arc& A = arcs[i];
            const Arc& B = arcs[j];

            if (sameCircle(A, B)) {
                if (positiveOverlap(A, B)) {
                    cout << "Infinity\n";
                    return 0;
                }

                if (containsPoint(B, A.a)) addPoint(ans, A.a);
                if (containsPoint(B, A.b)) addPoint(ans, A.b);
                if (containsPoint(A, B.a)) addPoint(ans, B.a);
                if (containsPoint(A, B.b)) addPoint(ans, B.b);
            } else {
                double d = dist(A.o, B.o);

                if (d > A.r + B.r + 1e-8) continue;
                if (d < fabs(A.r - B.r) - 1e-8) continue;
                if (d < EPS) continue;

                double x = (A.r * A.r - B.r * B.r + d * d) / (2.0 * d);
                double h2 = A.r * A.r - x * x;
                if (h2 < -1e-8) continue;
                if (h2 < 0) h2 = 0;

                double vx = (B.o.x - A.o.x) / d;
                double vy = (B.o.y - A.o.y) / d;

                Point base = {A.o.x + x * vx, A.o.y + x * vy};

                double h = sqrt(h2);

                Point p1 = {base.x - vy * h, base.y + vx * h};
                Point p2 = {base.x + vy * h, base.y - vx * h};

                if (containsPoint(A, p1) && containsPoint(B, p1)) addPoint(ans, p1);

                if (dist(p1, p2) > 1e-7) {
                    if (containsPoint(A, p2) && containsPoint(B, p2)) addPoint(ans, p2);
                }
            }
        }
    }

    sort(ans.begin(), ans.end(), [](const Point& p, const Point& q) {
        if (fabs(p.x - q.x) > 1e-9) return p.x < q.x;
        return p.y < q.y;
    });

    cout << ans.size() << '\n';
    cout << fixed << setprecision(3);

    for (Point p : ans) {
        if (fabs(p.x) < 0.0005) p.x = 0;
        if (fabs(p.y) < 0.0005) p.y = 0;
        cout << p.x << ' ' << p.y << '\n';
    }

    return 0;
}
