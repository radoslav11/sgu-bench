#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> T(n, vector<int>(m));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> T[i][j];
    
    // Collect all distinct values
    set<int> vals_set;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            vals_set.insert(T[i][j]);
    
    vector<int> vals(vals_set.begin(), vals_set.end());
    map<int,int> idx;
    for(int i = 0; i < (int)vals.size(); i++) idx[vals[i]] = i;
    
    int V = vals.size();
    vector<set<int>> adj(V);
    
    for(int j = 0; j < m; j++){
        int a = idx[T[0][j]];
        for(int i = 1; i < n; i++){
            int b = idx[T[i][j]];
            if(a != b){
                adj[a].insert(b);
                adj[b].insert(a);
            }
        }
    }
    
    // Greedy coloring
    vector<int> color(V, -1);
    for(int v = 0; v < V; v++){
        set<int> used;
        for(int u : adj[v]) if(color[u] >= 0) used.insert(color[u]);
        int c = 0;
        while(used.count(c)) c++;
        if(c > 50){ cout << "No\n"; return 0; }
        color[v] = c;
    }
    
    cout << "Yes\n";
    for(int i = 0; i < V; i++){
        cout << vals[i] << " -> " << color[i] << "\n";
    }
    return 0;
}
