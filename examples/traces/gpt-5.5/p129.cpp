#include <bits/stdc++.h>
using namespace std;

struct Point {
    long double x, y;
};

Point operator+(const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y};
}

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

Point operator*(const Point& a, long double k) {
    return {a.x * k, a.y * k};
}

long double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

long double dist(const Point& a, const Point& b) {
    long double dx = a.x - b.x;
    long double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<Point> poly(N);
    Point center{0, 0};

    for (int i = 0; i < N; ++i) {
        cin >> poly[i].x >> poly[i].y;
        center.x += poly[i].x;
        center.y += poly[i].y;
    }

    center.x /= N;
    center.y /= N;

    sort(poly.begin(), poly.end(), [&](const Point& a, const Point& b) {
        return atan2(a.y - center.y, a.x - center.x) <
               atan2(b.y - center.y, b.x - center.x);
    });

    long double area = 0;
    for (int i = 0; i < N; ++i) {
        area += cross(poly[i], poly[(i + 1) % N]);
    }

    if (area < 0) {
        reverse(poly.begin(), poly.end());
    }

    int M;
    cin >> M;

    const long double EPS = 1e-12;

    cout << fixed << setprecision(2);

    while (M--) {
        Point A, B;
        cin >> A.x >> A.y >> B.x >> B.y;

        Point D = B - A;
        long double len = dist(A, B);

        if (len < EPS) {
            cout << 0.0 << '\n';
            continue;
        }

        long double L = 0.0, R = 1.0;
        bool empty = false;

        for (int i = 0; i < N; ++i) {
            Point P = poly[i];
            Point Q = poly[(i + 1) % N];
            Point E = Q - P;

            long double c = cross(E, A - P);
            long double d = cross(E, D);

            if (fabsl(d) < EPS) {
                if (c <= EPS) {
                    empty = true;
                    break;
                }
            } else {
                long double t = -c / d;

                if (d > 0) {
                    L = max(L, t);
                } else {
                    R = min(R, t);
                }

                if (L - R > EPS) {
                    empty = true;
                    break;
                }
            }
        }

        if (empty || R - L <= EPS) {
            cout << 0.0 << '\n';
        } else {
            cout << (double)((R - L) * len) << '\n';
        }
    }

    return 0;
}
