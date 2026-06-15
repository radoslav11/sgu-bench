#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> a(n);
    int r = -1, c = -1;

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 'P') {
                r = i;
                c = j;
                a[i][j] = '.';
            }
        }
    }

    while (true) {
        if (a[r][c] == '.') {
            if (r == n - 1) {
                cout << c + 1 << '\n';
                return 0;
            }

            ++r;
        } else {
            char cur = a[r][c];
            int dir = (cur == '/') ? -1 : 1;
            int nc = c + dir;

            if (nc < 0 || nc >= m) {
                cout << -1 << '\n';
                return 0;
            }

            if (a[r][nc] != '.' && a[r][nc] != cur) {
                cout << -1 << '\n';
                return 0;
            }

            c = nc;
        }
    }

    return 0;
}
