#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    if (N == 1) {
        cout << 0 << '\n';
        cout << 0 << '\n';
        return 0;
    }

    int T = (N % 2 == 0 ? N - 1 : N);
    cout << T << '\n';

    if (N % 2 == 1) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int ans;
                if (i == j) ans = 0;
                else ans = (i + j) % N + 1;

                if (j) cout << ' ';
                cout << ans;
            }
            cout << '\n';
        }
    } else {
        int M = N - 1;
        int fixed = N - 1;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int ans;

                if (i == j) {
                    ans = 0;
                } else if (i == fixed) {
                    ans = (2 * j) % M + 1;
                } else if (j == fixed) {
                    ans = (2 * i) % M + 1;
                } else {
                    ans = (i + j) % M + 1;
                }

                if (j) cout << ' ';
                cout << ans;
            }
            cout << '\n';
        }
    }

    return 0;
}
