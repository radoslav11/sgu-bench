#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l1, l2, p;
    cin >> n >> l1 >> l2 >> p;

    __int128 C[55][55] = {};
    for (int i = 0; i <= 51; ++i) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }

    vector<__int128> pref(n + 2, 0);

    for (int k = 0; k <= n; ++k) {
        __int128 w = 0;
        if (k >= l1 && n - k >= l2) {
            w = C[k][l1] * C[n - k][l2];
        }
        pref[k + 1] = pref[k] + w;
    }

    __int128 total = pref[n + 1];

    for (int len = 0; len <= n; ++len) {
        for (int a = 0; a + len <= n; ++a) {
            int b = a + len;
            __int128 sum = pref[b + 1] - pref[a];

            if (sum * 100 >= (__int128)p * total) {
                cout << a << ' ' << b << '\n';
                return 0;
            }
        }
    }

    return 0;
}
