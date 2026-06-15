#include <bits/stdc++.h>
using namespace std;

struct Token {
    string s;
    int val;
    int len;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    cin >> input;

    vector<Token> tokens;

    for (int a = 0; a <= 6; ++a) {
        int denom = 1 << a;
        int base = 64 / denom;
        int val = 0;

        for (int k = 0; k <= 6 - a; ++k) {
            val += base >> k;
            string s = "R" + to_string(denom) + string(k, '.');
            tokens.push_back({s, val, (int)s.size()});
        }
    }

    int total = 0;

    for (int i = 0; i < (int)input.size();) {
        ++i; // skip 'R'

        int denom = 0;
        while (i < (int)input.size() && isdigit(input[i])) {
            denom = denom * 10 + (input[i] - '0');
            ++i;
        }

        int base = 64 / denom;
        int val = base;
        int add = base / 2;

        while (i < (int)input.size() && input[i] == '.') {
            val += add;
            add /= 2;
            ++i;
        }

        total += val;
    }

    const int INF = 1e9;
    vector<int> dp(total + 1, INF);
    dp[0] = 0;

    for (int i = 1; i <= total; ++i) {
        for (const auto& t : tokens) {
            if (i >= t.val && dp[i - t.val] != INF) {
                dp[i] = min(dp[i], dp[i - t.val] + t.len);
            }
        }
    }

    auto better = [&](const Token& a, const Token& b, int rem) {
        int na = a.s.size(), nb = b.s.size();
        int lim = max(na, nb) + 1;

        for (int i = 0; i < lim; ++i) {
            char ca, cb;

            if (i < na) ca = a.s[i];
            else ca = (rem == a.val ? '\0' : 'R');

            if (i < nb) cb = b.s[i];
            else cb = (rem == b.val ? '\0' : 'R');

            if (ca != cb) return ca < cb;
        }

        return false;
    };

    string ans;
    int rem = total;

    while (rem > 0) {
        int best = -1;

        for (int i = 0; i < (int)tokens.size(); ++i) {
            const auto& t = tokens[i];

            if (rem >= t.val && dp[rem] == dp[rem - t.val] + t.len) {
                if (best == -1 || better(t, tokens[best], rem)) {
                    best = i;
                }
            }
        }

        ans += tokens[best].s;
        rem -= tokens[best].val;
    }

    cout << ans << '\n';

    return 0;
}
