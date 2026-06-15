#include <bits/stdc++.h>
using namespace std;

const int MAXX = 10000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<bitset<MAXX + 1>> pref(n + 1), suff(n + 1);

    pref[0][0] = 1;
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] | (pref[i] << a[i]);
    }

    suff[n][0] = 1;
    for (int i = n - 1; i >= 0; --i) {
        suff[i] = suff[i + 1] | (suff[i + 1] << a[i]);
    }

    vector<int> answer;

    for (int i = 0; i < n; ++i) {
        bool canPayWithoutThisCoin = false;

        for (int s = 0; s <= x; ++s) {
            if (pref[i][s] && suff[i + 1][x - s]) {
                canPayWithoutThisCoin = true;
                break;
            }
        }

        if (!canPayWithoutThisCoin) {
            answer.push_back(a[i]);
        }
    }

    cout << answer.size() << '\n';
    for (int v : answer) {
        cout << v << ' ';
    }
    cout << '\n';

    return 0;
}
