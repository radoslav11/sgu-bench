#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N, k1, k2, p1, p2, p3;
    if (!(cin >> N >> k1 >> k2 >> p1 >> p2 >> p3)) return 0;

    if (N < p1) {
        cout << 0 << "\n";
        return 0;
    }

    long long time = k1;
    long long N_rem = N - p1;

    if (N_rem == 0) {
        cout << time << "\n";
        return 0;
    }

    if (p2 > 0 && k2 > N_rem / p2) {
        long long x = (N_rem + p2 - 1) / p2;
        time += x;
    } else {
        time += k2;
        N_rem -= k2 * p2;

        if (p3 > 0) {
            long long y = (N_rem + p3 - 1) / p3;
            time += y;
        }
    }

    cout << time << "\n";
    return 0;
}
