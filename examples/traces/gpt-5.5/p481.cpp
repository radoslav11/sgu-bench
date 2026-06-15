#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    cpp_int term = 1;

    // term for j = 0 is (n - 1)!
    for (int i = 2; i <= n - 1; ++i) {
        term *= i;
    }

    cpp_int sum = 0;

    // Sum over j = 0 .. n - 3:
    // term_j = n! / j! * n^(j-1)
    for (int j = 0; j <= n - 3; ++j) {
        sum += term;

        if (j < n - 3) {
            term *= n;
            term /= (j + 1);
        }
    }

    cpp_int answer = sum / 2;
    cout << answer << '\n';

    return 0;
}
