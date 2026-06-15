#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    vector<vector<bool>> dp(n + 1, vector<bool>(n + 2, false));

    for (int t = n; t >= 1; t--) {
        for (int u = 1; u <= n; u++) {
            if (dist[u] < t - 1) 
                continue;
            bool found = false;
            for (int v : adj[u]) {
                if (dist[v] >= t) {
                    if (!dp[v][t + 1]) {
                        found = true;
                        break;
                    }
                }
            }
            dp[u][t] = found;
        }
    }

    if (dp[1][1]) {
        cout << "Vladimir" << endl;
    } else {
        cout << "Nikolay" << endl;
    }

    return 0;
}
