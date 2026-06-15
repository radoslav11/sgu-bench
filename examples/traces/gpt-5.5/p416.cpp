#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int m = n / 2;
    int c = n + 1;

    vector<int> idx;

    for (int i = 1; i <= m; i += 2)
        idx.push_back(i);

    int startEven = (m % 2 == 0 ? m : m - 1);
    for (int i = startEven; i >= 2; i -= 2)
        idx.push_back(i);

    vector<int> ans(n + 1);

    for (int i = 0; i < m; ++i) {
        int d = 2 * idx[i] - 1;
        int sign = (i % 2 == 0 ? 1 : -1);

        int outer = (c + sign * d) / 2;
        int inner = c - outer;

        ans[2 * i + 1] = outer;
        ans[2 * i + 2] = inner;
    }

    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
