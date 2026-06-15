#include <iostream>
#include <numeric>
#include <cmath>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long l, v1, v2, t;
    if (cin >> l >> v1 >> v2 >> t) {
        long long sum_v = v1 + v2;
        long long diff_v = abs(v1 - v2);

        long long n1 = (sum_v * t + l) / (2 * l);
        long long n2 = (diff_v * t + l) / (2 * l);

        long long g = std::gcd(sum_v, diff_v);
        long long A = sum_v / g;
        long long B = diff_v / g;

        long long n12 = 0;
        if (A % 2 != 0 && B % 2 != 0) {
            long long M = (g * t) / l;
            n12 = (M + 1) / 2;
        }

        long long ans = n1 + n2 - n12;
        cout << ans << "\n";
    }

    return 0;
}
