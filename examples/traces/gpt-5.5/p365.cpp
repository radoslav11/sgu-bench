#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int S;
    cin >> S;

    cpp_int dp[10][2] = {};
    
    for (int d = 0; d <= 9; ++d) {
        dp[d][0] = 1;
    }

    for (int pos = 2; pos <= S; ++pos) {
        cpp_int ndp[10][2] = {};

        for (int last = 0; last <= 9; ++last) {
            for (int phase = 0; phase <= 1; ++phase) {
                if (dp[last][phase] == 0) continue;

                for (int d = 0; d <= 9; ++d) {
                    if (phase == 0) {
                        if (d >= last) {
                            ndp[d][0] += dp[last][phase];
                        } else {
                            ndp[d][1] += dp[last][phase];
                        }
                    } else {
                        if (d <= last) {
                            ndp[d][1] += dp[last][phase];
                        }
                    }
                }
            }
        }

        for (int d = 0; d <= 9; ++d) {
            dp[d][0] = ndp[d][0];
            dp[d][1] = ndp[d][1];
        }
    }

    cpp_int ans = 0;
    for (int d = 0; d <= 9; ++d) {
        ans += dp[d][0] + dp[d][1];
    }

    cout << ans << '\n';

    return 0;
}
