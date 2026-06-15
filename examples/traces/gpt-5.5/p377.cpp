#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    cpp_int ans = (cpp_int(1) << N) + (cpp_int(1) << M) - 2;
    cout << ans << '\n';

    return 0;
}
