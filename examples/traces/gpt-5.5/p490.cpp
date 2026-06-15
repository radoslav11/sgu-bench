#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for (int tc = 0; tc < T; ++tc) {
        int H, W, N;
        cin >> H >> W >> N;

        int innerH = max(0, H - 2);
        int innerW = max(0, W - 2);
        int capacity = (innerH * innerW + 1) / 2;

        if (N > capacity) {
            cout << "Impossible\n";
        } else {
            vector<string> ans(H, string(W, '.'));

            int left = N;
            for (int i = 1; i + 1 < H; ++i) {
                for (int j = 1; j + 1 < W; ++j) {
                    if (((i + j) % 2 == 0) && left > 0) {
                        ans[i][j] = '#';
                        --left;
                    }
                }
            }

            for (int i = 0; i < H; ++i) {
                cout << ans[i] << '\n';
            }
        }

        if (tc + 1 < T) cout << '\n';
    }

    return 0;
}
