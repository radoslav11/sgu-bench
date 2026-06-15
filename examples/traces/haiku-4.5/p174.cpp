#include <bits/stdc++.h>
using namespace std;

class DSU {
public:
    map<pair<long long, long long>, pair<long long, long long>> parent;
    
    pair<long long, long long> find(pair<long long, long long> x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
        }
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(pair<long long, long long> x, pair<long long, long long> y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false; // already connected, creates cycle
        }
        parent[x] = y;
        return true;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int M;
    cin >> M;
    
    DSU dsu;
    int answer = 0;
    
    for (int i = 1; i <= M; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        pair<long long, long long> p1 = {x1, y1};
        pair<long long, long long> p2 = {x2, y2};
        
        if (!dsu.unite(p1, p2)) {
            // Wall creates a cycle
            answer = i;
            break;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}
