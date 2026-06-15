#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<pair<int,int>> edges;
    map<pair<int,int>, int> edgeIdx;
    
    for(int i = 1; i <= n; i++){
        int j;
        while(cin >> j && j != 0){
            adj[i].push_back(j);
        }
    }
    
    // Build edge list
    for(int i = 1; i <= n; i++){
        for(int j : adj[i]){
            if(i < j){
                int idx = edges.size();
                edges.push_back({i, j});
                edgeIdx[{i, j}] = idx;
            }
        }
    }
    
    vector<int> color(edges.size(), 0); // 0 = uncolored, 1 = white, 2 = black
    
    // For each node with degree >= 2, ensure both colors
    for(int i = 1; i <= n; i++){
        if(adj[i].size() >= 2){
            bool has1 = false, has2 = false;
            vector<int> edgeIndices;
            
            for(int j : adj[i]){
                int u = i, v = j;
                if(u > v) swap(u, v);
                int idx = edgeIdx[{u, v}];
                edgeIndices.push_back(idx);
                if(color[idx] == 1) has1 = true;
                if(color[idx] == 2) has2 = true;
            }
            
            if(!has1 && edgeIndices.size() > 0){
                color[edgeIndices[0]] = 1;
                has1 = true;
            }
            if(!has2 && edgeIndices.size() > 1){
                color[edgeIndices[1]] = 2;
                has2 = true;
            }
            
            if(!has1 || !has2){
                cout << "No solution\n";
                return 0;
            }
        }
    }
    
    // Fill remaining with 1
    for(int i = 0; i < color.size(); i++){
        if(color[i] == 0) color[i] = 1;
    }
    
    // Output
    for(int i = 1; i <= n; i++){
        for(int j : adj[i]){
            int u = i, v = j;
            if(u > v) swap(u, v);
            int idx = edgeIdx[{u, v}];
            cout << color[idx] << " ";
        }
        cout << "0\n";
    }
    
    return 0;
}
