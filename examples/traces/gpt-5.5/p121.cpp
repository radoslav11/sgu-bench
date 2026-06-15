#include <bits/stdc++.h>
using namespace std;

struct OrigEdge {
    int u, v;
};

struct Edge {
    int u, v, orig;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<int>> order(N);
    vector<OrigEdge> orig;
    int id[105][105];
    memset(id, -1, sizeof(id));

    for (int i = 0; i < N; i++) {
        int x;
        while (cin >> x) {
            if (x == 0) break;
            --x;
            if (id[i][x] == -1) {
                int eid = (int)orig.size();
                id[i][x] = id[x][i] = eid;
                orig.push_back({i, x});
            }
            order[i].push_back(id[i][x]);
        }
    }

    int M = (int)orig.size();
    vector<int> deg(N, 0);
    vector<vector<int>> g(N);
    for (int i = 0; i < M; i++) {
        deg[orig[i].u]++;
        deg[orig[i].v]++;
        g[orig[i].u].push_back(orig[i].v);
        g[orig[i].v].push_back(orig[i].u);
    }

    vector<int> color(M, 0);
    vector<int> usedV(N, 0);

    for (int s = 0; s < N; s++) {
        if (usedV[s]) continue;

        vector<int> comp;
        queue<int> q;
        q.push(s);
        usedV[s] = 1;

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            comp.push_back(v);
            for (int to : g[v]) {
                if (!usedV[to]) {
                    usedV[to] = 1;
                    q.push(to);
                }
            }
        }

        int edgesCnt = 0;
        bool allDeg2 = true;
        for (int v : comp) {
            edgesCnt += deg[v];
            if (deg[v] != 2) allDeg2 = false;
        }
        edgesCnt /= 2;

        if (edgesCnt == 0) continue;

        if (allDeg2 && edgesCnt % 2 == 1) {
            cout << "No solution\n";
            return 0;
        }

        vector<Edge> edges;
        vector<vector<pair<int,int>>> adj(N);

        for (int i = 0; i < M; i++) {
            int u = orig[i].u, v = orig[i].v;
            if (usedV[u] && usedV[v]) {
                bool inComp = false;
                for (int x : comp) {
                    if (x == u) {
                        inComp = true;
                        break;
                    }
                }
                if (!inComp) continue;

                int eid = (int)edges.size();
                edges.push_back({u, v, i});
                adj[u].push_back({v, eid});
                adj[v].push_back({u, eid});
            }
        }

        vector<int> odd;
        for (int v : comp) {
            if (deg[v] % 2 == 1) odd.push_back(v);
        }

        for (int i = 0; i + 1 < (int)odd.size(); i += 2) {
            int u = odd[i], v = odd[i + 1];
            int eid = (int)edges.size();
            edges.push_back({u, v, -1});
            adj[u].push_back({v, eid});
            adj[v].push_back({u, eid});
        }

        int start = comp[0];
        if ((int)edges.size() % 2 == 1) {
            for (int v : comp) {
                if (deg[v] == 1 || deg[v] >= 3) {
                    start = v;
                    break;
                }
            }
        }

        vector<int> ptr(N, 0), usedE(edges.size(), 0), tour;

        function<void(int)> dfs = [&](int v) {
            while (ptr[v] < (int)adj[v].size()) {
                auto [to, eid] = adj[v][ptr[v]++];
                if (usedE[eid]) continue;
                usedE[eid] = 1;
                dfs(to);
                tour.push_back(eid);
            }
        };

        dfs(start);
        reverse(tour.begin(), tour.end());

        for (int i = 0; i < (int)tour.size(); i++) {
            int eid = tour[i];
            if (edges[eid].orig != -1) {
                color[edges[eid].orig] = (i % 2) + 1;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int eid : order[i]) {
            cout << color[eid] << ' ';
        }
        cout << "0\n";
    }

    return 0;
}
