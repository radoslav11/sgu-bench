#include <bits/stdc++.h>
using namespace std;

struct Pt {
    long double x, y;
    bool operator<(const Pt& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Pt& other) const {
        return x == other.x && y == other.y;
    }
};

long double cross(const Pt& a, const Pt& b) {
    return a.x * b.y - a.y * b.x;
}

long double cross(const Pt& a, const Pt& b, const Pt& c) {
    return cross({b.x - a.x, b.y - a.y}, {c.x - a.x, c.y - a.y});
}

long double dot(const Pt& a, const Pt& b) {
    return a.x * b.x + a.y * b.y;
}

long double norm2(const Pt& a) {
    return dot(a, a);
}

long double dist(const Pt& a) {
    return sqrtl(norm2(a));
}

vector<Pt> convexHull(vector<Pt> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    if (p.size() <= 1) return p;

    vector<Pt> lower, upper;

    for (const Pt& pt : p) {
        while (lower.size() >= 2 &&
               cross(lower[lower.size() - 2], lower.back(), pt) <= 0)
            lower.pop_back();
        lower.push_back(pt);
    }

    for (int i = (int)p.size() - 1; i >= 0; --i) {
        const Pt& pt = p[i];
        while (upper.size() >= 2 &&
               cross(upper[upper.size() - 2], upper.back(), pt) <= 0)
            upper.pop_back();
        upper.push_back(pt);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

long double distOriginSegment(const Pt& a, const Pt& b) {
    Pt ab{b.x - a.x, b.y - a.y};
    long double len2 = norm2(ab);

    if (len2 == 0) return dist(a);

    long double t = -dot(a, ab) / len2;
    t = max((long double)0, min((long double)1, t));

    Pt closest{a.x + ab.x * t, a.y + ab.y * t};
    return dist(closest);
}

// Returns signed distance from origin to convex hull:
// positive if origin is outside,
// zero if origin is on the hull,
// negative if origin is strictly inside.
long double signedDistanceToHull(vector<Pt> points) {
    vector<Pt> h = convexHull(points);
    int n = (int)h.size();

    if (n == 1) {
        return dist(h[0]);
    }

    if (n == 2) {
        long double d = distOriginSegment(h[0], h[1]);
        return d;
    }

    const long double EPS = 1e-18L;

    bool insideOrOn = true;
    bool onBoundary = false;
    long double minDist = 1e100L;

    Pt origin{0, 0};

    for (int i = 0; i < n; ++i) {
        Pt a = h[i];
        Pt b = h[(i + 1) % n];

        minDist = min(minDist, distOriginSegment(a, b));

        long double cr = cross(a, b, origin);

        if (cr < -EPS) {
            insideOrOn = false;
        }
        if (fabsl(cr) <= EPS) {
            onBoundary = true;
        }
    }

    if (insideOrOn) {
        if (onBoundary) return 0;
        return -minDist;
    }

    return minDist;
}

long double capArea(long double r, long double c) {
    const long double PI = acosl(-1.0L);

    if (c >= r) return 0;
    if (c <= -r) return PI * r * r;

    long double val = r * r * acosl(c / r) - c * sqrtl(max((long double)0, r * r - c * c));
    return val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long double r;
    cin >> r;

    int n;
    cin >> n;
    vector<Pt> java(n);
    for (int i = 0; i < n; ++i) {
        long long x, y;
        cin >> x >> y;
        java[i] = {(long double)x, (long double)y};
    }

    int m;
    cin >> m;
    vector<Pt> cpp(m);
    for (int i = 0; i < m; ++i) {
        long long x, y;
        cin >> x >> y;
        cpp[i] = {(long double)x, (long double)y};
    }

    long double dJava = signedDistanceToHull(java);
    long double dCpp = signedDistanceToHull(cpp);

    long double minArea = capArea(r, dJava);
    long double maxArea = capArea(r, -dCpp);

    cout.setf(ios::fixed);
    cout << setprecision(15) << (double)minArea << ' ' << (double)maxArea << '\n';

    return 0;
}
