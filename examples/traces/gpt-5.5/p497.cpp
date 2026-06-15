#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    const int MAX_EXP = 40;

    vector<cpp_int> part(MAX_EXP + 1);
    part[0] = 1;

    for (int x = 1; x <= MAX_EXP; ++x) {
        for (int s = x; s <= MAX_EXP; ++s) {
            part[s] += part[s - x];
        }
    }

    cpp_int answer = 1;

    for (long long d = 2; d * d <= n; d += (d == 2 ? 1 : 2)) {
        if (n % d == 0) {
            int exponent = 0;
            while (n % d == 0) {
                n /= d;
                ++exponent;
            }
            answer *= part[exponent];
        }
    }

    if (n > 1) {
        answer *= part[1];
    }

    cout << answer << '\n';

    return 0;
}
