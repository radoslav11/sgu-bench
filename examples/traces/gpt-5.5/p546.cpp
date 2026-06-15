#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b;
    cin >> n >> a >> b;

    string s;
    cin >> s;

    if (a + b > n) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> target = {a, b, n - a - b};
    vector<vector<int>> pos(3);

    for (int i = 0; i < n; i++) {
        pos[s[i] - '0'].push_back(i);
    }

    string ans = s;
    vector<int> surplus;
    vector<int> deficit;

    for (int d = 0; d < 3; d++) {
        int keep = min((int)pos[d].size(), target[d]);

        for (int i = keep; i < (int)pos[d].size(); i++) {
            surplus.push_back(pos[d][i]);
        }

        for (int i = keep; i < target[d]; i++) {
            deficit.push_back(d);
        }
    }

    for (int i = 0; i < (int)surplus.size(); i++) {
        ans[surplus[i]] = char('0' + deficit[i]);
    }

    cout << surplus.size() << '\n';
    cout << ans << '\n';

    return 0;
}
