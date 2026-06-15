#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<pair<int,int>> edges(m + 1);
    vector<int> deg(n + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int x, y;
        cin >> x >> y;
        edges[i] = {x, y};
        deg[x]++;
        deg[y]++;
    }

    int center = 1;
    for (int i = 1; i <= n; ++i) {
        if (deg[i] == n - 1) {
            center = i;
            break;
        }
    }

    vector<int> mate(n + 1, 0);
    vector<bool> spokeAlive(n + 1, false);
    vector<bool> baseAlive(n + 1, false);
    vector<bool> edgeAlive(m + 1, true);
    vector<int> edgeType(m + 1, 0); // 1 = spoke, 2 = base
    vector<int> edgeVertex(m + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int a = edges[i].first;
        int b = edges[i].second;

        if (a == center || b == center) {
            int v = a ^ b ^ center;
            edgeType[i] = 1;
            edgeVertex[i] = v;
            spokeAlive[v] = true;
        } else {
            edgeType[i] = 2;
            edgeVertex[i] = a;
            mate[a] = b;
            mate[b] = a;
            baseAlive[a] = baseAlive[b] = true;
        }
    }

    const int INF = 1e9;

    auto distToCenter = [&](int v) -> int {
        if (v == center) return 0;
        if (spokeAlive[v]) return 1;
        int u = mate[v];
        if (u != 0 && baseAlive[v] && spokeAlive[u]) return 2;
        return INF;
    };

    string op;
    while (cin >> op) {
        if (op == "DELETE") {
            int id;
            cin >> id;

            if (!edgeAlive[id]) continue;
            edgeAlive[id] = false;

            if (edgeType[id] == 1) {
                int v = edgeVertex[id];
                spokeAlive[v] = false;
            } else if (edgeType[id] == 2) {
                int v = edgeVertex[id];
                int u = mate[v];
                baseAlive[v] = false;
                if (u != 0) baseAlive[u] = false;
            }
        } else if (op == "LENGTH") {
            int x, y;
            cin >> x >> y;

            if (x == y) {
                cout << 0 << '\n';
                continue;
            }

            int ans = INF;

            if (x == center || y == center) {
                int v = (x == center ? y : x);
                ans = distToCenter(v);
            } else if (mate[x] == y) {
                if (baseAlive[x]) ans = min(ans, 1);

                int dx = distToCenter(x);
                int dy = distToCenter(y);
                if (dx < INF && dy < INF) ans = min(ans, dx + dy);
            } else {
                int dx = distToCenter(x);
                int dy = distToCenter(y);
                if (dx < INF && dy < INF) ans = dx + dy;
            }

            if (ans >= INF) cout << -1 << '\n';
            else cout << ans << '\n';
        }
    }

    return 0;
}
