#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, N1, N2;
    cin >> N >> N1 >> N2;

    double x1, y1, x2, y2;
    cin >> x1 >> y1;
    cin >> x2 >> y2;

    const double PI = acos(-1.0);
    double angle = 2.0 * PI / N;

    int d = (N2 - N1 + N) % N;

    complex<double> p1(x1, y1), p2(x2, y2);
    complex<double> q = polar(1.0, -d * angle);

    complex<double> center = (p2 - q * p1) / (1.0 - q);
    complex<double> radius = p1 - center;

    cout << fixed << setprecision(6);

    for (int i = 1; i <= N; ++i) {
        int diff = i - N1;
        complex<double> rot = polar(1.0, -diff * angle);
        complex<double> p = center + radius * rot;

        double x = p.real();
        double y = p.imag();

        if (fabs(x) < 0.0000005) x = 0.0;
        if (fabs(y) < 0.0000005) y = 0.0;

        cout << x << ' ' << y << '\n';
    }

    return 0;
}
