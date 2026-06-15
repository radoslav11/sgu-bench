#include <bits/stdc++.h>
using std::cout, std::cin, std::endl;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<double> x(n), y(n), r(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i] >> r[i];
    }
    
    if (n == 1) {
        cout << -2 << endl;
        return 0;
    }
    
    // Build system of linear equations
    // 2(x[i] - x[j])X + 2(y[i] - y[j])Y = (x[j]^2 + y[j]^2 - r[j]^2) - (x[i]^2 + y[i]^2 - r[i]^2)
    
    double a1 = 2 * (x[0] - x[1]);
    double b1 = 2 * (y[0] - y[1]);
    double c1 = (x[1]*x[1] + y[1]*y[1] - r[1]*r[1]) - (x[0]*x[0] + y[0]*y[0] - r[0]*r[0]);
    
    double cx, cy;
    bool unique = false;
    
    if (abs(a1) > 1e-9 || abs(b1) > 1e-9) {
        // Try to find solution from first two equations
        if (n >= 3) {
            double a2 = 2 * (x[0] - x[2]);
            double b2 = 2 * (y[0] - y[2]);
            double c2 = (x[2]*x[2] + y[2]*y[2] - r[2]*r[2]) - (x[0]*x[0] + y[0]*y[0] - r[0]*r[0]);
            
            double det = a1 * b2 - a2 * b1;
            
            if (abs(det) > 1e-9) {
                cx = (c1 * b2 - c2 * b1) / det;
                cy = (a1 * c2 - a2 * c1) / det;
                unique = true;
            } else {
                cout << -1 << endl;
                return 0;
            }
        } else {
            // n == 2, infinitely many solutions
            cout << -2 << endl;
            return 0;
        }
    } else {
        cout << -1 << endl;
        return 0;
    }
    
    // Check if this solution works for all circles
    double k = cx*cx + cy*cy - (2*x[0]*cx + 2*y[0]*cy + (r[0]*r[0] - x[0]*x[0] - y[0]*y[0]));
    
    if (k < -1e-9) {
        cout << -1 << endl;
        return 0;
    }
    
    double R = sqrt(max(0.0, k));
    
    // Verify with all circles
    for (int i = 0; i < n; i++) {
        double dist_sq = (cx - x[i]) * (cx - x[i]) + (cy - y[i]) * (cy - y[i]);
        double expected = R*R + r[i]*r[i];
        if (abs(dist_sq - expected) > 1e-6 * max(1.0, expected)) {
            cout << -1 << endl;
            return 0;
        }
    }
    
    cout << fixed << setprecision(10) << cx << " " << cy << " " << R << endl;
    
    return 0;
}
