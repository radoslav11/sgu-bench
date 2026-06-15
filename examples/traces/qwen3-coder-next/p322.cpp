#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    set<pair<int, int>> E1, E2;

    for (int i = 0; i < N - 1; i++) {
        int a, b;
        cin >> a >> b;
        if (a > b) swap(a, b);
        E1.insert({a, b});
    }

    for (int i = 0; i < N - 1; i++) {
        int a, b;
        cin >> a >> b;
        if (a > b) swap(a, b);
        E2.insert({a, b});
    }

    set<pair<int, int>> D1, D2;
    for (auto &e : E1) {
        if (E2.find(e) == E2.end()) {
            D1.insert(e);
        }
    }
    for (auto &e : E2) {
        if (E1.find(e) == E1.end()) {
            D2.insert(e);
        }
    }

    int k = D1.size();
    cout << k << '\n';

    // Build initial graph for Berland (T1)
    vector<vector<int>> T1(N + 1);
    for (auto &e : E1) {
        int a = e.first, b = e.second;
        T1[a].push_back(b);
        T1[b].push_back(a);
    }

    set<pair<int, int>> curD1 = D1;
    set<pair<int, int>> curD2 = D2;

    for (int i = 0; i < k; i++) {
        auto e0 = *curD1.begin();
        int u = e0.first, v = e0.second;

        // BFS to get the component of u without edge (u, v)
        vector<bool> vis(N + 1, false);
        queue<int> q;
        q.push(u);
        vis[u] = true;
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            for (int neighbor : T1[cur]) {
                // Skip the edge (u, v) in both directions
                if ((cur == u && neighbor == v) || (cur == v && neighbor == u)) {
                    continue;
                }
                if (!vis[neighbor]) {
                    vis[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        // Find an edge f in curD2 that crosses the cut
        pair<int, int> f_edge = {-1, -1};
        for (auto &e : curD2) {
            int x = e.first, y = e.second;
            if ((vis[x] && !vis[y]) || (!vis[x] && vis[y])) {
                f_edge = e;
                break;
            }
        }

        // Remove e0 from curD1 and f_edge from curD2
        curD1.erase(e0);
        curD2.erase(f_edge);

        // Update T1: remove e0
        auto it = find(T1[u].begin(), T1[u].end(), v);
        T1[u].erase(it);
        it = find(T1[v].begin(), T1[v].end(), u);
        T1[v].erase(it);

        // Add f_edge
        int x = f_edge.first, y = f_edge.second;
        T1[x].push_back(y);
        T1[y].push_back(x);

        // Output the operation: in Berland (p=1), add f_edge, remove e0
        cout << 1 << " " << x << " " << y << " " << u << " " << v << '\n';
    }

    return 0;
}
