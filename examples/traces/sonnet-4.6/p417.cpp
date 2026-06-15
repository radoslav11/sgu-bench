#include <bits/stdc++.h>
using namespace std;

int main() {
    double x0, y0, r;
    cin >> x0 >> y0 >> r;
    
    double R2 = x0*x0 + y0*y0;
    const double PI = acos(-1.0);
    double mass = PI * r * r * log(R2);
    
    printf("%.15f\n", mass);
    return 0;
}
