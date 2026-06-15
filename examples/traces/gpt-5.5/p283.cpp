#include <bits/stdc++.h>
using namespace std;

struct Vec {
    double x, y;

    Vec() : x(0), y(0) {}
    Vec(double x, double y) : x(x), y(y) {}

    Vec operator + (const Vec& other) const {
        return Vec(x + other.x, y + other.y);
    }

    Vec operator - (const Vec& other) const {
        return Vec(x - other.x, y - other.y);
    }

    Vec operator * (double k) const {
        return Vec(x * k, y * k);
    }

    Vec operator / (double k) const {
        return Vec(x / k, y / k);
    }
};

double dot(Vec a, Vec b) {
    return a.x * b.x + a.y * b.y;
}

struct Circle {
    Vec p, v;
    double r, m;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Circle A, B;
    double t;

    cin >> A.p.x >> A.p.y >> A.v.x >> A.v.y >> A.r >> A.m;
    cin >> B.p.x >> B.p.y >> B.v.x >> B.v.y >> B.r >> B.m;
    cin >> t;

    double R = A.r + B.r;

    Vec dp = B.p - A.p;
    Vec dv = B.v - A.v;

    double a = dot(dv, dv);
    double b = dot(dp, dv);
    double c = dot(dp, dp) - R * R;

    bool collision = false;
    double tc = 0.0;

    const double EPS = 1e-10;

    if (a > EPS && b < -EPS) {
        double disc = b * b - a * c;

        if (disc >= -EPS) {
            if (disc < 0) disc = 0;

            double root = (-b - sqrt(disc)) / a;

            if (root > EPS && root < t - EPS) {
                collision = true;
                tc = root;
            }
        }
    }

    if (!collision) {
        A.p = A.p + A.v * t;
        B.p = B.p + B.v * t;
    } else {
        A.p = A.p + A.v * tc;
        B.p = B.p + B.v * tc;

        Vec n = (B.p - A.p) / R;

        double rel = dot(A.v - B.v, n);

        Vec oldVA = A.v;
        Vec oldVB = B.v;

        A.v = oldVA - n * (2.0 * B.m / (A.m + B.m) * rel);
        B.v = oldVB + n * (2.0 * A.m / (A.m + B.m) * rel);

        double rem = t - tc;

        A.p = A.p + A.v * rem;
        B.p = B.p + B.v * rem;
    }

    cout << fixed << setprecision(3);
    cout << A.p.x << ' ' << A.p.y << ' ' << A.v.x << ' ' << A.v.y << '\n';
    cout << B.p.x << ' ' << B.p.y << ' ' << B.v.x << ' ' << B.v.y << '\n';

    return 0;
}
