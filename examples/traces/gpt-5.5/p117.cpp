#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    cin >> N >> M >> K;

    vector<pair<int, int>> factors;
    int t = K;

    for (int p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            factors.push_back({p, cnt});
        }
    }
    if (t > 1) {
        factors.push_back({t, 1});
    }

    int answer = 0;

    for (int i = 0; i < N; ++i) {
        int x;
        cin >> x;

        bool ok = true;

        for (auto [p, e] : factors) {
            int need = (e + M - 1) / M;
            int cnt = 0;
            int y = x;

            while (y % p == 0) {
                y /= p;
                ++cnt;
            }

            if (cnt < need) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ++answer;
        }
    }

    cout << answer << '\n';

    return 0;
}
