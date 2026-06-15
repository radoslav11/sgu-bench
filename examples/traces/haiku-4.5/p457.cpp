#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int to, rev, snow, is_hist;
};

vector<Edge> graph[105];
int n, m, A, B;
vector<pair<int,int>> hist_edges;
int snow[5005];
bool is_hist[5005];
vector<vector<int>> paths;

bool dfs_path(int v, int target, vector<int>& path, vector<bool>& visited) {
    if (v == target) return true;
    visited[v] = true;
    
    for (auto& e : graph[v]) {
        if (!visited[e.to] && snow[&e - (Edge*)nullptr] > 0) {
            path.push_back(e.to);
            if (dfs_path(e.to, target, path, visited)) return true;
            path.pop_back();
        }
    }
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m >> A >> B;
    
    vector<tuple<int,int,int,int>> edges;
    
    for(int i = 0; i < m; i++) {
        int x, y, w, t;
        cin >> x >> y >> w >> t;
        edges.push_back({x, y, w, t});
        snow[i] = w;
        is_hist[i] = (t == 1);
        if (t == 1) hist_edges.push_back({x, y});
    }
    
    // Build adjacency list with edge indices
    map<pair<int,int>, int> edge_id;
    for(int i = 0; i < m; i++) {
        auto [x, y, w, t] = edges[i];
        edge_id[{x, y}] = i;
        graph[x].push_back({y, i, w, t});
    }
    
    int day = 0;
    
    // Greedily find paths from A to B
    while(true) {
        vector<int> path = {A};
        vector<bool> visited(n + 1, false);
        
        if (!dfs_path(A, B, path, visited)) {
            break;
        }
        
        // Found a path, reduce snow on all edges in path
        for(int i = 0; i < (int)path.size() - 1; {
            int u = path[i];
            int v = path[i + 1];
            int eid = edge_id[{u, v}];
            snow[eid]--;
            i++;
        }
        
        paths.push_back(path);
        day++;
    }
    
    // Check if all historical roads are cleaned
    bool valid = true;
    for(int i = 0; i < m; i++) {
        if (is_hist[i] && snow[i] > 0) {
            valid = false;
            break;
        }
    }
    
    if (!valid) {
        cout << 0 << "\n";
    } else {
        cout << day << "\n";
        for(auto& path : paths) {
            for(int i = 0; i < path.size(); i++) {
                if (i > 0) cout << " ";
                cout << path[i];
            }
            cout << "\n";
        }
    }
    
    return 0;
}
