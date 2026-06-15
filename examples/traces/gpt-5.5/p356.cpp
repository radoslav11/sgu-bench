#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

cpp_int gcd_cpp(cpp_int a, cpp_int b) {
    while (b != 0) {
        cpp_int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K, N;
    cin >> K >> N;

    vector<cpp_int> fact(N + 1), der(N + 1);
    fact[0] = 1;
    for (int i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * i;
    }

    der[0] = 1;
    if (N >= 1) der[1] = 0;
    for (int i = 2; i <= N; ++i) {
        der[i] = (i - 1) * (der[i - 1] + der[i - 2]);
    }

    int m = N - K;
    cpp_int numerator = der[m];

    if (numerator == 0) {
        cout << 0 << '\n';
        return 0;
    }

    cpp_int denominator = fact[K] * fact[m];
    cpp_int g = gcd_cpp(numerator, denominator);

    numerator /= g;
    denominator /= g;

    cout << numerator << '/' << denominator << '\n';

    return 0;
}
