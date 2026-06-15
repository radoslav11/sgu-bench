#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

string s;

ll solve(int len) {
    if (len <= 0) 
        return 0;
    int n = s.size();
    vector<vector<ll>> dp(len/2 + 1, vector<ll>(2, 0));
    dp[0][0] = 1;

    for (int i = 0; i < len/2; i++) {
        for (int c = 0; c < 2; c++) {
            if (dp[i][c] == 0) 
                continue;
            int target = s[n-1-i] - '0';
            for (int a = 0; a < 10; a++) {
                if (i == 0 && a == 0) 
                    continue;
                for (int b = 0; b < 10; b++) {
                    int sum_val = a + b + c;
                    if (sum_val % 10 == target) {
                        dp[i+1][sum_val / 10] += dp[i][c];
                    }
                }
            }
        }
    }

    if (len % 2 == 1) {
        int i = len / 2;
        for (int c = 0; c < 2; c++) {
            if (dp[i][c] == 0) 
                continue;
            int target = s[n-1-i] - '0';
            for (int a = 0; a < 10; a++) {
                if (i == 0 && a == 0) 
                    continue;
                int sum_val = 2 * a + c;
                if (sum_val % 10 == target) {
                    dp[i+1][sum_val / 10] += dp[i][c];
                }
            }
        }
    }

    if (len == n) {
        return dp[len/2 + (len % 2)][0];
    } else {
        return dp[len/2 + (len % 2)][s[0] - '0'];
    }
}

int main() {
    while (cin >> s, s != "0") {
        ll ans = solve((int)s.size()) + solve((int)s.size() - 1);
        cout << ans << '\n';
    }
    return 0;
}
