#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

double get_v2(double ds, double x, double b, double t, double g) {
    if (x <= 1e-9) return 1e18;
    double A = (ds * x) / (ds + x);
    if (A <= 1e-9) return 1e18;
    double k_min = b / A;
    double k_max = t / A;
    
    if (k_min > k_max) return 1e18;
    
    double k_star = 1.0;
    if (k_star < k_min) k_star = k_min;
    else if (k_star > k_max) k_star = k_max;
    
    double v2 = g * (ds + x) * (1.0 + k_star * k_star) / (2.0 * k_star);
    return v2;
}

void solve() {
    double b1, t1, b2, t2, l, ds, df, g;
    while (cin >> b1 >> t1 >> b2 >> t2 >> l >> ds >> df >> g) {
        double low = 1e-7;
        double high = l;
        for (int iter = 0; iter < 200; ++iter) {
            double m1 = low + (high - low) / 3.0;
            double m2 = high - (high - low) / 3.0;
            
            double v1_m1 = max(get_v2(ds, m1, b1, t1, g), get_v2(l - m1, df, b2, t2, g));
            double v1_m2 = max(get_v2(ds, m2, b1, t1, g), get_v2(l - m2, df, b2, t2, g));
            
            if (v1_m1 < v1_m2) {
                high = m2;
            } else {
                low = m1;
            }
        }
        
        double ans_v2 = max(get_v2(ds, low, b1, t1, g), get_v2(l - low, df, b2, t2, g));
        if (ans_v2 > 1e17) {
            cout << -1 << "\n";
        } else {
            cout << fixed << setprecision(4) << sqrt(ans_v2) << "\n";
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
