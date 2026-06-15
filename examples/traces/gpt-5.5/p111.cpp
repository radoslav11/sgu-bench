#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/integer.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    cpp_int x = 0;
    for (char c : s) {
        x *= 10;
        x += c - '0';
    }

    cpp_int r = cpp_int(1) << ((boost::multiprecision::msb(x) + 2) / 2);

    while (true) {
        cpp_int nr = (r + x / r) / 2;
        if (nr >= r) break;
        r = nr;
    }

    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;

    cout << r << '\n';
    return 0;
}
