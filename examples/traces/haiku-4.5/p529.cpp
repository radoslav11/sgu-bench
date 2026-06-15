#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, cost;
    int id;
};

class UnionFind {
public:
    vector<int> parent, rank;
    
    UnionFind(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        for(int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if(parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if(px == py) return false;
        
        if(rank[px] < rank[py]) {
            parent[px] = py;
        } else if(rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
        return true;
    }
};

long long calculateMST(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });
    
    UnionFind uf(n);
    long long totalCost = 0;
    int edgesUsed = 0;
    
    for(const auto& edge : edges) {
        if(uf.unite(edge.u, edge.v)) {
            totalCost += edge.cost;
            edgesUsed++;
            if(edgesUsed == n - 1) break;
        }
    }
    
    return totalCost;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<Edge> edges(m);
    
    for(int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].cost;
        edges[i].id = i;
    }
    
    int t;
    cin >> t;
    
    for(int day = 0; day < t; day++) {
        int roadId, newCost;
        cin >> roadId >> newCost;
        roadId--; // Convert to 0-indexed
        
        edges[roadId].cost = newCost;
        
        long long mstCost = calculateMST(n, edges);
        cout << mstCost << "\n";
    }
    
    return 0;
}
