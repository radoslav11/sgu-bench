#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long double u, v, w, W, V, U;
    if (cin >> u >> v >> w >> W >> V >> U) {
        long double A = v * v + w * w - U * U;
        long double B = w * w + u * u - V * V;
        long double C = u * u + v * v - W * W;

        long double term = 4.0 * u * u * v * v * w * w 
                         - u * u * A * A 
                         - v * v * B * B 
                         - w * w * C * C 
                         + A * B * C;

        if (term < 0) {
            term = 0;
        }

        long double volume = sqrt(term) / 12.0;

        cout << fixed << setprecision(4) << (double)volume << "\n";
    }

    return 0;
}
