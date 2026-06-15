#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, cost, airline, idx;
};

class DSU {
public:
    vector<int> parent, rank;
    
    DSU(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        for(int i = 0; i <= n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if(parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if(px == py) return false;
        if(rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if(rank[px] == rank[py]) rank[px]++;
        return true;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<Edge> edges(K);
    for(int i = 0; i < K; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].airline >> edges[i].cost;
        edges[i].idx = i + 1;
    }
    
    int bestAirline = -1;
    long long minCost = LLONG_MAX;
    vector<int> bestTransfers;
    
    // Try each airline as the final one
    for(int airline = 1; airline <= M; airline++) {
        // Create edge list with costs: 0 for this airline, original cost for others
        vector<pair<long long, int>> sortedEdges;
        for(int i = 0; i < K; i++) {
            long long cost = (edges[i].airline == airline) ? 0 : edges[i].cost;
            sortedEdges.push_back({cost, i});
        }
        
        sort(sortedEdges.begin(), sortedEdges.end());
        
        DSU dsu(N);
        long long totalCost = 0;
        vector<int> transfers;
        int edgesAdded = 0;
        
        for(auto [cost, idx] : sortedEdges) {
            if(dsu.unite(edges[idx].u, edges[idx].v)) {
                totalCost += cost;
                if(edges[idx].airline != airline) {
                    transfers.push_back(edges[idx].idx);
                }
                edgesAdded++;
                if(edgesAdded == N - 1) break;
            }
        }
        
        if(edgesAdded == N - 1 && totalCost < minCost) {
            minCost = totalCost;
            bestAirline = airline;
            bestTransfers = transfers;
        }
    }
    
    cout << minCost << " " << bestAirline << " " << bestTransfers.size() << "\n";
    for(int idx : bestTransfers) {
        cout << idx << "\n";
    }
    
    return 0;
}
