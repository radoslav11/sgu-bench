#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < m; i++) {
        int a, b, d1, d2;
        cin >> a >> b >> d1 >> d2;
        a--; b--;
        int c = d1 * d2;
        g[a].push_back({b, c});
        g[b].push_back({a, c});
    }

    vector<int> color(n, -1); // -1: unvisited, else 0 or 1
    vector<int> assignment(n, -1);

    for (int i = 0; i < n; i++) {
        if (color[i] != -1) continue;

        vector<int> comp;
        queue<int> q;
        q.push(i);
        color[i] = 0;
        comp.push_back(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [v, c] : g[u]) {
                int delta = (c + 1) / 2;
                int expected = color[u] ^ delta;
                if (color[v] == -1) {
                    color[v] = expected;
                    q.push(v);
                    comp.push_back(v);
                } else if (color[v] != expected) {
                    cout << "NO" << endl;
                    return 0;
                }
            }
        }

        int ones = 0;
        for (int u : comp) {
            if (color[u] == 1) ones++;
        }
        int n_comp = comp.size();
        if (ones <= n_comp - ones) {
            for (int u : comp) {
                assignment[u] = color[u];
            }
        } else {
            for (int u : comp) {
                assignment[u] = 1 - color[u];
            }
        }
    }

    int total_flips = 0;
    for (int i = 0; i < n; i++) {
        total_flips += assignment[i];
    }

    cout << "YES" << '\n';
    cout << total_flips << '\n';
    for (int i = 0; i < n; i++) {
        if (assignment[i] == 1) {
            cout << i+1 << '\n';
        }
    }

    return 0;
}
