#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    for (int i = 0; i < n; ++i) cin >> x[i];

    int m, s;
    cin >> m >> s;

    vector<vector<int>> level(m, vector<int>(s));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < s; ++j) {
            cin >> level[i][j];
        }
    }

    int mask = m - 1;
    const long long INF = (1LL << 60);

    vector<long long> dpNext(m, 0), dpCur(m);
    vector<vector<unsigned char>> choice(n, vector<unsigned char>(m));

    for (int pos = n - 1; pos >= 0; --pos) {
        for (int set = 0; set < m; ++set) {
            long long best = INF;
            int bestK = 0;

            for (int k = 0; k < s; ++k) {
                int nextSet = k & mask;
                long long cost = llabs((long long)x[pos] - level[set][k]) + dpNext[nextSet];

                if (cost < best) {
                    best = cost;
                    bestK = k;
                }
            }

            dpCur[set] = best;
            choice[pos][set] = (unsigned char)bestK;
        }

        dpNext.swap(dpCur);
    }

    cout << dpNext[0] << '\n';

    int curSet = 0;
    for (int pos = 0; pos < n; ++pos) {
        int k = choice[pos][curSet];
        if (pos) cout << ' ';
        cout << k;
        curSet = k & mask;
    }
    cout << '\n';

    return 0;
}
