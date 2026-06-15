#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> rank;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        rank.assign(n, 0);
    }
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int M;
    cin >> M;
    
    DSU dsu(2 * M);
    map<pair<long long, long long>, int> point_to_id;
    int node_count = 0;
    
    for (int i = 0; i < M; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        pair<long long, long long> p1 = make_pair(x1, y1);
        pair<long long, long long> p2 = make_pair(x2, y2);
        
        if (point_to_id.find(p1) == point_to_id.end()) {
            point_to_id[p1] = node_count++;
        }
        if (point_to_id.find(p2) == point_to_id.end()) {
            point_to_id[p2] = node_count++;
        }
        
        int u = point_to_id[p1];
        int v = point_to_id[p2];
        
        if (dsu.find(u) == dsu.find(v)) {
            cout << i + 1 << '\n';
            return 0;
        } else {
            dsu.unite(u, v);
        }
    }
    
    cout << 0 << '\n';
    return 0;
}
