#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> score(n, 0);

    for (int game = 0; game < m; ++game) {
        int a, b;
        cin >> a >> b;

        int realSign = (a > b) - (a < b);
        int realDiff = a - b;

        for (int i = 0; i < n; ++i) {
            int x, y;
            cin >> x >> y;

            int predSign = (x > y) - (x < y);
            int predDiff = x - y;

            if (predSign == realSign) score[i] += 2;
            if (predDiff == realDiff) score[i] += 3;
            if (x == a) score[i] += 1;
            if (y == b) score[i] += 1;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << score[i];
    }
    cout << '\n';

    return 0;
}
