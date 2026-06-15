#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> grid(N);
    for(int i = 0; i < N; i++){
        cin >> grid[i];
    }
    
    bool changed = true;
    while(changed){
        changed = false;
        vector<string> newGrid = grid;
        
        for(int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                if(grid[i][j] == '.'){
                    int weedCount = 0;
                    // Check all 4 adjacent cells
                    if(i > 0 && grid[i-1][j] == 'X') weedCount++;
                    if(i < N-1 && grid[i+1][j] == 'X') weedCount++;
                    if(j > 0 && grid[i][j-1] == 'X') weedCount++;
                    if(j < M-1 && grid[i][j+1] == 'X') weedCount++;
                    
                    if(weedCount >= 2){
                        newGrid[i][j] = 'X';
                        changed = true;
                    }
                }
            }
        }
        grid = newGrid;
    }
    
    int count = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(grid[i][j] == 'X') count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
