#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double c, b, m;
    if (!(cin >> c >> b >> m)) return 0;

    double inner = 2.0*(b*b + c*c) - 4.0*m*m;
    if (inner < 0) {
        cout << "Mission impossible\n";
        return 0;
    }
    double a = sqrt(inner);

    if (a + b <= c || a + c <= b || b + c <= a) {
        cout << "Mission impossible\n";
        return 0;
    }

    double cosA = (c*c + b*b - a*a) / (2.0 * b * c);
    cosA = max(-1.0, min(1.0, cosA));
    double sinA = sqrt(1.0 - cosA*cosA);

    double Ax = 0.0, Ay = 0.0;
    double Bx = c,    By = 0.0;
    double Cx = b * cosA;
    double Cy = b * sinA;

    cout << fixed << setprecision(5);
    cout << Ax << " " << Ay << "\n";
    cout << Bx << " " << By << "\n";
    cout << Cx << " " << Cy << "\n";

    return 0;
}
