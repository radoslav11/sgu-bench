#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<double> mx(n), my(n);
    for (int i = 0; i < n; i++) cin >> mx[i] >> my[i];
    
    // Ci: offset for Vi, Vi = (-1)^(i) * V1 + C[i] (0-indexed: V[0]=V1)
    // V[0] = V1, sign = +1
    // V[i] = (-1)^i * V1 + C[i]
    vector<double> cx(n), cy(n);
    cx[0] = cy[0] = 0;
    for (int i = 1; i < n; i++) {
        cx[i] = 2*mx[i-1] - cx[i-1];
        cy[i] = 2*my[i-1] - cy[i-1];
    }
    
    // Constraint: V[n-1] + V[0] = 2*M[n-1]
    // (-1)^(n-1)*V1 + C[n-1] + V1 = 2*M[n-1]
    double v1x, v1y;
    int sign = (n % 2 == 1) ? 1 : -1; // (-1)^(n-1)
    double coeff = sign + 1.0;
    double rhsx = 2*mx[n-1] - cx[n-1];
    double rhsy = 2*my[n-1] - cy[n-1];
    
    if (abs(coeff) < 1e-9) {
        if (abs(rhsx) > 1e-6 || abs(rhsy) > 1e-6) { cout << "NO\n"; return 0; }
        v1x = 0; v1y = 0;
    } else {
        v1x = rhsx / coeff;
        v1y = rhsy / coeff;
    }
    
    cout << "YES\n";
    cout << fixed << setprecision(3);
    for (int i = 0; i < n; i++) {
        double s = (i % 2 == 0) ? 1.0 : -1.0;
        cout << s*v1x + cx[i] << " " << s*v1y + cy[i] << "\n";
    }
    return 0;
}
