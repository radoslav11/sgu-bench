#include <bits/stdc++.h>
using namespace std;

const long double PI = acosl(-1.0L);

long double clampv(long double x) {
    if (x < -1.0L) return -1.0L;
    if (x > 1.0L) return 1.0L;
    return x;
}

struct Vec {
    long double x, y, z;
};

long double norm(const Vec& v) {
    return sqrtl(v.x * v.x + v.y * v.y + v.z * v.z);
}

long double dotp(const Vec& a, const Vec& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

long double capAreaUnit(long double alpha) {
    return 2.0L * PI * (1.0L - cosl(alpha));
}

long double intersectionUnit(long double a, long double b, long double g) {
    const long double EPS = 1e-14L;

    if (g >= a + b - EPS) return 0.0L;

    if (g + min(a, b) <= max(a, b) + EPS) {
        return capAreaUnit(min(a, b));
    }

    long double A = acosl(clampv((cosl(b) - cosl(a) * cosl(g)) / (sinl(a) * sinl(g))));
    long double B = acosl(clampv((cosl(a) - cosl(b) * cosl(g)) / (sinl(b) * sinl(g))));
    long double C = acosl(clampv((cosl(g) - cosl(a) * cosl(b)) / (sinl(a) * sinl(b))));

    long double sphericalTriangleArea = A + B + C - PI;

    long double lens =
        2.0L * A * (1.0L - cosl(a)) +
        2.0L * B * (1.0L - cosl(b)) -
        2.0L * sphericalTriangleArea;

    return lens;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long double R;
    Vec p1, p2;

    if (!(cin >> R)) return 0;
    cin >> p1.x >> p1.y >> p1.z;
    cin >> p2.x >> p2.y >> p2.z;

    long double d1 = norm(p1);
    long double d2 = norm(p2);

    long double a = acosl(clampv(R / d1));
    long double b = acosl(clampv(R / d2));

    long double g = acosl(clampv(dotp(p1, p2) / (d1 * d2)));

    long double area1 = capAreaUnit(a);
    long double area2 = capAreaUnit(b);
    long double inter = intersectionUnit(a, b, g);

    long double answer = (area1 + area2 - inter) * R * R;

    cout << fixed << setprecision(3) << (double)answer << '\n';

    return 0;
}
