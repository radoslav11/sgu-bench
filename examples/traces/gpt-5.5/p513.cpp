#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int v, e;
    if (!(cin >> v >> e)) return 0;

    vector<vector<int>> g(v, vector<int>(v, 0));
    for (int i = 0; i < e; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        g[a][b] = g[b][a] = 1;
    }

    if (v % 3 != 0) {
        cout << "NO\n";
        return 0;
    }

    vector<vector<int>> h(v, vector<int>(v, 0));
    for (int i = 0; i < v; ++i) {
        for (int j = i + 1; j < v; ++j) {
            if (!g[i][j]) {
                h[i][j] = h[j][i] = 1;
            }
        }
    }

    vector<vector<int>> clauseEdge(v, vector<int>(v, 0));
    vector<vector<int>> contradictionEdge(v, vector<int>(v, 0));

    for (int i = 0; i < v; ++i) {
        for (int j = i + 1; j < v; ++j) {
            if (!h[i][j]) continue;

            int common = 0;
            for (int k = 0; k < v; ++k) {
                if (h[i][k] && h[j][k]) ++common;
            }

            if (common > 1) {
                cout << "NO\n";
                return 0;
            }

            if (common == 1) {
                clauseEdge[i][j] = clauseEdge[j][i] = 1;
            } else {
                contradictionEdge[i][j] = contradictionEdge[j][i] = 1;
            }
        }
    }

    vector<int> clauseId(v, -1);
    vector<vector<int>> clauses;
    vector<int> visited(v, 0);

    for (int s = 0; s < v; ++s) {
        if (visited[s]) continue;

        vector<int> comp;
        queue<int> q;
        q.push(s);
        visited[s] = 1;

        while (!q.empty()) {
            int x = q.front();
            q.pop();
            comp.push_back(x);

            for (int y = 0; y < v; ++y) {
                if (clauseEdge[x][y] && !visited[y]) {
                    visited[y] = 1;
                    q.push(y);
                }
            }
        }

        if ((int)comp.size() != 3) {
            cout << "NO\n";
            return 0;
        }

        int edges = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = i + 1; j < 3; ++j) {
                if (clauseEdge[comp[i]][comp[j]]) ++edges;
            }
        }

        if (edges != 3) {
            cout << "NO\n";
            return 0;
        }

        int id = (int)clauses.size();
        for (int x : comp) clauseId[x] = id;
        clauses.push_back(comp);
    }

    vector<int> varComp(v, -1);
    vector<int> color(v, -1);
    visited.assign(v, 0);
    int compId = 0;

    for (int s = 0; s < v; ++s) {
        if (visited[s]) continue;

        int deg = 0;
        for (int i = 0; i < v; ++i) deg += contradictionEdge[s][i];

        if (deg == 0) {
            visited[s] = 1;
            continue;
        }

        vector<int> comp;
        queue<int> q;
        q.push(s);
        visited[s] = 1;
        color[s] = 0;

        bool ok = true;

        while (!q.empty()) {
            int x = q.front();
            q.pop();
            comp.push_back(x);

            for (int y = 0; y < v; ++y) {
                if (!contradictionEdge[x][y]) continue;

                if (!visited[y]) {
                    visited[y] = 1;
                    color[y] = color[x] ^ 1;
                    q.push(y);
                } else if (color[y] == color[x]) {
                    ok = false;
                }
            }
        }

        if (!ok) {
            cout << "NO\n";
            return 0;
        }

        long long cnt[2] = {0, 0};
        long long edgeCount = 0;

        for (int x : comp) {
            cnt[color[x]]++;
            for (int y : comp) {
                if (x < y && contradictionEdge[x][y]) ++edgeCount;
            }
        }

        if (edgeCount != cnt[0] * cnt[1]) {
            cout << "NO\n";
            return 0;
        }

        for (int x : comp) varComp[x] = compId;
        ++compId;
    }

    for (const auto &clause : clauses) {
        set<int> seen;
        for (int x : clause) {
            if (varComp[x] == -1) continue;
            if (seen.count(varComp[x])) {
                cout << "NO\n";
                return 0;
            }
            seen.insert(varComp[x]);
        }
    }

    cout << "YES\n";
    return 0;
}
