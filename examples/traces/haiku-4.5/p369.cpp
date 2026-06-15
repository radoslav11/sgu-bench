#include<bits/stdc++.h>
using namespace std;

class DSU {
public:
    map<long long, long long> parent;
    
    long long find(long long x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
        }
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(long long x, long long y) {
        x = find(x);
        y = find(y);
        if (x != y) {
            parent[x] = y;
        }
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    cin >> K;
    
    DSU dsu;
    set<long long> xs, ys;
    
    for (int i = 0; i < K; i++) {
        long long x, y;
        cin >> x >> y;
        xs.insert(x);
        ys.insert(y);
        
        // Use negative numbers for y to distinguish from x
        dsu.unite(x, -y - 1);
    }
    
    // Group by component
    map<long long, pair<set<long long>, set<long long>>> components;
    
    for (long long x : xs) {
        long long comp = dsu.find(x);
        components[comp].first.insert(x);
    }
    
    for (long long y : ys) {
        long long comp = dsu.find(-y - 1);
        components[comp].second.insert(y);
    }
    
    long long answer = 0;
    for (auto& p : components) {
        long long nx = p.second.first.size();
        long long ny = p.second.second.size();
        answer += nx * ny;
    }
    
    cout << answer << endl;
    
    return 0;
}
