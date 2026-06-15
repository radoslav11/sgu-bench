#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w, id;
};

struct DSU {
    vector<int> p, sz;

    DSU() {}
    DSU(int n) { init(n); }

    void init(int n) {
        p.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m + 1);
    for (int i = 1; i <= m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].id = i;
    }

    int t;
    cin >> t;

    vector<int> qEdge(t), qCost(t);
    for (int i = 0; i < t; i++) {
        cin >> qEdge[i] >> qCost[i];
    }

    const int BLOCK = 700;

    vector<char> special(m + 1, false);

    for (int L = 0; L < t; L += BLOCK) {
        int R = min(t, L + BLOCK);

        vector<int> spec;
        for (int i = L; i < R; i++) {
            int e = qEdge[i];
            if (!special[e]) {
                special[e] = true;
                spec.push_back(e);
            }
        }

        vector<Edge> ordinary;
        ordinary.reserve(m);

        for (int i = 1; i <= m; i++) {
            if (!special[i]) ordinary.push_back(edges[i]);
        }

        sort(ordinary.begin(), ordinary.end(), [](const Edge& a, const Edge& b) {
            if (a.w != b.w) return a.w < b.w;
            return a.id < b.id;
        });

        /*
            Candidate fixed edges.

            All edges changed inside the current block are considered "special".
            Their weights may vary from query to query.

            If we put all special edges before ordinary edges in Kruskal,
            every ordinary edge that is still chosen is potentially relevant.
            Other ordinary edges can never appear in an MST during this block.
        */
        DSU dsu(n);
        for (int id : spec) {
            dsu.unite(edges[id].u, edges[id].v);
        }

        vector<Edge> cand;
        cand.reserve(n);

        for (const Edge& e : ordinary) {
            if (dsu.unite(e.u, e.v)) {
                cand.push_back(e);
            }
        }

        sort(cand.begin(), cand.end(), [](const Edge& a, const Edge& b) {
            if (a.w != b.w) return a.w < b.w;
            return a.id < b.id;
        });

        for (int qi = L; qi < R; qi++) {
            int eid = qEdge[qi];
            int nw = qCost[qi];
            edges[eid].w = nw;

            vector<Edge> curSpec;
            curSpec.reserve(spec.size());
            for (int id : spec) curSpec.push_back(edges[id]);

            sort(curSpec.begin(), curSpec.end(), [](const Edge& a, const Edge& b) {
                if (a.w != b.w) return a.w < b.w;
                return a.id < b.id;
            });

            DSU mst(n);
            long long ans = 0;
            int used = 0;

            size_t i = 0, j = 0;

            while ((i < cand.size() || j < curSpec.size()) && used < n - 1) {
                Edge e;

                if (j == curSpec.size() ||
                    (i < cand.size() &&
                     (cand[i].w < curSpec[j].w ||
                      (cand[i].w == curSpec[j].w && cand[i].id < curSpec[j].id)))) {
                    e = cand[i++];
                } else {
                    e = curSpec[j++];
                }

                if (mst.unite(e.u, e.v)) {
                    ans += e.w;
                    used++;
                }
            }

            cout << ans << '\n';
        }

        for (int id : spec) special[id] = false;
    }

    return 0;
}
