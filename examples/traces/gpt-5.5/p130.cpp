#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<cpp_int> catalan(k + 1);
    catalan[0] = 1;

    for (int n = 1; n <= k; ++n) {
        catalan[n] = 0;
        for (int i = 0; i < n; ++i) {
            catalan[n] += catalan[i] * catalan[n - 1 - i];
        }
    }

    cout << catalan[k] << ' ' << k + 1 << '\n';

    return 0;
}
