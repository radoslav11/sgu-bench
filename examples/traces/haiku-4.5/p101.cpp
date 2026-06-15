#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<int,int>> dominoes(n);
    vector<pair<int,int>> adj[7]; // adjacency list: adj[i] = {(neighbor, domino_id)}
    vector<int> degree(7, 0);
    
    for(int i = 0; i < n; i++){
        int a, b;
        cin >> a >> b;
        dominoes[i] = {a, b};
        adj[a].push_back({b, i});
        if(a != b) adj[b].push_back({a, i});
        degree[a]++;
        degree[b]++;
    }
    
    // Find start node for Eulerian path
    int start = 0;
    int odd_count = 0;
    for(int i = 0; i <= 6; i++){
        if(degree[i] % 2 == 1){
            odd_count++;
            start = i;
        }
    }
    
    // Check if Eulerian path exists
    if(odd_count != 0 && odd_count != 2){
        cout << "No solution\n";
        return 0;
    }
    
    // If all vertices have even degree, start from any vertex with edges
    if(odd_count == 0){
        for(int i = 0; i <= 6; i++){
            if(degree[i] > 0){
                start = i;
                break;
            }
        }
    }
    
    // Hierholzer's algorithm
    vector<bool> used(n, false);
    vector<int> result;
    vector<bool> flipped;
    
    function<void(int)> dfs = [&](int v){
        while(!adj[v].empty()){
            auto [u, domino_id] = adj[v].back();
            adj[v].pop_back();
            
            if(used[domino_id]) continue;
            used[domino_id] = true;
            
            // Determine if we need to flip
            bool need_flip = (dominoes[domino_id].first != v);
            
            dfs(u);
            result.push_back(domino_id);
            flipped.push_back(need_flip);
        }
    };
    
    dfs(start);
    
    // Check if all dominoes were used
    for(int i = 0; i < n; i++){
        if(!used[i]){
            cout << "No solution\n";
            return 0;
        }
    }
    
    // Reverse to get correct order
    reverse(result.begin(), result.end());
    reverse(flipped.begin(), flipped.end());
    
    for(int i = 0; i < n; i++){
        cout << result[i] + 1 << " " << (flipped[i] ? "-" : "+") << "\n";
    }
    
    return 0;
}
