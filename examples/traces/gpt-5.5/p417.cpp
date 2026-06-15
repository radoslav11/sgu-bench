#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long double x0, y0, r;
    cin >> x0 >> y0 >> r;

    long double d2 = x0 * x0 + y0 * y0;
    long double pi = acosl(-1.0L);

    long double mass = pi * r * r * logl(d2);

    cout << setprecision(20) << (double)mass << '\n';
    return 0;
}
