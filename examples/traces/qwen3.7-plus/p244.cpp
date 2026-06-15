#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    double h, d, m;
    if (!(cin >> h >> d >> m)) return 0;

    if (m < h || d < h || d > m) {
        cout << "NO\n";
        return 0;
    }

    if (h == d || d == m || h == m) {
        if (h != d || d != m) {
            cout << "NO\n";
            return 0;
        }
        cout << "YES\n";
        cout << fixed << setprecision(8);
        cout << 0.0 << " " << h << "\n";
        cout << -h / 2.0 << " 0.0\n";
        cout << h / 2.0 << " 0.0\n";
        return 0;
    }

    double xm = sqrt(m * m - h * h);
    double xd = sqrt(d * d - h * h);

    double delta = xm * xm - xm * xd + h * h * (xm - xd) / xd;
    if (delta < 0) {
        if (delta > -1e-9) {
            delta = 0;
        } else {
            cout << "NO\n";
            return 0;
        }
    }

    double xb = xm - sqrt(delta);
    double xc = 2 * xm - xb;

    cout << "YES\n";
    cout << fixed << setprecision(8);
    cout << 0.0 << " " << h << "\n";
    cout << xb << " 0.0\n";
    cout << xc << " 0.0\n";

    return 0;
}
