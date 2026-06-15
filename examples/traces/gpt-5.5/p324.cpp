#include <bits/stdc++.h>
using namespace std;

bool isLetter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

struct Choice {
    bool ok = false;
    string token;
    int next = -1;
    bool isDash = false;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    string line;
    getline(cin, line);

    while (N--) {
        getline(cin, line);

        string s;
        for (int i = 1; i + 1 < (int)line.size(); ++i) {
            if (line[i] != ' ')
                s += line[i];
        }

        int n = (int)s.size();
        vector<int> memo(n + 1, -1);
        vector<Choice> choice(n + 1);

        function<bool(int)> dfs = [&](int pos) -> bool {
            if (pos == n) return true;
            if (memo[pos] != -1) return memo[pos];

            // Try a word: [A-Za-z]+(-[A-Za-z]+)*
            if (pos < n && isLetter(s[pos])) {
                bool bad = false;
                for (int j = pos; j < n && !bad; ++j) {
                    if (isLetter(s[j])) {
                        string token = s.substr(pos, j - pos + 1);
                        if (dfs(j + 1)) {
                            choice[pos] = {true, token, j + 1, false};
                            return memo[pos] = 1;
                        }
                    } else if (s[j] == '-') {
                        if (j == pos || s[j - 1] == '-') {
                            bad = true;
                        }
                    } else {
                        bad = true;
                    }
                }
            }

            // Try short dash "--"
            if (pos + 2 <= n && s.substr(pos, 2) == "--") {
                if (dfs(pos + 2)) {
                    choice[pos] = {true, "--", pos + 2, true};
                    return memo[pos] = 1;
                }
            }

            // Try long dash "---"
            if (pos + 3 <= n && s.substr(pos, 3) == "---") {
                if (dfs(pos + 3)) {
                    choice[pos] = {true, "---", pos + 3, true};
                    return memo[pos] = 1;
                }
            }

            return memo[pos] = 0;
        };

        if (!dfs(0)) {
            cout << "error\n";
            continue;
        }

        vector<pair<string, bool>> tokens;
        for (int pos = 0; pos < n; pos = choice[pos].next) {
            tokens.push_back({choice[pos].token, choice[pos].isDash});
        }

        string ans;
        if (!tokens.empty() && tokens[0].second)
            ans += ' ';

        for (int i = 0; i < (int)tokens.size(); ++i) {
            if (i > 0) ans += ' ';
            ans += tokens[i].first;
        }

        if (!tokens.empty() && tokens.back().second)
            ans += ' ';

        cout << '"' << ans << '"' << '\n';
    }

    return 0;
}
