#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cctype>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }

    // Prim's algorithm for MST
    vector<int> parent(n, -1);
    vector<double> parentDist(n, 0.0);
    vector<bool> inMST(n, false);
    vector<double> minDist(n, 1e18); // 1e18 as infinity

    minDist[0] = 0.0;

    for (int i = 0; i < n; i++) {
        // Find the node with minimum minDist not in MST
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!inMST[j] && (u == -1 || minDist[j] < minDist[u])) {
                u = j;
            }
        }

        if (u == -1) break;
        inMST[u] = true;

        // Update all adjacent nodes (all other nodes)
        for (int v = 0; v < n; v++) {
            if (u == v || inMST[v]) continue;
            double dx = x[u] - x[v];
            double dy = y[u] - y[v];
            double d = sqrt(dx * dx + dy * dy);
            if (d < minDist[v]) {
                minDist[v] = d;
                parent[v] = u;
                parentDist[v] = d;
            }
        }
    }

    // Build adjacency list for the MST
    vector<vector<pair<int, double>>> adj(n);
    for (int v = 1; v < n; v++) {
        int u = parent[v];
        double w = parentDist[v];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // Precompute the answer for all pairs
    vector<vector<double>> ansMatrix(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        vector<double> maxEdge(n, -1.0);
        queue<int> q;
        maxEdge[i] = 0.0;
        q.push(i);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                int v = edge.first;
                double w = edge.second;
                if (maxEdge[v] < 0) {
                    maxEdge[v] = max(maxEdge[u], w);
                    q.push(v);
                }
            }
        }
        for (int j = 0; j < n; j++) {
            ansMatrix[i][j] = maxEdge[j];
        }
    }

    int q_count;
    cin >> q_count;
    cout << fixed << setprecision(12);
    while (q_count--) {
        int s, t;
        cin >> s >> t;
        s--; t--;
        cout << ansMatrix[s][t] << '\n';
    }

    return 0;
}
