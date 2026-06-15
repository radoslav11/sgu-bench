#include <iostream>
#include <algorithm>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int main() {
    long long l, v1, v2, t;
    cin >> l >> v1 >> v2 >> t;

    long long total_relative = v1 + v2;
    long long count1 = (total_relative * t + l) / (2 * l);

    long long diff = abs(v1 - v2);
    long long count2 = 0;
    long long g = 0;
    long long double_count = 0;

    if (diff > 0) {
        count2 = (diff * t + l) / (2 * l);
        g = gcd(total_relative, diff);
        double_count = (t * g + l) / (2 * l);
    }

    long long ans = count1;
    if (diff > 0) {
        ans = count1 + count2 - double_count;
    }

    cout << ans << endl;

    return 0;
}
