#include <bits/stdc++.h>
using namespace std;

struct UF {
    map<int,int> parent, rank_;
    int find(int x) {
        if (parent.find(x) == parent.end()) { parent[x] = x; rank_[x] = 0; }
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rank_[a] < rank_[b]) swap(a,b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int K;
    cin >> K;
    
    vector<pair<int,int>> cells(K);
    for (int i = 0; i < K; i++) {
        cin >> cells[i].first >> cells[i].second;
    }
    
    // Use offset to distinguish x-coords from y-coords
    // x-coords: use as-is with tag 0, y-coords: tag 1
    // Encode: x -> (x, 0), y -> (y, 1) but UF uses int
    // Use a map from (val, type) to id
    
    UF uf;
    map<pair<int,int>, int> id_map;
    int cnt = 0;
    auto get_id = [&](int val, int type) -> int {
        auto key = make_pair(val, type);
        if (id_map.find(key) == id_map.end()) id_map[key] = cnt++;
        return id_map[key];
    };
    
    for (auto& [x,y] : cells) {
        int ix = get_id(x, 0);
        int iy = get_id(y, 1);
        uf.find(ix); uf.find(iy);
        uf.unite(ix, iy);
    }
    
    // For each component, count rows and cols
    map<int, pair<long long,long long>> comp; // root -> (row_count, col_count)
    for (auto& [key, id] : id_map) {
        int root = uf.find(id);
        if (key.second == 0) comp[root].first++;
        else comp[root].second++;
    }
    
    long long ans = 0;
    for (auto& [root, rc] : comp) {
        ans += rc.first * rc.second;
    }
    
    cout << ans << endl;
    return 0;
}
