#include <bits/stdc++.h>
using namespace std;

int main() {
    double s, m, p;
    cin >> s >> m >> p;
    
    double x;
    if (p == 0) {
        x = s / m;
    } else {
        double r = 1.0 + p / 100.0;
        double rm = pow(r, m);
        x = s * rm * (r - 1.0) / (rm - 1.0);
    }
    
    cout << fixed << setprecision(5) << x << endl;
    return 0;
}
