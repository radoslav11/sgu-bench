#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<vector<pair<int,int>>> adj(4*N + 1); // adjacency list with edge indices
    vector<pair<int,int>> edges(2*N);
    vector<int> degree(4*N + 1, 0);
    
    for(int i = 0; i < 2*N; i++){
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        degree[u]++;
        degree[v]++;
    }
    
    // Check if all vertices with edges have even degree
    for(int i = 1; i <= 4*N; i++){
        if(degree[i] % 2 != 0){
            cout << "No\n";
            return 0;
        }
    }
    
    // Hierholzer's algorithm
    vector<bool> used(2*N, false);
    stack<int> st;
    vector<int> result;
    
    st.push(1);
    
    while(!st.empty()){
        int u = st.top();
        bool found = false;
        
        for(auto [v, idx] : adj[u]){
            if(!used[idx]){
                used[idx] = true;
                st.push(v);
                found = true;
                break;
            }
        }
        
        if(!found){
            result.push_back(u);
            st.pop();
        }
    }
    
    // Check if all edges were used
    for(int i = 0; i < 2*N; i++){
        if(!used[i]){
            cout << "No\n";
            return 0;
        }
    }
    
    // Result is in reverse order
    reverse(result.begin(), result.end());
    
    // Check if path starts and ends at gate 1
    if(result[0] != 1 || result.back() != 1){
        cout << "No\n";
        return 0;
    }
    
    cout << "Yes\n";
    for(int i = 0; i < result.size(); i++){
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
