#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, K, P;
    cin >> N >> K >> P;

    vector<int> B(K);
    for (int i = 0; i < K; i++) {
        cin >> B[i];
    }

    vector<vector<pair<int, int>>> gums(K);
    for (int i = 0; i < N; i++) {
        int A, C;
        cin >> A >> C;
        C--;
        gums[C].push_back({A, i + 1});
    }

    // Precompute prefix sums for each gum
    vector<vector<int>> prefix(K);
    for (int j = 0; j < K; j++) {
        sort(gums[j].begin(), gums[j].end());
        prefix[j].resize(gums[j].size() + 1);
        prefix[j][0] = 0;
        for (int k = 1; k <= (int)gums[j].size(); k++) {
            prefix[j][k] = prefix[j][k - 1] + gums[j][k - 1].first;
        }
    }

    const int INF = 1e9;
    // dp[j][t] = minimal cost to have t teeth from the first j gums
    vector<vector<int>> dp(K + 1, vector<int>(N + 1, INF));
    // parent[j][t] = number of teeth taken from gum j-1 to achieve dp[j][t]
    vector<vector<int>> parent(K + 1, vector<int>(N + 1, 0));

    dp[0][0] = 0;

    for (int j = 0; j < K; j++) {
        int num_teeth_j = gums[j].size();
        for (int t = 0; t <= N; t++) {
            // Option: skip gum j
            dp[j + 1][t] = dp[j][t];
            parent[j + 1][t] = 0;

            // Try taking k teeth from gum j (k >= 1)
            for (int k = 1; k <= num_teeth_j && k <= t; k++) {
                int cost_j = B[j] + prefix[j][k];
                if (dp[j][t - k] + cost_j < dp[j + 1][t]) {
                    dp[j + 1][t] = dp[j][t - k] + cost_j;
                    parent[j + 1][t] = k;
                }
            }
        }
    }

    // Find maximum number of teeth
    int max_teeth = 0;
    for (int t = N; t >= 0; t--) {
        if (dp[K][t] <= P) {
            max_teeth = t;
            break;
        }
    }

    // Backtrack to collect the tooth indices
    vector<int> chosen;
    int t_remaining = max_teeth;
    for (int j = K - 1; j >= 0; j--) {
        int k = parent[j + 1][t_remaining];
        for (int i = 0; i < k; i++) {
            chosen.push_back(gums[j][i].second);
        }
        t_remaining -= k;
    }

    cout << max_teeth << "\n";
    if (max_teeth > 0) {
        for (int i = 0; i < chosen.size(); i++) {
            if (i > 0) cout << " ";
            cout << chosen[i];
        }
        cout << "\n";
    } else {
        cout << "\n";
    }

    return 0;
}
