#include <bits/stdc++.h>
using namespace std;

string revstr(string s) {
    reverse(s.begin(), s.end());
    return s;
}

bool containsStr(const string& a, const string& b) {
    return a.find(b) != string::npos;
}

int overlapLen(const string& a, const string& b) {
    int mx = min(a.size(), b.size());
    for (int k = mx; k >= 0; --k) {
        bool ok = true;
        for (int i = 0; i < k; ++i) {
            if (a[(int)a.size() - k + i] != b[i]) {
                ok = false;
                break;
            }
        }
        if (ok) return k;
    }
    return 0;
}

bool isPalSuffix(const string& s, int start) {
    int l = start, r = (int)s.size() - 1;
    while (l < r) {
        if (s[l++] != s[r--]) return false;
    }
    return true;
}

string makePalindrome(const string& s) {
    int n = s.size();
    int best = 0;
    for (int i = 0; i < n; ++i) {
        if (isPalSuffix(s, i)) {
            best = n - i;
            break;
        }
    }
    string add = s.substr(0, n - best);
    reverse(add.begin(), add.end());
    return s + add;
}

struct Parent {
    int pmask = -1;
    int plast = -1;
    int por = -1;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<string> input(N);
    for (int i = 0; i < N; ++i) cin >> input[i];

    sort(input.begin(), input.end());
    input.erase(unique(input.begin(), input.end()), input.end());

    sort(input.begin(), input.end(), [](const string& a, const string& b) {
        if (a.size() != b.size()) return a.size() > b.size();
        return a < b;
    });

    vector<string> words;
    for (const string& w : input) {
        bool redundant = false;
        string rw = revstr(w);
        for (const string& kept : words) {
            if (containsStr(kept, w) || containsStr(revstr(kept), w)) {
                redundant = true;
                break;
            }
        }
        if (!redundant) words.push_back(w);
    }

    int K = words.size();

    vector<array<string, 2>> s(K);
    for (int i = 0; i < K; ++i) {
        s[i][0] = words[i];
        s[i][1] = revstr(words[i]);
    }

    if (K == 0) {
        cout << "\n";
        return 0;
    }

    vector<vector<vector<int>>> ov(K, vector<vector<int>>(2, vector<int>(K * 2)));
    for (int i = 0; i < K; ++i) {
        for (int oi = 0; oi < 2; ++oi) {
            for (int j = 0; j < K; ++j) {
                for (int oj = 0; oj < 2; ++oj) {
                    ov[i][oi][j * 2 + oj] = overlapLen(s[i][oi], s[j][oj]);
                }
            }
        }
    }

    int M = 1 << K;
    const int INF = 1e9;

    vector<vector<array<int, 2>>> dp(M, vector<array<int, 2>>(K));
    vector<vector<array<Parent, 2>>> par(M, vector<array<Parent, 2>>(K));

    for (int mask = 0; mask < M; ++mask)
        for (int i = 0; i < K; ++i)
            dp[mask][i] = {INF, INF};

    for (int i = 0; i < K; ++i) {
        for (int o = 0; o < 2; ++o) {
            dp[1 << i][i][o] = s[i][o].size();
        }
    }

    for (int mask = 0; mask < M; ++mask) {
        for (int last = 0; last < K; ++last) {
            for (int lo = 0; lo < 2; ++lo) {
                if (dp[mask][last][lo] == INF) continue;

                for (int nxt = 0; nxt < K; ++nxt) {
                    if (mask & (1 << nxt)) continue;
                    for (int no = 0; no < 2; ++no) {
                        int nmask = mask | (1 << nxt);
                        int add = (int)s[nxt][no].size() - ov[last][lo][nxt * 2 + no];
                        int val = dp[mask][last][lo] + add;

                        if (val < dp[nmask][nxt][no]) {
                            dp[nmask][nxt][no] = val;
                            par[nmask][nxt][no] = {mask, last, lo};
                        }
                    }
                }
            }
        }
    }

    auto reconstruct = [&](int last, int ori) {
        vector<pair<int, int>> seq;
        int mask = M - 1;

        while (last != -1) {
            seq.push_back({last, ori});
            Parent p = par[mask][last][ori];
            mask = p.pmask;
            last = p.plast;
            ori = p.por;
        }

        reverse(seq.begin(), seq.end());

        string res = s[seq[0].first][seq[0].second];
        for (int i = 1; i < (int)seq.size(); ++i) {
            int a = seq[i - 1].first;
            int ao = seq[i - 1].second;
            int b = seq[i].first;
            int bo = seq[i].second;
            int o = overlapLen(res, s[b][bo]);
            res += s[b][bo].substr(o);
        }

        return res;
    };

    string answer;
    bool hasAnswer = false;

    int full = M - 1;
    for (int last = 0; last < K; ++last) {
        for (int ori = 0; ori < 2; ++ori) {
            if (dp[full][last][ori] == INF) continue;

            string base = reconstruct(last, ori);
            string pal = makePalindrome(base);

            if (!hasAnswer || pal.size() < answer.size()) {
                answer = pal;
                hasAnswer = true;
            }
        }
    }

    cout << answer << '\n';

    return 0;
}
