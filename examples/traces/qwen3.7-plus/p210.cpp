#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> A(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> A[i];
    }

    vector<vector<long long>> W(n + 1, vector<long long>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        int k;
        cin >> k;
        for (int j = 0; j < k; ++j) {
            int g;
            cin >> g;
            W[i][g] = A[i] * A[i];
        }
    }

    vector<long long> u(n + 1, 0), v(n + 1, 0);
    vector<int> p(n + 1, 0), way(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        long long min_cost = INF;
        for (int j = 1; j <= n; ++j) {
            min_cost = min(min_cost, -W[i][j]);
        }
        u[i] = min_cost;
    }

    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<long long> minv(n + 1, INF);
        vector<bool> used(n + 1, false);

        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            long long delta = INF;

            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    long long cur = -W[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }

            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }

    vector<int> ans(n + 1, 0);
    for (int j = 1; j <= n; ++j) {
        if (p[j] != 0) {
            ans[p[j]] = j;
        }
    }

    for (int i = 1; i <= n; ++i) {
        if (W[i][ans[i]] > 0) {
            cout << ans[i] << (i == n ? "" : " ");
        } else {
            cout << 0 << (i == n ? "" : " ");
        }
    }
    cout << "\n";

    return 0;
}
