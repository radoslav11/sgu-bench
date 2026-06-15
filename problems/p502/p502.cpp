#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

string s;

void read() { cin >> s; }

void solve() {
    // Bitmask DP over which digit positions have already been placed.
    //
    // - dp[mask][r] = the smallest number we can build by placing exactly
    //   the digits in mask, whose value modulo 17 equals r. We extend a
    //   state by appending one not-yet-used digit, keeping the minimal
    //   value so the final answer is the smallest valid permutation.
    //
    // - To avoid a leading zero we forbid placing a '0' as the very first
    //   digit (when no non-zero digit has been placed yet, i.e. the count
    //   of non-zero digits already in mask is 0).
    //
    // - The answer is dp[full][0]; if it stays at infinity no permutation
    //   is divisible by 17 and we print -1.

    if(s == "0") {
        cout << s << '\n';
        return;
    }

    int n = s.size();
    vector<vector<int64_t>> dp(
        1 << n, vector<int64_t>(17, numeric_limits<int64_t>::max())
    );
    dp[0][0] = 0;
    for(int mask = 0; mask < (1 << n); mask++) {
        int cnt_non_zero = 0;
        for(int i = 0; i < n; i++) {
            if((mask & (1 << i)) && s[i] != '0') {
                cnt_non_zero++;
            }
        }

        for(int i = 0; i < n; i++) {
            if((s[i] == '0' && cnt_non_zero == 0) || (mask & (1 << i))) {
                continue;
            }

            for(int j = 0; j < 17; j++) {
                if(dp[mask][j] == numeric_limits<int64_t>::max()) {
                    continue;
                }
                int nmask = mask | (1 << i);
                int nval = (j * 10 + s[i] - '0') % 17;
                dp[nmask][nval] =
                    min(dp[nmask][nval], dp[mask][j] * 10 + s[i] - '0');
            }
        }
    }

    int64_t ans = dp[(1 << n) - 1][0];
    if(ans == numeric_limits<int64_t>::max()) {
        cout << "-1\n";
        return;
    }

    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
