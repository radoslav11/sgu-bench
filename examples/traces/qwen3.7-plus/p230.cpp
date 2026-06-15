#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n + 1);
    vector<int> in_degree(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        in_degree[v]++;
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 1; i <= n; ++i) {
        if (in_degree[i] == 0) {
            pq.push(i);
        }
    }

    vector<int> ans(n + 1, 0);
    int current_weight = 1;

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();

        ans[u] = current_weight++;

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                pq.push(v);
            }
        }
    }

    if (current_weight <= n) {
        cout << "No solution\n";
    } else {
        for (int i = 1; i <= n; ++i) {
            cout << ans[i] << (i == n ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
