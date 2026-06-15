#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int N = n * m;
    vector<int> b(N), deg(N, 0);

    auto id = [&](int r, int c) {
        return r * m + c;
    };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int x;
            cin >> x;
            b[id(i, j)] = x - 1;
        }
    }

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int v = id(i, j);
            for (int k = 0; k < 4; ++k) {
                int ni = i + dr[k], nj = j + dc[k];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                    deg[v]++;
                }
            }
        }
    }

    queue<int> q;
    vector<char> removed(N, 0);

    for (int v = 0; v < N; ++v) {
        if (deg[v] == b[v]) q.push(v);
    }

    vector<int> answer;

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (removed[v] || deg[v] != b[v]) continue;

        removed[v] = 1;
        answer.push_back(v);

        int r = v / m;
        int c = v % m;

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;

            int u = id(nr, nc);
            if (removed[u]) continue;

            deg[u]--;
            if (deg[u] == b[u]) q.push(u);
        }
    }

    if ((int)answer.size() != N) {
        cout << "No solution\n";
        return 0;
    }

    for (int v : answer) {
        cout << v / m + 1 << ' ' << v % m + 1 << '\n';
    }

    return 0;
}
