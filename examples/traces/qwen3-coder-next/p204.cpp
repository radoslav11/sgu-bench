#include <iostream>
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

double evaluate(double x, double b1, double t1, double b2, double t2, double l, double ds, double df, double g) {
    double D1 = x;
    double d1 = ds;
    double u1_low = b1 * D1 / (d1 * (D1 - d1));
    double u1_high = t1 * D1 / (d1 * (D1 - d1));

    double D2 = ds + l + df - x;
    double d2 = ds + l - x;
    double u2_low = b2 * D2 / (d2 * df);
    double u2_high = t2 * D2 / (d2 * df);

    double f1;
    if (u1_low <= 1.0 && 1.0 <= u1_high) {
        f1 = 2.0;
    } else if (u1_high < 1.0) {
        f1 = u1_high + 1.0 / u1_high;
    } else {
        f1 = u1_low + 1.0 / u1_low;
    }

    double f2;
    if (u2_low <= 1.0 && 1.0 <= u2_high) {
        f2 = 2.0;
    } else if (u2_high < 1.0) {
        f2 = u2_high + 1.0 / u2_high;
    } else {
        f2 = u2_low + 1.0 / u2_low;
    }

    double v1 = sqrt((g * D1 / 2.0) * f1);
    double v2 = sqrt((g * D2 / 2.0) * f2);

    return max(v1, v2);
}

int main() {
    double b1, t1, b2, t2, l, ds, df, g;
    while (cin >> b1 >> t1 >> b2 >> t2 >> l >> ds >> df >> g) {
        double lo_x = ds + 1e-12;
        double hi_x = ds + l - 1e-12;

        for (int iter = 0; iter < 100; iter++) {
            double m1 = lo_x + (hi_x - lo_x) / 3.0;
            double m2 = hi_x - (hi_x - lo_x) / 3.0;
            double v1 = evaluate(m1, b1, t1, b2, t2, l, ds, df, g);
            double v2 = evaluate(m2, b1, t1, b2, t2, l, ds, df, g);
            if (v1 < v2) {
                hi_x = m2;
            } else {
                lo_x = m1;
            }
        }

        double ans = evaluate((lo_x + hi_x) * 0.5, b1, t1, b2, t2, l, ds, df, g);
        printf("%.4f\n", ans);
    }
    return 0;
}
