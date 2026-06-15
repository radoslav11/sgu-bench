#include <bits/stdc++.h>
using namespace std;

struct Option {
    int t;
    vector<int> s;
};

int H, W, N;
vector<string> B;
vector<vector<Option>> doms;
vector<int> choice;
unordered_set<string> bad;

bool applyLabel(int mask, int lab, int &nmask) {
    nmask = 0;
    for (int cur = 0; cur <= 1; ++cur) {
        if (!(mask & (1 << cur))) continue;
        int nxt = lab - cur;
        if (nxt == 0 || nxt == 1) nmask |= (1 << nxt);
    }
    return nmask != 0;
}

string makeKey(int pos, const vector<unsigned char>& masks) {
    string key;
    key.reserve(1 + masks.size());
    key.push_back(char(pos));
    for (unsigned char x : masks) key.push_back(char(x));
    return key;
}

bool dfs(int pos, vector<unsigned char> masks) {
    if (pos == N) return true;

    string key = makeKey(pos, masks);
    if (bad.count(key)) return false;

    for (int k = 0; k < (int)doms[pos].size(); ++k) {
        vector<unsigned char> nmasks(W);
        bool ok = true;

        for (int j = 0; j < W; ++j) {
            int nm;
            if (!applyLabel(masks[j], doms[pos][k].s[j], nm)) {
                ok = false;
                break;
            }
            nmasks[j] = (unsigned char)nm;
        }

        if (ok) {
            choice[pos] = k;
            if (dfs(pos + 1, nmasks)) return true;
        }
    }

    bad.insert(key);
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> H >> W;
    N = H - 1;
    B.resize(N);
    for (int i = 0; i < N; ++i) cin >> B[i];

    doms.resize(N);

    for (int i = 0; i < N; ++i) {
        for (int t = 0; t <= 2; ++t) {
            vector<int> s(W);
            s[0] = t;
            bool ok = true;

            for (int j = 0; j < W - 1; ++j) {
                int b = B[i][j] - '0';
                s[j + 1] = b - s[j];
                if (s[j + 1] < 0 || s[j + 1] > 2) {
                    ok = false;
                    break;
                }
            }

            if (ok) doms[i].push_back({t, s});
        }

        if (doms[i].empty()) {
            cout << "CORRUPT\n";
            return 0;
        }
    }

    choice.assign(N, -1);
    vector<unsigned char> masks(W, 3);

    if (!dfs(0, masks)) {
        cout << "CORRUPT\n";
        return 0;
    }

    vector<vector<int>> S(N, vector<int>(W));
    for (int i = 0; i < N; ++i) {
        S[i] = doms[i][choice[i]].s;
    }

    vector<string> A(H, string(W, '0'));

    for (int j = 0; j < W; ++j) {
        int cur = -1;

        for (int start = 0; start <= 1; ++start) {
            int x = start;
            bool ok = true;
            for (int i = 0; i < N; ++i) {
                int nxt = S[i][j] - x;
                if (nxt < 0 || nxt > 1) {
                    ok = false;
                    break;
                }
                x = nxt;
            }
            if (ok) {
                cur = start;
                break;
            }
        }

        if (cur == -1) {
            cout << "CORRUPT\n";
            return 0;
        }

        A[0][j] = char('0' + cur);
        for (int i = 0; i < N; ++i) {
            int nxt = S[i][j] - cur;
            A[i + 1][j] = char('0' + nxt);
            cur = nxt;
        }
    }

    for (int i = 0; i < H; ++i) {
        cout << A[i] << '\n';
    }

    return 0;
}
