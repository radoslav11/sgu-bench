#include <bits/stdc++.h>
using namespace std;

int colorId(char c) {
    if (c == 'b') return 0;
    if (c == 'r') return 1;
    if (c == 'y') return 2;
    return 3; // 'w'
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> tok;
    string t;
    while (cin >> t) tok.push_back(t);

    auto isNumber = [](const string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (!isdigit((unsigned char)c)) return false;
        }
        return true;
    };

    int pos = -1;
    for (int i = 0; i + 3 < (int)tok.size(); ++i) {
        if (isNumber(tok[i]) && isNumber(tok[i + 1]) &&
            isNumber(tok[i + 2]) && isNumber(tok[i + 3])) {
            pos = i;
            break;
        }
    }

    if (pos == -1) return 0;

    int cnt[4];
    for (int i = 0; i < 4; ++i) cnt[i] = stoi(tok[pos + i]);

    int rule[4][4] = {};
    int idx = pos + 4;

    for (int res = 0; res < 4; ++res) {
        for (int j = 0; j < cnt[res]; ++j) {
            string p = tok[idx++];
            int a = colorId(p[0]);
            int b = colorId(p[1]);
            rule[a][b] |= (1 << res);
        }
    }

    string s = tok[idx];
    int n = (int)s.size();

    int combine[16][16] = {};
    for (int lm = 0; lm < 16; ++lm) {
        for (int rm = 0; rm < 16; ++rm) {
            int res = 0;
            for (int a = 0; a < 4; ++a) {
                if (!(lm & (1 << a))) continue;
                for (int b = 0; b < 4; ++b) {
                    if (rm & (1 << b)) {
                        res |= rule[a][b];
                    }
                }
            }
            combine[lm][rm] = res;
        }
    }

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        dp[i][i] = 1 << colorId(s[i]);
    }

    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            int mask = 0;
            for (int mid = l; mid < r; ++mid) {
                mask |= combine[dp[l][mid]][dp[mid + 1][r]];
            }
            dp[l][r] = mask;
        }
    }

    int ans = dp[0][n - 1];
    string colors = "bryw";
    string out;

    for (int i = 0; i < 4; ++i) {
        if (ans & (1 << i)) out += colors[i];
    }

    if (out.empty()) cout << "Nobody\n";
    else cout << out << '\n';

    return 0;
}
