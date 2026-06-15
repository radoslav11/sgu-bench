#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

cpp_int central_binomial(int p) {
    cpp_int res = 1;
    for (int i = 1; i <= p; ++i) {
        res *= (p + i);
        res /= i;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    while (N--) {
        int P;
        cin >> P;

        cpp_int ans = central_binomial(P);
        ans += 2 * (P - 1);
        ans /= P;

        cout << ans << '\n';
    }

    return 0;
}
