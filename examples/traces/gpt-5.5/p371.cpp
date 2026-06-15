#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, M;
    if (!(cin >> N >> M)) return 0;

    long long Kll = M - N + 1;
    if (Kll < 1) {
        cout << "No solution\n";
        return 0;
    }

    long long low = 3 * Kll;
    long long need = N + 3 * Kll - 3;
    low = max(low, (need + 1) / 2);

    long long high = min(10 * Kll, N + Kll - 1);

    if (low > high) {
        cout << "No solution\n";
        return 0;
    }

    long long S = low;
    long long Rll = N - S + Kll - 1;

    if (Rll < 0) {
        cout << "No solution\n";
        return 0;
    }

    int K = (int)Kll;
    int R = (int)Rll;

    vector<int> len(K, 3);
    long long extra = S - 3 * Kll;
    for (int i = 0; i < K && extra > 0; i++) {
        int add = (int)min(7LL, extra);
        len[i] += add;
        extra -= add;
    }

    vector<vector<int>> circles(K);
    vector<int> isTransfer((int)N + 1, 0);

    int nextStation = 1;
    int nextShare = -1;

    for (int i = 0; i < K; i++) {
        if (i == 0) {
            for (int j = 0; j < len[i]; j++) {
                circles[i].push_back(nextStation++);
            }
            if (K > 1) {
                nextShare = circles[i][0];
                isTransfer[nextShare] = 1;
            }
        } else {
            int shared = nextShare;
            circles[i].push_back(shared);

            for (int j = 1; j < len[i]; j++) {
                circles[i].push_back(nextStation++);
            }

            if (i + 1 < K) {
                nextShare = circles[i][1];
                isTransfer[nextShare] = 1;
            }
        }
    }

    int circleStations = nextStation - 1;
    if (circleStations + R != N) {
        cout << "No solution\n";
        return 0;
    }

    vector<int> attach;
    for (int v = 1; v <= circleStations; v++) {
        if (!isTransfer[v]) attach.push_back(v);
    }

    if ((int)attach.size() < R) {
        cout << "No solution\n";
        return 0;
    }

    cout << K << '\n';
    for (int i = 0; i < K; i++) {
        cout << len[i];
        for (int v : circles[i]) cout << ' ' << v;
        cout << '\n';
    }

    cout << R << '\n';
    int terminal = circleStations + 1;
    for (int i = 0; i < R; i++) {
        cout << attach[i] << ' ' << terminal++ << '\n';
    }

    return 0;
}
