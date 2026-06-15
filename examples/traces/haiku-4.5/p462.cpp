#include<bits/stdc++.h>
using namespace std;

struct Wire {
    int a, b, r, p, idx;
};

class DSU {
public:
    vector<int> parent, rank;
    vector<int> minRelIdx; // index of minimum reliability wire in component
    
    DSU(int n) : parent(n), rank(n, 0), minRelIdx(n) {
        iota(parent.begin(), parent.end(), 0);
        iota(minRelIdx.begin(), minRelIdx.end(), 0);
    }
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y, int wireIdx, const vector<Wire>& wires) {
        x = find(x);
        y = find(y);
        if (x == y) return false; // cycle detected
        
        if (rank[x] < rank[y]) swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
        
        int minIdx = minRelIdx[x];
        int minIdx2 = minRelIdx[y];
        if (wires[minIdx2].r < wires[minIdx].r || 
            (wires[minIdx2].r == wires[minIdx].r && minIdx2 < minIdx)) {
            minIdx = minIdx2;
        }
        minRelIdx[x] = minIdx;
        
        return true;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Wire> wires(n);
    for(int i = 0; i < n; i++) {
        cin >> wires[i].a >> wires[i].b >> wires[i].r >> wires[i].p;
        wires[i].idx = i + 1;
    }
    
    // Sort by cost descending (we want to keep expensive wires)
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        return wires[i].p > wires[j].p;
    });
    
    DSU dsu(1000001);
    vector<int> result;
    long long totalCost = 0;
    vector<bool> burned(n, false);
    
    for(int idx : order) {
        Wire& w = wires[idx];
        int u = dsu.find(w.a);
        int v = dsu.find(w.b);
        
        if(u == v) {
            // Would create cycle - this wire burns
            burned[idx] = true;
        } else {
            dsu.unite(u, v, idx, wires);
            result.push_back(w.idx);
            totalCost += w.p;
        }
    }
    
    cout << totalCost << "\n";
    for(int i = 0; i < result.size(); i++) {
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
