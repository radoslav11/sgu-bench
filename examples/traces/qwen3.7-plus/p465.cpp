#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const long long INF = 1e18;

struct Road {
    int u, v;
    long long L;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    long long R;
    if (!(cin >> n >> m >> R)) return 0;

    vector<long long> P(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> P[i];
    }

    vector<vector<long long>> D(n + 1, vector<long long>(n + 1, INF));
    for (int i = 1; i <= n; i++) {
        D[i][i] = 0;
    }

    vector<Road> roads;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long L;
        cin >> u >> v >> L;
        if (L < D[u][v]) {
            D[u][v] = L;
            D[v][u] = L;
        }
        roads.push_back({u, v, L});
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (D[i][k] < INF && D[k][j] < INF) {
                    D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
                }
            }
        }
    }

    if (n > 1) {
        for (int i = 2; i <= n; i++) {
            if (D[1][i] >= INF / 2) {
                cout << -1 << "\n";
                return 0;
            }
        }
    }

    if (n == 1) {
        if (R == 0) {
            cout << fixed << setprecision(5) << 0.0 << "\n";
        } else {
            cout << -1 << "\n";
        }
        return 0;
    }

    long long min_E = INF;
    bool found = false;

    for (const auto& road : roads) {
        if (road.L >= 2 * R) {
            vector<long long> xs = {R, road.L - R};
            for (long long x : xs) {
                long long current_E = 0;
                for (int c = 1; c <= n; c++) {
                    long long dist = min(D[road.u][c] + x, D[road.v][c] + road.L - x);
                    current_E += P[c] * dist;
                }
                if (current_E < min_E) {
                    min_E = current_E;
                    found = true;
                }
            }
        }
    }

    if (!found) {
        cout << -1 << "\n";
    } else {
        cout << fixed << setprecision(5) << (double)min_E / 10000.0 << "\n";
    }

    return 0;
}
