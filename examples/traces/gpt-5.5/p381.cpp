#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int,int>>> g(n + 1);

    for (int i = 0; i < m; ++i) {
        int a, b, d1, d2;
        cin >> a >> b >> d1 >> d2;

        int c = -d1 * d2;
        g[a].push_back({b, c});
        g[b].push_back({a, c});
    }

    vector<int> val(n + 1, 0);
    vector<int> answer;

    for (int start = 1; start <= n; ++start) {
        if (val[start] != 0) continue;

        vector<int> comp;
        queue<int> q;

        val[start] = 1;
        q.push(start);

        bool ok = true;

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            comp.push_back(v);

            for (auto [to, c] : g[v]) {
                int need = val[v] * c;

                if (val[to] == 0) {
                    val[to] = need;
                    q.push(to);
                } else if (val[to] != need) {
                    ok = false;
                }
            }
        }

        if (!ok) {
            cout << "NO\n";
            return 0;
        }

        vector<int> minus_vertices, plus_vertices;

        for (int v : comp) {
            if (val[v] == -1) minus_vertices.push_back(v);
            else plus_vertices.push_back(v);
        }

        if (minus_vertices.size() <= plus_vertices.size()) {
            for (int v : minus_vertices) answer.push_back(v);
        } else {
            for (int v : plus_vertices) answer.push_back(v);
        }
    }

    cout << "YES\n";
    cout << answer.size() << '\n';
    for (int v : answer) {
        cout << v << '\n';
    }

    return 0;
}
