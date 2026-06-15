#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Edge {
    int a, b, c, p, idx;
};

struct DSU {
    vector<int> parent;
    vector<int> rank;
    int comp;
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        comp = n;
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        if (rank[x] < rank[y]) {
            swap(x, y);
        }
        parent[y] = x;
        if (rank[x] == rank[y]) {
            rank[x]++;
        }
        comp--;
        return true;
    }
};

int main() {
    scanf("%d %d %d", &N, &M, &K);
    
    vector<Edge> edges;
    vector<vector<Edge>> airlineEdges(M + 1);
    
    for (int i = 0; i < K; i++) {
        int a, b, c, p;
        scanf("%d %d %d %d", &a, &b, &c, &p);
        Edge e = {a, b, c, p, i + 1};
        edges.push_back(e);
        airlineEdges[c].push_back(e);
    }
    
    vector<Edge> global_sorted = edges;
    sort(global_sorted.begin(), global_sorted.end(), [](const Edge& x, const Edge& y) {
        return x.p < y.p;
    });
    
    long long best_cost = 1e18;
    int best_R = -1;
    vector<int> best_ops;
    
    for (int R = 1; R <= M; R++) {
        DSU dsu(N);
        long long cost = 0;
        vector<int> ops;
        
        for (Edge& e : airlineEdges[R]) {
            dsu.unite(e.a - 1, e.b - 1);
        }
        
        if (dsu.comp == 1) {
            if (cost < best_cost) {
                best_cost = cost;
                best_R = R;
                best_ops = ops;
            }
            continue;
        }
        
        for (Edge& e : global_sorted) {
            if (e.c == R) continue;
            int u = e.a - 1;
            int v = e.b - 1;
            if (dsu.find(u) == dsu.find(v)) continue;
            dsu.unite(u, v);
            cost += e.p;
            ops.push_back(e.idx);
            if (dsu.comp == 1) break;
        }
        
        if (dsu.comp != 1) continue;
        
        if (cost < best_cost) {
            best_cost = cost;
            best_R = R;
            best_ops = ops;
        }
    }
    
    printf("%lld %d %d\n", best_cost, best_R, (int)best_ops.size());
    for (int idx : best_ops) {
        printf("%d\n", idx);
    }
    
    return 0;
}
