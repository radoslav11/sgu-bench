#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000;

int n, m;
vector<vector<int>> g;
vector<pair<int,int>> edges;

int timer_dfs = 0, compCnt = 0;
vector<int> tin, low, comp, st;
vector<bool> inStack;
vector<int> compSize;

void tarjan(int v) {
    tin[v] = low[v] = ++timer_dfs;
    st.push_back(v);
    inStack[v] = true;

    for (int to : g[v]) {
        if (!tin[to]) {
            tarjan(to);
            low[v] = min(low[v], low[to]);
        } else if (inStack[to]) {
            low[v] = min(low[v], tin[to]);
        }
    }

    if (low[v] == tin[v]) {
        int sz = 0;
        while (true) {
            int u = st.back();
            st.pop_back();
            inStack[u] = false;
            comp[u] = compCnt;
            sz++;
            if (u == v) break;
        }
        compSize.push_back(sz);
        compCnt++;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    g.assign(n, {});
    edges.resize(m);

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        --a, --b;
        edges[i] = {a, b};
        g[a].push_back(b);
    }

    tin.assign(n, 0);
    low.assign(n, 0);
    comp.assign(n, -1);
    inStack.assign(n, false);

    for (int i = 0; i < n; i++) {
        if (!tin[i]) tarjan(i);
    }

    int base = 0;
    for (int sz : compSize) base = max(base, sz);

    vector<bitset<MAXN>> reach(compCnt);

    for (int i = 0; i < compCnt; i++) {
        reach[i][i] = 1;
    }

    for (auto [u, v] : edges) {
        int cu = comp[u];
        int cv = comp[v];
        if (cu != cv) {
            reach[cu][cv] = 1;
        }
    }

    for (int k = 0; k < compCnt; k++) {
        for (int i = 0; i < compCnt; i++) {
            if (reach[i][k]) {
                reach[i] |= reach[k];
            }
        }
    }

    vector<bitset<MAXN>> canReachTo(compCnt);

    for (int i = 0; i < compCnt; i++) {
        for (int j = 0; j < compCnt; j++) {
            if (reach[i][j]) {
                canReachTo[j][i] = 1;
            }
        }
    }

    vector<vector<int>> value(compCnt, vector<int>(compCnt, -1));

    auto getValue = [&](int from, int to) {
        if (from == to) return base;

        int &res = value[from][to];
        if (res != -1) return res;

        bitset<MAXN> inside = reach[from] & canReachTo[to];

        int sum = 0;
        for (int i = 0; i < compCnt; i++) {
            if (inside[i]) {
                sum += compSize[i];
            }
        }

        res = max(base, sum);
        return res;
    };

    vector<int> edgeValue(m);
    int w = base;

    for (int i = 0; i < m; i++) {
        int u = edges[i].first;
        int v = edges[i].second;
        int cu = comp[u];
        int cv = comp[v];

        edgeValue[i] = getValue(cu, cv);
        w = max(w, edgeValue[i]);
    }

    vector<int> answer;
    for (int i = 0; i < m; i++) {
        if (edgeValue[i] == w) {
            answer.push_back(i + 1);
        }
    }

    cout << w << '\n';
    cout << answer.size() << '\n';
    for (int idx : answer) {
        cout << idx << ' ';
    }
    cout << '\n';

    return 0;
}
