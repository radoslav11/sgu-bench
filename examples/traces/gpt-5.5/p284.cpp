#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

struct Info {
    cpp_int len = 0;
    cpp_int cnt = 0;
    string pref, suf;
};

struct Item {
    bool terminal;
    char ch;
    int id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    string S;
    cin >> S;
    int m = (int)S.size();
    int cap = m - 1;

    vector<vector<Item>> rules(N + 1);

    for (int k = 1; k <= N; ++k) {
        int L;
        cin >> L;
        rules[k].reserve(L);

        for (int i = 0; i < L; ++i) {
            string tok;
            cin >> tok;

            if (tok == "a" || tok == "b") {
                rules[k].push_back({true, tok[0], 0});
            } else {
                rules[k].push_back({false, 0, stoi(tok)});
            }
        }
    }

    auto first_cap = [&](const string& s) {
        if ((int)s.size() <= cap) return s;
        return s.substr(0, cap);
    };

    auto last_cap = [&](const string& s) {
        if ((int)s.size() <= cap) return s;
        return s.substr((int)s.size() - cap);
    };

    auto terminal_info = [&](char c) {
        Info res;
        res.len = 1;
        res.cnt = (S.size() == 1 && S[0] == c) ? 1 : 0;
        if (cap > 0) {
            res.pref = string(1, c);
            res.suf = string(1, c);
        }
        return res;
    };

    auto cross_count = [&](const string& left_suf, const string& right_pref) {
        cpp_int res = 0;
        string t = left_suf + right_pref;
        int L = (int)left_suf.size();

        int from = max(0, L - m + 1);
        int to = min(L - 1, (int)t.size() - m);

        for (int i = from; i <= to; ++i) {
            bool ok = true;
            for (int j = 0; j < m; ++j) {
                if (t[i + j] != S[j]) {
                    ok = false;
                    break;
                }
            }
            if (ok) ++res;
        }

        return res;
    };

    vector<Info> dp(N + 1);

    for (int k = 1; k <= N; ++k) {
        Info cur;

        for (const Item& item : rules[k]) {
            Info add = item.terminal ? terminal_info(item.ch) : dp[item.id];

            cur.cnt += add.cnt;
            cur.cnt += cross_count(cur.suf, add.pref);

            cur.pref = first_cap(cur.pref + add.pref);
            cur.suf = last_cap(cur.suf + add.suf);
            cur.len += add.len;
        }

        dp[k] = cur;
    }

    cout << dp[N].cnt << '\n';

    return 0;
}
