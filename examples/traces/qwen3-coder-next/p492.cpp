#include <iostream>
#include <cstring>
#include <string>
using namespace std;

__int128 T[61];

void precompute_T() {
    T[1] = 9;
    T[2] = 89;
    for (int i = 3; i <= 60; i++) {
        T[i] = 10 * T[i-1] - T[i-2];
    }
}

string toString(__int128 x) {
    if (x == 0) return "0";
    string s = "";
    while (x > 0) {
        s = char('0' + (x % 10)) + s;
        x /= 10;
    }
    return s;
}

__int128 count(__int128 x) {
    if (x == 0) return 0;
    string s = toString(x);
    int n = s.size();
    __int128 total = 0;
    for (int len = 1; len < n; len++) {
        total += T[len];
    }

    __int128 dp[61][2][2];
    memset(dp, 0, sizeof(dp));

    for (int d = 1; d <= (s[0] - '0'); d++) {
        int state = (d == 1) ? 1 : 0;
        int tight = (d == (s[0] - '0')) ? 1 : 0;
        dp[1][state][tight] = 1;
    }

    for (int i = 1; i < n; i++) {
        for (int state = 0; state < 2; state++) {
            for (int tight = 0; tight < 2; tight++) {
                if (dp[i][state][tight] == 0) continue;
                int limit = tight ? (s[i] - '0') : 9;
                for (int d = 0; d <= limit; d++) {
                    if (state == 1 && d == 3) continue;
                    int new_state = (d == 1) ? 1 : 0;
                    int new_tight = tight && (d == limit);
                    dp[i+1][new_state][new_tight] += dp[i][state][tight];
                }
            }
        }
    }

    for (int state = 0; state < 2; state++) {
        for (int tight = 0; tight < 2; tight++) {
            total += dp[n][state][tight];
        }
    }
    return total;
}

int main() {
    precompute_T();
    int T_cases;
    cin >> T_cases;
    while (T_cases--) {
        unsigned long long N;
        cin >> N;
        __int128 low = 1;
        __int128 high = 1;
        while (count(high) < (__int128)N) {
            high *= 10;
            if (high > (__int128)1e20) {
                break;
            }
        }
        while (low < high) {
            __int128 mid = (low + high) / 2;
            if (count(mid) >= (__int128)N) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        cout << toString(low) << endl;
    }
    return 0;
}
