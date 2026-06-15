#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, sz;

    int add() {
        int id = (int)parent.size();
        parent.push_back(id);
        sz.push_back(1);
        return id;
    }

    int find(int v) {
        if (parent[v] == v) return v;
        return parent[v] = find(parent[v]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

struct Point {
    long long x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash {
    size_t operator()(const Point& p) const {
        uint64_t x = (uint64_t)(p.x + 1000000007LL);
        uint64_t y = (uint64_t)(p.y + 1000000009LL);

        x ^= x >> 23;
        x *= 0x2127599bf4325c37ULL;
        x ^= x >> 47;

        y ^= y >> 23;
        y *= 0x2127599bf4325c37ULL;
        y ^= y >> 47;

        return (size_t)(x ^ (y + 0x9e3779b97f4a7c15ULL + (x << 6) + (x >> 2)));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M;
    cin >> M;

    unordered_map<Point, int, PointHash> id;
    id.reserve(2 * M + 10);
    id.max_load_factor(0.7);

    DSU dsu;

    for (int i = 1; i <= M; ++i) {
        Point a, b;
        cin >> a.x >> a.y >> b.x >> b.y;

        if (!id.count(a)) id[a] = dsu.add();
        if (!id.count(b)) id[b] = dsu.add();

        int u = id[a];
        int v = id[b];

        if (dsu.find(u) == dsu.find(v)) {
            cout << i << '\n';
            return 0;
        }

        dsu.unite(u, v);
    }

    cout << 0 << '\n';
    return 0;
}
