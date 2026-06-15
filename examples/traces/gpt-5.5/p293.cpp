#include <bits/stdc++.h>
using namespace std;

string canonical(int q, int c) {
    char center = (q % 2 ? 'Q' : 'C');
    int q2 = q / 2, c2 = c / 2;

    string s;
    s.append(q2, 'Q');
    s.append(c2, 'C');
    s.push_back(center);
    s.append(c2, 'C');
    s.append(q2, 'Q');
    return s;
}

vector<pair<int,int>> get_swaps(string s, const string& t) {
    int n = (int)s.size();
    vector<pair<int,int>> res;

    for (int i = 0; i < n; i++) {
        if (s[i] == t[i]) continue;

        int j = -1;
        for (int k = i + 1; k < n; k++) {
            if (s[k] == t[i] && s[k] != t[k]) {
                j = k;
                break;
            }
        }

        if (j == -1) {
            for (int k = i + 1; k < n; k++) {
                if (s[k] == t[i]) {
                    j = k;
                    break;
                }
            }
        }

        if (j == -1) return vector<pair<int,int>>(3, {0, 0});

        swap(s[i], s[j]);
        res.push_back({i + 1, j + 1});

        if ((int)res.size() > 2) return res;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string he;

    cin >> n >> he;

    vector<pair<int,int>> ans;
    ans.push_back({0, 0});

    int q = (he[0] == 'Q');
    int c = (he[0] == 'C');

    string cur = canonical(q, c);

    bool ok = true;

    for (int len = 1; len < 2 * n - 1; len += 2) {
        char a = he[len];
        char b = he[len + 1];

        int nq = q + (a == 'Q') + (b == 'Q');
        int nc = c + (a == 'C') + (b == 'C');

        string target = canonical(nq, nc);
        string full = cur + a + b;

        vector<pair<int,int>> sw = get_swaps(full, target);

        if ((int)sw.size() > 2) {
            ok = false;
            break;
        }

        pair<int,int> evenMove = {0, 0};
        pair<int,int> oddMove = {0, 0};

        int newPos = len + 2;

        if (sw.size() == 1) {
            if (sw[0].first == newPos || sw[0].second == newPos) {
                oddMove = sw[0];
            } else {
                evenMove = sw[0];
            }
        } else if (sw.size() == 2) {
            bool firstUsesNew = (sw[0].first == newPos || sw[0].second == newPos);
            bool secondUsesNew = (sw[1].first == newPos || sw[1].second == newPos);

            if (!firstUsesNew) {
                evenMove = sw[0];
                oddMove = sw[1];
            } else if (!secondUsesNew) {
                evenMove = sw[1];
                oddMove = sw[0];
            } else {
                ok = false;
                break;
            }
        }

        ans.push_back(evenMove);
        ans.push_back(oddMove);

        cur = target;
        q = nq;
        c = nc;
    }

    if (!ok) {
        cout << "He\n";
        return 0;
    }

    cout << "Qc\n";
    for (auto [x, y] : ans) {
        cout << x << ' ' << y << '\n';
    }

    return 0;
}
