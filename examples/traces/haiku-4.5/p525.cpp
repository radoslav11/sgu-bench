#include<bits/stdc++.h>
using namespace std;

int n, m;
vector<pair<int,int>> edges;
vector<vector<int>> adj, radj;

void dfs1(int u, vector<vector<int>>& g, vector<bool>& vis, stack<int>& st) {
    vis[u] = true;
    for(int v : g[u]) {
        if(!vis[v]) dfs1(v, g, vis, st);
    }
    st.push(u);
}

void dfs2(int u, vector<vector<int>>& g, vector<bool>& vis, vector<int>& comp, int id) {
    vis[u] = true;
    comp[u] = id;
    for(int v : g[u]) {
        if(!vis[v]) dfs2(v, g, vis, comp, id);
    }
}

int getLargestSCC(vector<vector<int>>& g, vector<vector<int>>& gr) {
    vector<bool> vis(n+1, false);
    stack<int> st;
    
    for(int i = 1; i <= n; i++) {
        if(!vis[i]) dfs1(i, g, vis, st);
    }
    
    fill(vis.begin(), vis.end(), false);
    vector<int> comp(n+1, -1);
    int compId = 0;
    vector<int> compSize(n+1, 0);
    
    while(!st.empty()) {
        int u = st.top();
        st.pop();
        if(!vis[u]) {
            dfs2(u, gr, vis, comp, compId);
            compId++;
        }
    }
    
    for(int i = 1; i <= n; i++) {
        compSize[comp[i]]++;
    }
    
    int maxSize = 0;
    for(int i = 0; i < compId; i++) {
        maxSize = max(maxSize, compSize[i]);
    }
    
    return maxSize;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m;
    
    edges.resize(m);
    adj.resize(n+1);
    radj.resize(n+1);
    
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        edges[i] = {a, b};
        adj[a].push_back(b);
        radj[b].push_back(a);
    }
    
    int baseMax = getLargestSCC(adj, radj);
    int w = baseMax;
    vector<int> result;
    
    for(int i = 0; i < m; i++) {
        // Try reversing edge i
        int u = edges[i].first;
        int v = edges[i].second;
        
        // Remove original edge
        adj[u].erase(find(adj[u].begin(), adj[u].end(), v));
        radj[v].erase(find(radj[v].begin(), radj[v].end(), u));
        
        // Add reversed edge
        adj[v].push_back(u);
        radj[u].push_back(v);
        
        int newMax = getLargestSCC(adj, radj);
        
        if(newMax > w) {
            w = newMax;
            result.clear();
            result.push_back(i+1);
        } else if(newMax == w) {
            result.push_back(i+1);
        }
        
        // Restore original edge
        adj[v].erase(find(adj[v].begin(), adj[v].end(), u));
        radj[u].erase(find(radj[u].begin(), radj[u].end(), v));
        adj[u].push_back(v);
        radj[v].push_back(u);
    }
    
    cout << w << "\n";
    cout << result.size() << "\n";
    for(int idx : result) {
        cout << idx << " ";
    }
    if(result.size() > 0) cout << "\n";
    
    return 0;
}
