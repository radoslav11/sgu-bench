#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double s, m, p;
    cin >> s >> m >> p;
    
    double r = p / 100.0;
    double x;
    
    if (r == 0) {
        x = s / m;
    } else {
        double factor = pow(1 + r, m);
        x = s * r * factor / (factor - 1);
    }
    
    cout << fixed << setprecision(5) << x << endl;
    
    return 0;
}
