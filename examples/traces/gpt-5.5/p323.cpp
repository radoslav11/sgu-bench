#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int a, b, c, p, idx;
};

struct DSU {
    vector<int> parent, sz;
    int comps;

    DSU(int n = 0) {
        parent.resize(n + 1);
        sz.resize(n + 1);
    }

    void init(int n) {
        comps = n;
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
            sz[i] = 1;
        }
    }

    int find(int v) {
        while (v != parent[v]) {
            parent[v] = parent[parent[v]];
            v = parent[v];
        }
        return v;
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        --comps;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    cin >> N >> M >> K;

    vector<Edge> edges;
    edges.reserve(K);

    vector<vector<int>> byColor(M + 1);

    for (int i = 1; i <= K; ++i) {
        Edge e;
        cin >> e.a >> e.b >> e.c >> e.p;
        e.idx = i;
        edges.push_back(e);
        byColor[e.c].push_back((int)edges.size() - 1);
    }

    vector<int> order(K);
    iota(order.begin(), order.end(), 0);

    sort(order.begin(), order.end(), [&](int x, int y) {
        if (edges[x].p != edges[y].p) return edges[x].p < edges[y].p;
        return edges[x].idx < edges[y].idx;
    });

    const long long INF = (1LL << 60);

    long long bestCost = INF;
    int bestColor = 1;

    DSU dsu(N);

    for (int color = 1; color <= M; ++color) {
        dsu.init(N);

        for (int id : byColor[color]) {
            dsu.unite(edges[id].a, edges[id].b);
        }

        if (dsu.comps == 1) {
            bestCost = 0;
            bestColor = color;
            break;
        }

        long long curCost = 0;

        for (int id : order) {
            const Edge &e = edges[id];
            if (e.c == color) continue;

            if (dsu.unite(e.a, e.b)) {
                curCost += e.p;

                if (curCost >= bestCost) break;
                if (dsu.comps == 1) break;
            }
        }

        if (dsu.comps == 1 && curCost < bestCost) {
            bestCost = curCost;
            bestColor = color;
        }
    }

    vector<int> operations;
    long long finalCost = 0;

    dsu.init(N);

    for (int id : byColor[bestColor]) {
        dsu.unite(edges[id].a, edges[id].b);
    }

    for (int id : order) {
        const Edge &e = edges[id];
        if (e.c == bestColor) continue;

        if (dsu.unite(e.a, e.b)) {
            operations.push_back(e.idx);
            finalCost += e.p;
            if (dsu.comps == 1) break;
        }
    }

    cout << finalCost << ' ' << bestColor << ' ' << operations.size() << '\n';
    for (int idx : operations) {
        cout << idx << '\n';
    }

    return 0;
}
