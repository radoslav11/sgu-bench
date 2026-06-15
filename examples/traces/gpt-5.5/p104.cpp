#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int F, V;
    cin >> F >> V;

    vector<vector<int>> A(F + 1, vector<int>(V + 1));
    for (int i = 1; i <= F; i++) {
        for (int j = 1; j <= V; j++) {
            cin >> A[i][j];
        }
    }

    const int NEG_INF = -1e9;

    vector<vector<int>> dp(F + 1, vector<int>(V + 1, NEG_INF));
    vector<vector<int>> parent(F + 1, vector<int>(V + 1, -1));

    for (int j = 1; j <= V; j++) {
        dp[1][j] = A[1][j];
    }

    for (int i = 2; i <= F; i++) {
        for (int j = i; j <= V; j++) {
            for (int k = i - 1; k < j; k++) {
                if (dp[i - 1][k] + A[i][j] > dp[i][j]) {
                    dp[i][j] = dp[i - 1][k] + A[i][j];
                    parent[i][j] = k;
                }
            }
        }
    }

    int bestValue = NEG_INF;
    int lastVase = -1;

    for (int j = F; j <= V; j++) {
        if (dp[F][j] > bestValue) {
            bestValue = dp[F][j];
            lastVase = j;
        }
    }

    vector<int> arrangement(F + 1);
    int vase = lastVase;

    for (int i = F; i >= 1; i--) {
        arrangement[i] = vase;
        vase = parent[i][vase];
    }

    cout << bestValue << '\n';
    for (int i = 1; i <= F; i++) {
        if (i > 1) cout << ' ';
        cout << arrangement[i];
    }
    cout << '\n';

    return 0;
}
