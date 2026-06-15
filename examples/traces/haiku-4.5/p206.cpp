#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, cost, id, type;
};

vector<pair<int,int>> tree[65];
int parent[65];
int parent_cost[65];

bool dfs_find_path(int u, int target, int p, vector<pair<int,int>>& path) {
    if (u == target) return true;
    
    for (auto [v, cost] : tree[u]) {
        if (v == p) continue;
        path.push_back({u, cost});
        if (dfs_find_path(v, target, u, path)) return true;
        path.pop_back();
    }
    return false;
}

int find_max_on_path(int u, int v) {
    vector<pair<int,int>> path;
    dfs_find_path(u, v, -1, path);
    int maxc = 0;
    for (auto [node, cost] : path) {
        maxc = max(maxc, cost);
    }
    return maxc;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<Edge> edges(m);
    
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].cost;
        edges[i].id = i;
        edges[i].type = (i < n - 1) ? 0 : 1; // 0 = stone, 1 = country
    }
    
    // Build tree from stone roads
    for (int i = 0; i < n - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        tree[u].push_back({v, edges[i].cost});
        tree[v].push_back({u, edges[i].cost});
    }
    
    // Assign costs
    vector<int> result(m);
    
    // Stone roads keep original cost
    for (int i = 0; i < n - 1; i++) {
        result[i] = edges[i].cost;
    }
    
    // Country roads: set to max cost on their tree path
    for (int i = n - 1; i < m; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int max_on_path = find_max_on_path(u, v);
        result[i] = max_on_path;
    }
    
    for (int i = 0; i < m; i++) {
        cout << result[i] << "\n";
    }
    
    return 0;
}
