#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<long long> s(n + 1);
    for (int i = 1; i <= n; i++) s[i] = a[i];
    sort(s.begin() + 1, s.end());

    bool alreadySorted = true;
    for (int i = 1; i <= n; i++) {
        if (a[i] != s[i]) {
            alreadySorted = false;
            break;
        }
    }

    if (alreadySorted) {
        cout << 0 << '\n';
        return 0;
    }

    map<pair<long long, long long>, vector<int>> need;
    for (int i = 1; i <= n; i++) {
        if (a[i] != s[i]) {
            need[{a[i], s[i]}].push_back(i);
        }
    }

    bool canOneStep = true;
    for (auto &it : need) {
        auto key = it.first;
        auto rev = make_pair(key.second, key.first);

        if (!need.count(rev) || need[rev].size() != it.second.size()) {
            canOneStep = false;
            break;
        }
    }

    if (canOneStep) {
        vector<pair<int, int>> swaps;
        set<pair<long long, long long>> used;

        for (auto &it : need) {
            auto key = it.first;
            if (used.count(key)) continue;

            auto rev = make_pair(key.second, key.first);
            used.insert(key);
            used.insert(rev);

            if (key == rev) continue;

            auto &v1 = need[key];
            auto &v2 = need[rev];

            for (size_t i = 0; i < v1.size(); i++) {
                swaps.push_back({v1[i], v2[i]});
            }
        }

        cout << 1 << '\n';
        cout << swaps.size();
        for (auto [x, y] : swaps) {
            cout << ' ' << x << ' ' << y;
        }
        cout << '\n';
        return 0;
    }

    map<long long, queue<int>> pos;
    for (int i = 1; i <= n; i++) {
        pos[s[i]].push(i);
    }

    vector<int> p(n + 1);
    for (int i = 1; i <= n; i++) {
        p[i] = pos[a[i]].front();
        pos[a[i]].pop();
    }

    vector<int> used(n + 1, 0);
    vector<pair<int, int>> round1, round2;

    for (int i = 1; i <= n; i++) {
        if (used[i] || p[i] == i) continue;

        vector<int> cyc;
        int cur = i;

        while (!used[cur]) {
            used[cur] = 1;
            cyc.push_back(cur);
            cur = p[cur];
        }

        int m = (int)cyc.size();

        for (int t = 1; t < m; t++) {
            int u = (m - t) % m;
            if (t < u) {
                round1.push_back({cyc[t], cyc[u]});
            }
        }

        for (int t = 0; t < m; t++) {
            int u = (1 - t) % m;
            if (u < 0) u += m;
            if (t < u) {
                round2.push_back({cyc[t], cyc[u]});
            }
        }
    }

    cout << 2 << '\n';

    cout << round1.size();
    for (auto [x, y] : round1) {
        cout << ' ' << x << ' ' << y;
    }
    cout << '\n';

    cout << round2.size();
    for (auto [x, y] : round2) {
        cout << ' ' << x << ' ' << y;
    }
    cout << '\n';

    return 0;
}
