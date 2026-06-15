#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m1, m2;
    if (!(cin >> n >> m1 >> m2)) return 0;

    vector<pair<int,int>> juan_in(m1), rosa_in(m2);
    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m1; ++i) {
        int a, b;
        cin >> a >> b;
        juan_in[i] = {a, b};
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    for (int i = 0; i < m2; ++i) {
        int a, b;
        cin >> a >> b;
        rosa_in[i] = {a, b};
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> part(n + 1, -1);
    for (int s = 1; s <= n; ++s) {
        if (part[s] != -1) continue;
        part[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int to : adj[v]) {
                if (part[to] == -1) {
                    part[to] = part[v] ^ 1;
                    q.push(to);
                }
            }
        }
    }

    const int INF = 1000000;

    vector<vector<int>> cost(n + 1, vector<int>(n + 1, INF));
    vector<vector<int>> type(n + 1, vector<int>(n + 1, 0));
    // type: 1 = Juan edge, 2 = Rosa edge

    for (int i = 1; i <= n; ++i) cost[i][i] = 0;

    for (auto [a, b] : juan_in) {
        if (part[a] == 1) swap(a, b);
        // Juan edges are directed from part 0 to part 1
        cost[a][b] = -1;
        type[a][b] = 1;
    }

    for (auto [a, b] : rosa_in) {
        if (part[a] == 0) swap(a, b);
        // Rosa edges are directed from part 1 to part 0
        cost[a][b] = -1;
        type[a][b] = 2;
    }

    // Hungarian algorithm for minimum weight perfect matching/assignment.
    vector<int> u(n + 1), v(n + 1), p(n + 1), way(n + 1);

    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv(n + 1, INF);
        vector<char> used(n + 1, false);

        do {
            used[j0] = true;
            int i0 = p[j0];
            int delta = INF;
            int j1 = 0;

            for (int j = 1; j <= n; ++j) {
                if (used[j]) continue;
                int cur = cost[i0][j] - u[i0] - v[j];
                if (cur < minv[j]) {
                    minv[j] = cur;
                    way[j] = j0;
                }
                if (minv[j] < delta) {
                    delta = minv[j];
                    j1 = j;
                }
            }

            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }

            j0 = j1;
        } while (p[j0] != 0);

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }

    vector<int> match(n + 1);
    for (int j = 1; j <= n; ++j) {
        match[p[j]] = j;
    }

    vector<pair<int,int>> juan, rosa;

    for (int i = 1; i <= n; ++i) {
        int j = match[i];
        if (i == j) continue;

        if (type[i][j] == 1) {
            juan.push_back({i, j});
        } else if (type[i][j] == 2) {
            rosa.push_back({i, j});
        }
    }

    cout << juan.size() << '\n';

    for (auto [a, b] : juan) {
        cout << a << ' ' << b << '\n';
    }

    for (auto [a, b] : rosa) {
        cout << a << ' ' << b << '\n';
    }

    return 0;
}
