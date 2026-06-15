#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;

    DSU() {}
    DSU(int n) { init(n); }

    void init(int n) {
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int v) {
        if (p[v] == v) return v;
        return p[v] = find(p[v]);
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

struct Edge {
    long long a, b, r, cost;
    int u, v;
    int id;
    bool keep = false;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Edge> e(n);
    vector<long long> coords;

    for (int i = 0; i < n; i++) {
        cin >> e[i].a >> e[i].b >> e[i].r >> e[i].cost;
        e[i].id = i;
        coords.push_back(e[i].a);
        coords.push_back(e[i].b);
    }

    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());

    for (int i = 0; i < n; i++) {
        e[i].u = lower_bound(coords.begin(), coords.end(), e[i].a) - coords.begin();
        e[i].v = lower_bound(coords.begin(), coords.end(), e[i].b) - coords.begin();
    }

    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);

    sort(ord.begin(), ord.end(), [&](int x, int y) {
        return e[x].r > e[y].r;
    });

    DSU global((int)coords.size());

    long long answer = 0;
    vector<vector<int>> groups;

    for (int l = 0; l < n; ) {
        int r = l;
        while (r < n && e[ord[r]].r == e[ord[l]].r) r++;

        vector<int> group;
        for (int i = l; i < r; i++) group.push_back(ord[i]);
        groups.push_back(group);

        vector<int> roots;
        struct Cand {
            int id;
            int ru, rv;
        };
        vector<Cand> cand;

        for (int id : group) {
            int ru = global.find(e[id].u);
            int rv = global.find(e[id].v);

            if (ru == rv) continue;

            cand.push_back({id, ru, rv});
            roots.push_back(ru);
            roots.push_back(rv);
        }

        sort(roots.begin(), roots.end());
        roots.erase(unique(roots.begin(), roots.end()), roots.end());

        auto getRootId = [&](int x) {
            return int(lower_bound(roots.begin(), roots.end(), x) - roots.begin());
        };

        sort(cand.begin(), cand.end(), [&](const Cand &x, const Cand &y) {
            return e[x.id].cost > e[y.id].cost;
        });

        DSU temp((int)roots.size());

        vector<int> selected;

        for (auto &c : cand) {
            int x = getRootId(c.ru);
            int y = getRootId(c.rv);

            if (temp.unite(x, y)) {
                e[c.id].keep = true;
                answer += e[c.id].cost;
                selected.push_back(c.id);
            }
        }

        for (int id : selected) {
            global.unite(e[id].u, e[id].v);
        }

        l = r;
    }

    cout << answer << '\n';

    vector<int> result;

    for (auto &group : groups) {
        for (int id : group) {
            if (!e[id].keep) result.push_back(id + 1);
        }
        for (int id : group) {
            if (e[id].keep) result.push_back(id + 1);
        }
    }

    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << result[i];
    }
    cout << '\n';

    return 0;
}
