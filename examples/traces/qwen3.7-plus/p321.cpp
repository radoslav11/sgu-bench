#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

struct Edge {
    int to;
    int type; // 1 for protected, -1 for almost protected
    int id;
};

int n;
vector<vector<Edge>> adj;
vector<char> flipped;
vector<int> ans;
set<pair<int, int>> avail; // {depth, edge_id}

void dfs(int u, int p, int current_S, int depth) {
    if (current_S < 0) {
        auto it = avail.begin();
        int id = it->second;
        avail.erase(it);
        flipped[id] = 1;
        ans.push_back(id);
        current_S += 2;
    }
    
    for (auto& edge : adj[u]) {
        int v = edge.to;
        if (v == p) continue;
        
        bool added = false;
        if (edge.type == -1) {
            avail.insert({depth, edge.id});
            added = true;
        }
        
        int next_S = current_S + edge.type;
        dfs(v, u, next_S, depth + 1);
        
        if (added && !flipped[edge.id]) {
            avail.erase({depth, edge.id});
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n)) return 0;
    
    adj.resize(n + 1);
    flipped.assign(n + 1, 0);
    
    for (int i = 1; i < n; ++i) {
        int u, v;
        string s1, s2;
        cin >> u >> v >> s1;
        int type = 1;
        if (s1 == "almost") {
            cin >> s2;
            type = -1;
        }
        // The direction is u -> v.
        // Since messages go to Kat (node 1), v is the parent of u.
        // So we traverse from v to u in our DFS.
        adj[v].push_back({u, type, i});
    }
    
    dfs(1, 0, 0, 0);
    
    cout << ans.size() << "\n";
    for (int i = 0; i < (int)ans.size(); ++i) {
        cout << ans[i] << (i + 1 == (int)ans.size() ? "" : " ");
    }
    cout << "\n";
    
    return 0;
}
