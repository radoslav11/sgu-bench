#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<long long>> a(n, vector<long long>(m));
    vector<long long> vals;
    vals.reserve(1LL * n * m);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> a[i][j];
            vals.push_back(a[i][j]);
        }
    }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    int V = (int)vals.size();

    auto id = [&](long long x) {
        return (int)(lower_bound(vals.begin(), vals.end(), x) - vals.begin());
    };

    vector<vector<int>> b(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            b[i][j] = id(a[i][j]);

    vector<unordered_set<int>> g(V);
    vector<int> color(V, 0);

    auto recolor = [&]() -> bool {
        vector<int> ord(V);
        iota(ord.begin(), ord.end(), 0);

        sort(ord.begin(), ord.end(), [&](int x, int y) {
            if (g[x].size() != g[y].size()) return g[x].size() > g[y].size();
            return x < y;
        });

        fill(color.begin(), color.end(), -1);

        for (int v : ord) {
            bool used[51] = {};
            for (int to : g[v]) {
                if (color[to] != -1) used[color[to]] = true;
            }

            int c = -1;
            for (int i = 0; i <= 50; ++i) {
                if (!used[i]) {
                    c = i;
                    break;
                }
            }

            if (c == -1) return false;
            color[v] = c;
        }

        return true;
    };

    if (!recolor()) {
        cout << "No\n";
        return 0;
    }

    bool changed = true;
    int iterations = 0;

    while (changed && iterations <= n * m + 5) {
        changed = false;
        ++iterations;

        for (int i = 1; i < n; ++i) {
            bool ok = false;
            int x = -1, y = -1;

            for (int j = 0; j < m; ++j) {
                if (color[b[0][j]] != color[b[i][j]]) {
                    ok = true;
                    break;
                }
                if (b[0][j] != b[i][j] && x == -1) {
                    x = b[0][j];
                    y = b[i][j];
                }
            }

            if (!ok) {
                if (x == -1) {
                    cout << "No\n";
                    return 0;
                }

                if (!g[x].count(y)) {
                    g[x].insert(y);
                    g[y].insert(x);
                    changed = true;

                    if (!recolor()) {
                        cout << "No\n";
                        return 0;
                    }
                }
            }
        }
    }

    for (int i = 1; i < n; ++i) {
        bool ok = false;
        for (int j = 0; j < m; ++j) {
            if (color[b[0][j]] != color[b[i][j]]) {
                ok = true;
                break;
            }
        }

        if (!ok) {
            cout << "No\n";
            return 0;
        }
    }

    cout << "Yes\n";
    for (int i = 0; i < V; ++i) {
        cout << vals[i] << " -> " << color[i] << '\n';
    }

    return 0;
}
