#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const ull INF = 4000000000000000000ULL;
    const int MAXL = 100;

    // cnt[len][state]:
    // number of valid digit sequences of length len that can be appended
    // when the previous digit is '1' iff state == 1.
    vector<array<ull, 2>> cnt(MAXL + 1);
    cnt[0][0] = cnt[0][1] = 1;

    for (int len = 1; len <= MAXL; ++len) {
        for (int state = 0; state <= 1; ++state) {
            __int128 sum = 0;
            for (int d = 0; d <= 9; ++d) {
                if (state == 1 && d == 3) continue;
                sum += cnt[len - 1][d == 1];
            }
            cnt[len][state] = (sum > INF ? INF : (ull)sum);
        }
    }

    auto count_len = [&](int len) -> ull {
        __int128 sum = 0;
        for (int d = 1; d <= 9; ++d) {
            sum += cnt[len - 1][d == 1];
        }
        return (sum > INF ? INF : (ull)sum);
    };

    int T;
    cin >> T;

    while (T--) {
        ull N;
        cin >> N;

        int len = 1;
        while (true) {
            ull c = count_len(len);
            if (N <= c) break;
            N -= c;
            ++len;
        }

        string ans;
        int prevIsOne = 0;

        for (int pos = 0; pos < len; ++pos) {
            int start = (pos == 0 ? 1 : 0);
            int rem = len - pos - 1;

            for (int d = start; d <= 9; ++d) {
                if (prevIsOne && d == 3) continue;

                ull c = cnt[rem][d == 1];
                if (N > c) {
                    N -= c;
                } else {
                    ans.push_back(char('0' + d));
                    prevIsOne = (d == 1);
                    break;
                }
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
