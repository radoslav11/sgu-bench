#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int q, c, n;
    long long t;
    cin >> q >> c >> n >> t;
    
    map<pair<int,int>, pair<int,int>> grid; // {x,y} -> {culture, generation}
    
    for(int i = 0; i < n; i++){
        int x, y;
        cin >> x >> y;
        grid[{x, y}] = {i, 0};
    }
    
    int dx[] = {0, -1, 1, 0, 0};
    int dy[] = {0, 0, 0, -1, 1};
    
    for(long long iter = 0; iter < t; iter++){
        map<pair<int,int>, pair<int,int>> newGrid = grid;
        bool changed = false;
        
        for(int culture = 0; culture < n; culture++){
            map<pair<int,int>, int> newCells;
            
            for(auto& [pos, info] : grid){
                if(info.first != culture) continue;
                
                int x = pos.first, y = pos.second, gen = info.second;
                
                for(int d = 0; d < 5; d++){
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    
                    if(nx < 0 || nx >= q || ny < 0 || ny >= c) continue;
                    if(newGrid.count({nx, ny}) && newGrid[{nx, ny}].first != culture) continue;
                    
                    newCells[{nx, ny}] = max(newCells[{nx, ny}], gen + 1);
                }
            }
            
            for(auto& [pos, gen] : newCells){
                if(!newGrid.count(pos) || newGrid[pos].first == culture){
                    newGrid[pos] = {culture, gen};
                    changed = true;
                }
            }
        }
        
        if(!changed) break;
        grid = newGrid;
    }
    
    vector<int> count(n, 0);
    for(auto& [pos, info] : grid) count[info.first]++;
    
    for(int i = 0; i < n; i++) cout << count[i] << "\n";
    
    return 0;
}
