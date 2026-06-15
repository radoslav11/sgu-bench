#include <bits/stdc++.h>
using namespace std;

int main() {
    double c, b, m;
    cin >> c >> b >> m;
    
    // a^2 = (b^2 + c^2)/2 - m^2, where BC/2 = a
    double a2 = (b*b + c*c)/2.0 - m*m;
    
    if (a2 <= 0) {
        cout << "Mission impossible" << endl;
        return 0;
    }
    
    double a = sqrt(a2);
    
    // x = (c^2 - b^2) / (4a)
    double x = (c*c - b*b) / (4.0 * a);
    
    double y2 = m*m - x*x;
    
    if (y2 < 0) {
        cout << "Mission impossible" << endl;
        return 0;
    }
    
    double y = sqrt(y2);
    
    if (y < 1e-9) {
        cout << "Mission impossible" << endl;
        return 0;
    }
    
    // A = (x, y), B = (-a, 0), C = (a, 0)
    cout << fixed << setprecision(5);
    cout << x << " " << y << "\n";
    cout << -a << " " << 0.0 << "\n";
    cout << a << " " << 0.0 << "\n";
    
    return 0;
}
