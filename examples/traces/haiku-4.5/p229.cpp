#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<string> grid(n);
    vector<pair<int,int>> cells;
    
    for(int i = 0; i < n; i++){
        cin >> grid[i];
        for(int j = 0; j < n; j++){
            if(grid[i][j] == '1'){
                cells.push_back({i, j});
            }
        }
    }
    
    int total = cells.size();
    
    // Try all non-empty proper subsets
    for(int mask = 1; mask < (1 << total) - 1; mask++){
        vector<pair<int,int>> subset_a;
        set<pair<int,int>> subset_b_set;
        
        for(int i = 0; i < total; i++){
            if(mask & (1 << i)){
                subset_a.push_back(cells[i]);
            } else {
                subset_b_set.insert(cells[i]);
            }
        }
        
        // Try all 4 rotations
        for(int rot = 0; rot < 4; rot++){
            vector<pair<int,int>> rotated = subset_a;
            
            // Apply rotation rot times
            for(int r = 0; r < rot; r++){
                for(auto& p : rotated){
                    // Rotate 90 degrees: (r,c) -> (c, n-1-r)
                    int nr = p.second;
                    int nc = n - 1 - p.first;
                    p = {nr, nc};
                }
            }
            
            // Try all translations
            if(!rotated.empty()){
                // Get all possible translations
                set<pair<int,int>> rotated_set(rotated.begin(), rotated.end());
                
                for(int dr = -n; dr <= n; dr++){
                    for(int dc = -n; dc <= n; dc++){
                        vector<pair<int,int>> translated;
                        bool valid = true;
                        
                        for(auto p : rotated){
                            int nr = p.first + dr;
                            int nc = p.second + dc;
                            if(nr < 0 || nr >= n || nc < 0 || nc >= n){
                                valid = false;
                                break;
                            }
                            translated.push_back({nr, nc});
                        }
                        
                        if(!valid) continue;
                        
                        set<pair<int,int>> translated_set(translated.begin(), translated.end());
                        
                        if(translated_set == subset_b_set){
                            // Found a valid partition
                            cout << "YES\n";
                            for(int i = 0; i < n; i++){
                                for(int j = 0; j < n; j++){
                                    bool in_a = false;
                                    for(auto p : subset_a){
                                        if(p.first == i && p.second == j){
                                            in_a = true;
                                            break;
                                        }
                                    }
                                    cout << (in_a ? '1' : '0');
                                }
                                cout << "\n";
                            }
                            return 0;
                        }
                    }
                }
            }
        }
    }
    
    cout << "NO\n";
    return 0;
}
