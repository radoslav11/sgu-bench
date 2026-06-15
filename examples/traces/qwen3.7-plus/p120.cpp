#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, N1, N2;
    if (!(cin >> N >> N1 >> N2)) return 0;

    long double x1, y1, x2, y2;
    cin >> x1 >> y1;
    cin >> x2 >> y2;

    int d = N2 - N1;
    if (d <= 0) d += N;

    const long double PI = acos(-1.0L);
    long double alpha = d * 2.0L * PI / N;
    long double cos_a = cos(alpha);
    long double sin_a = sin(alpha);

    long double A = 1.0L - cos_a;
    long double B = sin_a;

    long double R1 = x2 - x1 * cos_a - y1 * sin_a;
    long double R2 = y2 + x1 * sin_a - y1 * cos_a;

    long double denom = A * A + B * B;
    long double xc = (A * R1 + B * R2) / denom;
    long double yc = (A * R2 - B * R1) / denom;

    long double phi = (N1 - 1) * 2.0L * PI / N;
    long double cos_phi = cos(phi);
    long double sin_phi = sin(phi);

    long double v1_rel_x = (x1 - xc) * cos_phi - (y1 - yc) * sin_phi;
    long double v1_rel_y = (x1 - xc) * sin_phi + (y1 - yc) * cos_phi;

    cout << fixed << setprecision(6);

    for (int k = 1; k <= N; ++k) {
        long double theta = (k - 1) * 2.0L * PI / N;
        long double cos_t = cos(theta);
        long double sin_t = sin(theta);

        long double vx = xc + v1_rel_x * cos_t + v1_rel_y * sin_t;
        long double vy = yc - v1_rel_x * sin_t + v1_rel_y * cos_t;

        if (vx > -1e-11L && vx < 1e-11L) vx = 0.0L;
        if (vy > -1e-11L && vy < 1e-11L) vy = 0.0L;

        cout << vx << " " << vy << "\n";
    }

    return 0;
}
