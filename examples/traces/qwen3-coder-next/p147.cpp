#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    long long n;
    cin >> n;
    long long p1, q1, p2, q2, p3, q3;
    cin >> p1 >> q1;
    cin >> p2 >> q2;
    cin >> p3 >> q3;

    long long dx = abs(p1 - p2);
    long long dy = abs(q1 - q2);
    long long d = max(dx, dy);
    long long T = d / 2;

    long long D = max(abs(p1 - p3), abs(q1 - q3));
    long long E = max(abs(p2 - p3), abs(q2 - q3));

    long long k0 = min((D + 2) / 3, (E + 2) / 3);

    if (k0 < T) {
        cout << "YES" << endl;
        cout << k0 << endl;
    } else {
        cout << "NO" << endl;
        cout << 2 * T << endl;
    }

    return 0;
}
