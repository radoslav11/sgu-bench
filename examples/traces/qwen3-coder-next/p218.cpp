#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <climits>
#include <algorithm>
using namespace std;

bool hopcroft_karp(int x, const vector<vector<int>>& a, int n, vector<int>& matching) {
    vector<vector<int>> adj(n);
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (a[u][v] <= x) {
                adj[u].push_back(v);
            }
        }
    }

    vector<int> matchL(n, -1);
    vector<int> matchR(n, -1);
    vector<int> dist(n);

    auto bfs = [&]() -> bool {
        queue<int> q;
        for (int u = 0; u < n; u++) {
            if (matchL[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int next_u = matchR[v];
                if (next_u != -1 && dist[next_u] == -1) {
                    dist[next_u] = dist[u] + 1;
                    q.push(next_u);
                } else if (next_u == -1) {
                    found = true;
                }
            }
        }
        return found;
    };

    function<bool(int)> dfs = [&](int u) {
        for (int v : adj[u]) {
            int next_u = matchR[v];
            if (next_u == -1 || (dist[next_u] != -1 && dist[next_u] == dist[u] + 1 && dfs(next_u))) {
                matchR[v] = u;
                matchL[u] = v;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    };

    int matching_size = 0;
    while (bfs()) {
        for (int u = 0; u < n; u++) {
            if (matchL[u] == -1 && dfs(u)) {
                matching_size++;
            }
        }
    }

    if (matching_size != n) {
        return false;
    }
    matching = move(matchL);
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    int low = INT_MAX;
    int high = INT_MIN;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            low = min(low, matrix[i][j]);
            high = max(high, matrix[i][j]);
        }
    }

    int ans_val = high;
    vector<int> best_matching;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        vector<int> matching;
        if (hopcroft_karp(mid, matrix, n, matching)) {
            ans_val = mid;
            best_matching = matching;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << ans_val << '\n';
    for (int i = 0; i < n; i++) {
        cout << i + 1 << " " << best_matching[i] + 1 << '\n';
    }

    return 0;
}
