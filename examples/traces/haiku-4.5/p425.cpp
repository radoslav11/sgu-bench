#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> T(n, vector<int>(m));
    set<int> unique_vals;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> T[i][j];
            unique_vals.insert(T[i][j]);
        }
    }
    
    // Build conflict graph
    map<int, set<int>> conflicts;
    for(int val : unique_vals){
        conflicts[val] = set<int>();
    }
    
    // For each column, first row value must differ from all other rows
    for(int j = 0; j < m; j++){
        for(int i = 1; i < n; i++){
            if(T[0][j] != T[i][j]){
                conflicts[T[0][j]].insert(T[i][j]);
                conflicts[T[i][j]].insert(T[0][j]);
            }
        }
    }
    
    // Try to color the graph greedily
    map<int, int> color;
    vector<int> sorted_vals(unique_vals.begin(), unique_vals.end());
    
    for(int val : sorted_vals){
        set<int> used_colors;
        for(int neighbor : conflicts[val]){
            if(color.count(neighbor)){
                used_colors.insert(color[neighbor]);
            }
        }
        
        // Find smallest available color
        int c = 0;
        while(used_colors.count(c)) c++;
        
        if(c > 50){
            cout << "No\n";
            return 0;
        }
        
        color[val] = c;
    }
    
    // Verify the solution
    for(int j = 0; j < m; j++){
        for(int i = 1; i < n; i++){
            if(color[T[0][j]] == color[T[i][j]]){
                cout << "No\n";
                return 0;
            }
        }
    }
    
    cout << "Yes\n";
    for(int val : sorted_vals){
        cout << val << " -> " << color[val] << "\n";
    }
    
    return 0;
}
