#include<bits/stdc++.h>
using namespace std;

int N, M;
vector<int> adj[10005];
bool visited[10005];
vector<int> path;

bool dfs(int v, int parent, int start, int depth) {
    visited[v] = true;
    path.push_back(v);
    
    if (depth == N) {
        for (int u : adj[v]) {
            if (u == start) {
                return true;
            }
        }
        path.pop_back();
        visited[v] = false;
        return false;
    }
    
    for (int u : adj[v]) {
        if (u == parent) continue;
        if (!visited[u]) {
            if (dfs(u, v, start, depth + 1)) {
                return true;
            }
        }
    }
    
    path.pop_back();
    visited[v] = false;
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    for (int start = 1; start <= N; start++) {
        memset(visited, false, sizeof(visited));
        path.clear();
        
        if (dfs(start, -1, start, 1)) {
            for (int i = 0; i < path.size(); i++) {
                if (i > 0) cout << " ";
                cout << path[i];
            }
            cout << "\n";
            return 0;
        }
    }
    
    cout << "-1\n";
    return 0;
}
