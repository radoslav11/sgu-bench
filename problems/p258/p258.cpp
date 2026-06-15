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

int a, b;

map<string, int64_t> answer_for_9s = {
    {"99", 81},
    {"9999", 7389},
    {"999999", 676133},
    {"99999999", 62563644},
    {"9999999999", 1550148951}
};

int solve_dp(string bound) {
    int n = bound.size() / 2;
    assert(n * 2 == (int)bound.size());

    vector<vector<vector<vector<vector<vector<int>>>>>> dp(
        11, vector<vector<vector<vector<vector<int>>>>>(
                201, vector<vector<vector<vector<int>>>>(
                         10, vector<vector<vector<int>>>(
                                 10, vector<vector<int>>(2, vector<int>(2, -1))
                             )
                     )
            )
    );

    function<int(int, int, int, int, bool, bool)> rec =
        [&](int pos, int balance, int max_inc, int max_dec, bool tight,
            bool changed) -> int {
        if(pos == (int)bound.size()) {
            bool can_be_almost_lucky = false;
            if(balance != 0) {
                if(balance < 0 && max_inc >= -balance) {
                    can_be_almost_lucky = true;
                }

                if(balance > 0 && max_dec >= balance) {
                    can_be_almost_lucky = true;
                }
            }

            return can_be_almost_lucky ? 1 : 0;
        }

        if(dp[pos][balance + 100][max_inc][max_dec][tight][changed] != -1) {
            return dp[pos][balance + 100][max_inc][max_dec][tight][changed];
        }

        int limit = tight ? (bound[pos] - '0') : 9;
        int result = 0;

        for(int digit = (pos == 0 ? 1 : 0); digit <= limit; digit++) {
            int new_balance = balance;
            int new_max_inc = max_inc;
            int new_max_dec = max_dec;

            if(pos < n) {
                new_balance += digit;
                if(pos == 0) {
                    new_max_dec = max(new_max_dec, digit - 1);
                } else {
                    new_max_dec = max(new_max_dec, digit);
                }

                new_max_inc = max(new_max_inc, 9 - digit);
            } else {
                new_balance -= digit;
                new_max_inc = max(new_max_inc, digit);
                new_max_dec = max(new_max_dec, 9 - digit);
            }

            bool new_tight = tight && (digit == limit);
            result += rec(
                pos + 1, new_balance, new_max_inc, new_max_dec, new_tight,
                changed
            );
        }

        return dp[pos][balance + 100][max_inc][max_dec][tight][changed] =
                   result;
    };

    return rec(0, 0, 0, 0, true, false);
}

int count_up_to(int n) {
    if(n <= 0) {
        return 0;
    }

    int c_digits_n = 0;
    int tmp = n;
    while(tmp) {
        c_digits_n++;
        tmp /= 10;
    }

    int64_t ans = 0;
    string bound;
    for(int cnt_digits = 2; cnt_digits <= c_digits_n; cnt_digits += 2) {
        if(cnt_digits == c_digits_n) {
            bound = to_string(n);
            ans += solve_dp(bound);
        } else {
            bound = string(cnt_digits, '9');
            if(answer_for_9s.count(bound)) {
                ans += answer_for_9s[bound];
            } else {
                ans += solve_dp(bound);
            }
        }
    }

    return ans;
}

void read() { cin >> a >> b; }

void solve() {
    // A number with 2N digits is "lucky" when its first-half digit sum equals
    // its second-half sum, and "almost lucky" when changing exactly one digit
    // (never the leading digit to 0) can make it lucky. We count almost-lucky
    // numbers in [A, B] by a count(x) = (count in [1, x]) difference.
    //
    // count is computed with a digit DP over a fixed even length 2N. The state
    // carries: position; the running balance = (sum of placed first-half digits)
    // - (sum of placed second-half digits), offset by 100; the best single-digit
    // increase and decrease still achievable to the balance (max_inc / max_dec),
    // where a first-half digit can be raised by 9-d or lowered by d, a
    // second-half digit can be raised by d or lowered by 9-d, and the leading
    // digit can only drop to 1; plus the usual tight flag. At the end a number
    // contributes iff it is not already lucky (balance != 0) but one allowed edit
    // can cancel the balance: a negative balance fixable by max_inc, or a
    // positive one by max_dec.
    //
    // Lengths shorter than that of x are summed over all 2,4,...; full all-nines
    // bounds reuse a small precomputed table to skip recomputation.

    int result_b = count_up_to(b);
    int result_a = (a > 0) ? count_up_to(a - 1) : 0;
    cout << result_b - result_a << '\n';
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
