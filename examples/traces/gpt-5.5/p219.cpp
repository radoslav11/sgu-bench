#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<Edge> edges;
    edges.reserve(M);
    vector<vector<int>> g(N);

    for (int i = 0; i < M; ++i) {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        --a; --b;
        edges.push_back({a, b, c});
        g[a].push_back(b);
    }

    vector<int> tin(N, -1), low(N), st;
    vector<bool> inStack(N, false);
    vector<int> comp(N, -1);
    int timer = 0, compCnt = 0;

    function<void(int)> dfs = [&](int v) {
        tin[v] = low[v] = timer++;
        st.push_back(v);
        inStack[v] = true;

        for (int to : g[v]) {
            if (tin[to] == -1) {
                dfs(to);
                low[v] = min(low[v], low[to]);
            } else if (inStack[to]) {
                low[v] = min(low[v], tin[to]);
            }
        }

        if (low[v] == tin[v]) {
            while (true) {
                int x = st.back();
                st.pop_back();
                inStack[x] = false;
                comp[x] = compCnt;
                if (x == v) break;
            }
            ++compCnt;
        }
    };

    for (int i = 0; i < N; ++i) {
        if (tin[i] == -1) dfs(i);
    }

    vector<vector<int>> zeroG(N);
    vector<vector<int>> compDag(compCnt);
    vector<vector<int>> incomingComps(compCnt);
    vector<int> indeg(compCnt, 0);
    vector<bool> zeroCycle(compCnt, false);

    set<pair<int, int>> dagEdges;

    for (const auto &e : edges) {
        int cu = comp[e.u], cv = comp[e.v];

        if (cu == cv) {
            if (e.w == 0) {
                zeroG[e.u].push_back(e.v);
            }
        } else {
            if (dagEdges.insert({cu, cv}).second) {
                compDag[cu].push_back(cv);
                incomingComps[cv].push_back(cu);
                indeg[cv]++;
            }
        }
    }

    vector<int> color(N, 0);

    function<void(int)> dfsZero = [&](int v) {
        color[v] = 1;
        for (int to : zeroG[v]) {
            if (comp[to] != comp[v]) continue;

            if (color[to] == 0) {
                dfsZero(to);
            } else if (color[to] == 1) {
                zeroCycle[comp[v]] = true;
            }
        }
        color[v] = 2;
    };

    for (int i = 0; i < N; ++i) {
        if (color[i] == 0) {
            dfsZero(i);
        }
    }

    queue<int> q;
    for (int c = 0; c < compCnt; ++c) {
        if (indeg[c] == 0) q.push(c);
    }

    vector<int> topo;
    while (!q.empty()) {
        int c = q.front();
        q.pop();
        topo.push_back(c);

        for (int to : compDag[c]) {
            if (--indeg[to] == 0) {
                q.push(to);
            }
        }
    }

    vector<bool> aliveComp(compCnt, false);

    for (int c : topo) {
        bool ok = !zeroCycle[c];

        for (int from : incomingComps[c]) {
            if (!aliveComp[from]) {
                ok = false;
                break;
            }
        }

        aliveComp[c] = ok;
    }

    for (int i = 0; i < N; ++i) {
        cout << (aliveComp[comp[i]] ? 1 : 0) << '\n';
    }

    return 0;
}
