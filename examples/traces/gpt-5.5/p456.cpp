#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double s, p;
    int m;
    cin >> s >> m >> p;

    double r = p / 100.0;
    double x;

    if (r == 0.0) {
        x = s / m;
    } else {
        double q = pow(1.0 + r, m);
        x = s * r * q / (q - 1.0);
    }

    cout << fixed << setprecision(5) << x << '\n';

    return 0;
}
