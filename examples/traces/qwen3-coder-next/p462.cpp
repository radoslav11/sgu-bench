#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <numeric>
using namespace std;

struct Edge {
    int a, b, r, p, idx;
};

struct DSU {
    vector<int> parent;
    vector<int> sz;
    DSU(int n) {
        parent.resize(n);
        sz.resize(n, 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y])
            swap(x, y);
        parent[y] = x;
        sz[x] += sz[y];
        return true;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Edge> edges(n);
    vector<int> all_nodes;
    for (int i = 0; i < n; i++) {
        cin >> edges[i].a >> edges[i].b >> edges[i].r >> edges[i].p;
        edges[i].idx = i;
        all_nodes.push_back(edges[i].a);
        all_nodes.push_back(edges[i].b);
    }

    // Compress node coordinates
    sort(all_nodes.begin(), all_nodes.end());
    all_nodes.erase(unique(all_nodes.begin(), all_nodes.end()), all_nodes.end());
    auto get_id = [&](int x) {
        return lower_bound(all_nodes.begin(), all_nodes.end(), x) - all_nodes.begin();
    };

    // Step 1: Sort edges by reliability descending for DSU1
    vector<Edge> e1 = edges;
    sort(e1.begin(), e1.end(), [](const Edge& x, const Edge& y) {
        return x.r > y.r;
    });

    DSU dsu1(all_nodes.size());
    vector<bool> is_keepable(n, false);
    for (int i = 0; i < n; i++) {
        int a = get_id(e1[i].a);
        int b = get_id(e1[i].b);
        if (dsu1.find(a) != dsu1.find(b)) {
            dsu1.unite(a, b);
            is_keepable[e1[i].idx] = true;
        } else {
            is_keepable[e1[i].idx] = false;
        }
    }

    // Step 2: Collect keepable edges and sort by cost descending for DSU2
    vector<Edge> keepable_edges;
    for (int i = 0; i < n; i++) {
        if (is_keepable[i]) {
            keepable_edges.push_back(edges[i]);
        }
    }
    sort(keepable_edges.begin(), keepable_edges.end(), [](const Edge& x, const Edge& y) {
        return x.p > y.p;
    });

    DSU dsu2(all_nodes.size());
    vector<bool> in_forest(n, false);
    for (int i = 0; i < keepable_edges.size(); i++) {
        int a = get_id(keepable_edges[i].a);
        int b = get_id(keepable_edges[i].b);
        if (dsu2.find(a) != dsu2.find(b)) {
            dsu2.unite(a, b);
            in_forest[keepable_edges[i].idx] = true;
        }
    }

    // Compute total cost
    long long total_cost = 0;
    for (int i = 0; i < n; i++) {
        if (in_forest[i]) {
            total_cost += edges[i].p;
        }
    }

    // Step 3: Build soldering order
    vector<int> order_indices(n);
    for (int i = 0; i < n; i++) {
        order_indices[i] = i;
    }
    sort(order_indices.begin(), order_indices.end(), [&](int i, int j) {
        if (edges[i].r != edges[j].r) {
            return edges[i].r < edges[j].r;
        }
        // For same reliability: non-forest (in_forest[i]=false) comes before forest (true)
        return in_forest[i] < in_forest[j];
    });

    // Output
    cout << total_cost << '\n';
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << order_indices[i] + 1;
    }
    cout << '\n';

    return 0;
}
