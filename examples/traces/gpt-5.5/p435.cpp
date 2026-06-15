#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-10;

struct Circle {
    double x, y, r;
};

double normAng(double a) {
    while (a < 0) a += 2 * PI;
    while (a >= 2 * PI) a -= 2 * PI;
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<Circle> c(N);
    for (int i = 0; i < N; ++i) {
        cin >> c[i].x >> c[i].y >> c[i].r;
    }

    vector<double> area_ge(N + 2, 0.0);

    for (int i = 0; i < N; ++i) {
        vector<double> ang;
        ang.push_back(0.0);
        ang.push_back(2 * PI);

        for (int j = 0; j < N; ++j) {
            if (i == j) continue;

            double dx = c[j].x - c[i].x;
            double dy = c[j].y - c[i].y;
            double d = hypot(dx, dy);

            if (d < EPS) continue;
            if (d > c[i].r + c[j].r + EPS) continue;
            if (d < fabs(c[i].r - c[j].r) - EPS) continue;

            double base = atan2(dy, dx);
            double val = (c[i].r * c[i].r + d * d - c[j].r * c[j].r) / (2.0 * c[i].r * d);
            val = max(-1.0, min(1.0, val));
            double delta = acos(val);

            ang.push_back(normAng(base - delta));
            ang.push_back(normAng(base + delta));
        }

        sort(ang.begin(), ang.end());

        vector<double> a;
        for (double v : ang) {
            if (a.empty() || fabs(v - a.back()) > 1e-9) {
                a.push_back(v);
            }
        }

        for (int k = 0; k + 1 < (int)a.size(); ++k) {
            double l = a[k], r = a[k + 1];
            if (r - l < EPS) continue;

            double m = (l + r) / 2.0;
            double px = c[i].x + c[i].r * cos(m);
            double py = c[i].y + c[i].r * sin(m);

            int cnt = 0;
            for (int j = 0; j < N; ++j) {
                double dx = px - c[j].x;
                double dy = py - c[j].y;
                if (dx * dx + dy * dy <= c[j].r * c[j].r + 1e-7) {
                    ++cnt;
                }
            }

            double contrib = 0.5 * (
                c[i].r * c[i].r * (r - l)
                + c[i].r * c[i].x * (sin(r) - sin(l))
                - c[i].r * c[i].y * (cos(r) - cos(l))
            );

            area_ge[cnt] += contrib;
        }
    }

    double burned = 0.0, inverse = 0.0;

    for (int k = 1; k <= N; ++k) {
        double exact = area_ge[k] - area_ge[k + 1];
        if (k % 2 == 1) burned += exact;
        else inverse += exact;
    }

    cout.setf(ios::fixed);
    cout << setprecision(5) << burned << ' ' << inverse << '\n';

    return 0;
}
