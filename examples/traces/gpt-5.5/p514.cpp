#include <bits/stdc++.h>
using namespace std;

struct Interval {
    double l, r;
};

double areaTri(double a, double b, double c) {
    if (a + b < c - 1e-12 || a + c < b - 1e-12 || b + c < a - 1e-12) return 1e100;
    double s = (a + b + c) / 2.0;
    double v = s * (s - a) * (s - b) * (s - c);
    if (v < 0 && v > -1e-9) v = 0;
    if (v < 0) return 1e100;
    return sqrt(v);
}

bool intersect(double l1, double r1, double l2, double r2) {
    return max(l1, l2) <= min(r1, r2) + 1e-12;
}

bool canZero(const Interval& A, const Interval& B, const Interval& C) {
    if (intersect(A.l, A.r, B.l + C.l, B.r + C.r)) return true;
    if (intersect(B.l, B.r, A.l + C.l, A.r + C.r)) return true;
    if (intersect(C.l, C.r, A.l + B.l, A.r + B.r)) return true;
    return false;
}

Interval getInterval(const vector<int>& a, int l, int r) {
    int sum = 0, mx = 0;
    for (int i = l; i < r; i++) {
        sum += a[i];
        mx = max(mx, a[i]);
    }
    int lo = max(0, 2 * mx - sum);
    return {(double)lo, (double)sum};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(2 * n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a[i + n] = a[i];
    }

    double ans = 1e100;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < i + n; j++) {
            for (int k = j + 1; k < i + n; k++) {
                Interval A = getInterval(a, i, j);
                Interval B = getInterval(a, j, k);
                Interval C = getInterval(a, k, i + n);

                if (canZero(A, B, C)) {
                    cout << fixed << setprecision(10) << 0.0 << '\n';
                    return 0;
                }

                vector<double> xs = {A.l, A.r};
                vector<double> ys = {B.l, B.r};
                vector<double> zs = {C.l, C.r};

                auto addClamped = [](vector<double>& v, double x, const Interval& I) {
                    x = max(I.l, min(I.r, x));
                    v.push_back(x);
                };

                for (double y : ys) {
                    for (double z : zs) {
                        addClamped(xs, fabs(y - z), A);
                        addClamped(xs, y + z, A);
                    }
                }

                for (double x : xs) {
                    for (double z : zs) {
                        addClamped(ys, fabs(x - z), B);
                        addClamped(ys, x + z, B);
                    }
                }

                for (double x : xs) {
                    for (double y : ys) {
                        addClamped(zs, fabs(x - y), C);
                        addClamped(zs, x + y, C);
                    }
                }

                for (double x : xs) {
                    if (x < A.l - 1e-9 || x > A.r + 1e-9) continue;
                    for (double y : ys) {
                        if (y < B.l - 1e-9 || y > B.r + 1e-9) continue;
                        for (double z : zs) {
                            if (z < C.l - 1e-9 || z > C.r + 1e-9) continue;
                            ans = min(ans, areaTri(x, y, z));
                        }
                    }
                }
            }
        }
    }

    cout << fixed << setprecision(10) << ans << '\n';
    return 0;
}
