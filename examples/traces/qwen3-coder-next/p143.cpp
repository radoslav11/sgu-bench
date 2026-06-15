#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> profit(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> profit[i];
    }

    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    vector<int> dp(n + 1, 0);
    int maxProfit = -1000 * n; // worst case: all negative

    function<void(int, int)> dfs = [&](int u, int parent) {
        dp[u] = profit[u];
        for (int v : graph[u]) {
            if (v == parent) continue;
            dfs(v, u);
            if (dp[v] > 0) {
                dp[u] += dp[v];
            }
        }
        maxProfit = max(maxProfit, dp[u]);
    };

    dfs(1, 0);

    cout << maxProfit << endl;

    return 0;
}
