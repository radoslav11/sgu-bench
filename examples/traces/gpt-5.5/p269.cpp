#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];

    sort(b.begin(), b.end());

    vector<cpp_int> dp(k + 1);
    dp[0] = 1;

    for (int i = 0; i < n; ++i) {
        for (int j = min(i + 1, k); j >= 1; --j) {
            int choices = b[i] - (j - 1);
            if (choices > 0) {
                dp[j] += dp[j - 1] * choices;
            }
        }
    }

    cout << dp[k] << '\n';

    return 0;
}
