#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int N = n * n;
    vector<int> top(N);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> top[i * n + j];
        }
    }

    vector<vector<int>> g(N);
    vector<int> indeg(N, 0);
    bool ok = true;

    auto add_edge = [&](int u, int v) {
        g[u].push_back(v);
        ++indeg[v];
    };

    vector<int> order;
    order.reserve(n);

    // Process left matrix: each row defines a total order inside that row.
    for (int i = 0; i < n; ++i) {
        order.clear();

        for (int j = 0; j < n; ++j) {
            int x;
            cin >> x;

            if (x < 0 || x > j) ok = false;

            int rank_pos = j - x; // 0-based position in increasing order
            if (rank_pos < 0 || rank_pos > (int)order.size()) ok = false;

            int id = i * n + j;
            if (ok) {
                order.insert(order.begin() + rank_pos, id);
            }
        }

        if (ok) {
            for (int k = 0; k + 1 < n; ++k) {
                add_edge(order[k], order[k + 1]);
            }
        }
    }

    if (!ok) {
        cout << 0 << '\n';
        return 0;
    }

    // Process top matrix: each column defines a total order inside that column.
    for (int j = 0; j < n; ++j) {
        order.clear();

        for (int i = 0; i < n; ++i) {
            int x = top[i * n + j];

            if (x < 0 || x > i) {
                ok = false;
                break;
            }

            int rank_pos = i - x; // 0-based position in increasing order
            if (rank_pos < 0 || rank_pos > (int)order.size()) {
                ok = false;
                break;
            }

            int id = i * n + j;
            order.insert(order.begin() + rank_pos, id);
        }

        if (!ok) break;

        for (int k = 0; k + 1 < n; ++k) {
            add_edge(order[k], order[k + 1]);
        }
    }

    if (!ok) {
        cout << 0 << '\n';
        return 0;
    }

    queue<int> q;
    for (int v = 0; v < N; ++v) {
        if (indeg[v] == 0) q.push(v);
    }

    vector<int> ans(N, 0);
    int cur = 0;

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        ans[v] = ++cur;

        for (int to : g[v]) {
            --indeg[to];
            if (indeg[to] == 0) q.push(to);
        }
    }

    if (cur != N) {
        cout << 0 << '\n';
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << ans[i * n + j];
        }
        cout << '\n';
    }

    return 0;
}
